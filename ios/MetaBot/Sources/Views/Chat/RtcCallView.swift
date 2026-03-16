import AVFoundation
import MediaPlayer
import SwiftUI

/// Full-screen RTC voice call overlay.
/// Uses Volcengine RTC for real-time AI voice chat (ASR → LLM → TTS in cloud).
/// No local recording/VAD needed — all audio processing happens server-side.
struct RtcCallView: View {
    @Environment(AppState.self) private var appState
    @Environment(\.dismiss) private var dismiss

    let botName: String
    let chatId: String
    /// Non-nil for agent-initiated (incoming) calls
    let incoming: IncomingVoiceCall?

    @State private var rtcService = RtcVoiceService()
    @State private var callDuration: TimeInterval = 0
    @State private var callTimer: Timer?
    @State private var interruptionObserver: Any?

    init(botName: String, chatId: String, incoming: IncomingVoiceCall? = nil) {
        self.botName = botName
        self.chatId = chatId
        self.incoming = incoming
    }

    var body: some View {
        ZStack {
            // Background gradient
            LinearGradient(
                colors: [Color(red: 0.05, green: 0.1, blue: 0.15), Color(red: 0.02, green: 0.05, blue: 0.08)],
                startPoint: .top,
                endPoint: .bottom
            )
            .ignoresSafeArea()

            VStack(spacing: 0) {
                // Top bar
                HStack {
                    Button {
                        endCall()
                    } label: {
                        Image(systemName: "chevron.down")
                            .font(.title3)
                            .foregroundStyle(.white.opacity(0.7))
                    }
                    Spacer()
                    HStack(spacing: 6) {
                        Text(formattedDuration)
                            .font(.system(size: 14, design: .monospaced))
                            .foregroundStyle(.white.opacity(0.6))
                        // RTC badge
                        Text("RTC")
                            .font(.system(size: 10, weight: .bold))
                            .foregroundStyle(.white)
                            .padding(.horizontal, 6)
                            .padding(.vertical, 2)
                            .background(Color.accentColor.opacity(0.6))
                            .clipShape(Capsule())
                    }
                    Spacer()
                    Color.clear.frame(width: 24)
                }
                .padding(.horizontal, 24)
                .padding(.top, 16)

                Spacer()

                // Avatar + status
                VStack(spacing: 20) {
                    ZStack {
                        // Pulsing rings when connected
                        if rtcService.callPhase == .connected {
                            ForEach(0..<3, id: \.self) { i in
                                Circle()
                                    .stroke(Color.accentColor.opacity(0.15 - Double(i) * 0.05), lineWidth: 2)
                                    .frame(width: 120 + CGFloat(i) * 30, height: 120 + CGFloat(i) * 30)
                            }
                        }

                        GradientAvatar(name: botName, size: 100)
                    }

                    Text(botName)
                        .font(.system(size: 24, weight: .semibold))
                        .foregroundStyle(.white)

                    // Phase indicator
                    HStack(spacing: 8) {
                        switch rtcService.callPhase {
                        case .connecting:
                            ProgressView()
                                .tint(.white)
                                .scaleEffect(0.8)
                        case .connected:
                            Image(systemName: rtcService.isMuted ? "mic.slash.fill" : "waveform")
                                .foregroundStyle(Color.accentColor)
                                .symbolEffect(.variableColor.iterative)
                        case .error:
                            Image(systemName: "exclamationmark.triangle.fill")
                                .foregroundStyle(.red)
                        default:
                            EmptyView()
                        }

                        Text(rtcService.callPhase.displayText)
                            .font(.system(size: 15))
                            .foregroundStyle(.white.opacity(0.7))
                    }
                    .frame(height: 24)
                }

                Spacer()

                // Live subtitle display
                if !rtcService.subtitleText.isEmpty && rtcService.callPhase == .connected {
                    Text(rtcService.subtitleText)
                        .font(.system(size: 15))
                        .foregroundStyle(.white.opacity(0.8))
                        .multilineTextAlignment(.center)
                        .padding(.horizontal, 40)
                        .padding(.bottom, 20)
                        .transition(.opacity)
                        .animation(.easeInOut(duration: 0.2), value: rtcService.subtitleText)
                }

                // Error message
                if case .error(let msg) = rtcService.callPhase {
                    Text(msg)
                        .font(.caption)
                        .foregroundStyle(.red)
                        .padding(.bottom, 8)
                }

                // Bottom controls
                HStack(spacing: 60) {
                    // Mute
                    Button {
                        rtcService.toggleMute()
                    } label: {
                        ZStack {
                            Circle()
                                .fill(rtcService.isMuted ? .white : .white.opacity(0.15))
                                .frame(width: 56, height: 56)
                            Image(systemName: rtcService.isMuted ? "mic.slash.fill" : "mic.fill")
                                .font(.title3)
                                .foregroundStyle(rtcService.isMuted ? .black : .white)
                        }
                    }
                    .disabled(rtcService.callPhase != .connected)

                    // End call
                    Button {
                        endCall()
                    } label: {
                        ZStack {
                            Circle()
                                .fill(.red)
                                .frame(width: 64, height: 64)
                            Image(systemName: "phone.down.fill")
                                .font(.title2)
                                .foregroundStyle(.white)
                        }
                    }

                    // Speaker
                    Button {
                        // Toggle speaker (placeholder)
                    } label: {
                        ZStack {
                            Circle()
                                .fill(.white.opacity(0.15))
                                .frame(width: 56, height: 56)
                            Image(systemName: "speaker.wave.2.fill")
                                .font(.title3)
                                .foregroundStyle(.white)
                        }
                    }
                }
                .padding(.bottom, 50)
            }
        }
        .onAppear { startCall() }
        .onDisappear { cleanup() }
    }

    private var formattedDuration: String {
        let mins = Int(callDuration) / 60
        let secs = Int(callDuration) % 60
        return String(format: "%02d:%02d", mins, secs)
    }

    // MARK: - Call Lifecycle

    private func startCall() {
        // Prevent auto-lock during call
        UIApplication.shared.isIdleTimerDisabled = true

        // Set up audio interruption handling
        setupInterruptionHandling()

        // Set up Now Playing info for lock screen
        setupNowPlaying()

        // Start call timer
        callTimer = Timer.scheduledTimer(withTimeInterval: 1, repeats: true) { _ in
            if rtcService.callStartTime != nil {
                callDuration += 1
                updateNowPlayingTime()
            }
        }

        Task {
            guard let token = appState.auth.token else {
                rtcService.callPhase = .error("Not authenticated")
                return
            }

            if let incoming {
                // Agent-initiated call — join existing room
                await rtcService.joinCall(
                    incoming: incoming,
                    serverURL: appState.serverURL,
                    token: token
                )
            } else {
                // User-initiated call — create room + join
                let chatContext = appState.buildChatContext(forSession: chatId)
                var systemPrompt: String?
                if !chatContext.isEmpty {
                    systemPrompt = """
                    你是 \(botName)。用用户说的语言回答。简洁、自然地对话。

                    以下是你和用户之前的文字聊天记录，请基于这些上下文继续对话：

                    \(chatContext)
                    """
                }

                await rtcService.startCall(
                    serverURL: appState.serverURL,
                    token: token,
                    botName: botName,
                    chatId: chatId,
                    systemPrompt: systemPrompt
                )
            }
        }
    }

    private func endCall() {
        Task {
            let transcriptText = await rtcService.endCall()

            // Inject transcript into chat for Claude processing
            if let transcriptText {
                appState.injectRtcTranscript(transcriptText)
            }

            cleanup()
            dismiss()
        }
    }

    private func cleanup() {
        callTimer?.invalidate()
        callTimer = nil

        // Re-enable auto-lock
        UIApplication.shared.isIdleTimerDisabled = false

        // Clear Now Playing
        clearNowPlaying()

        // Remove interruption observer
        if let observer = interruptionObserver {
            NotificationCenter.default.removeObserver(observer)
            interruptionObserver = nil
        }
    }

    // MARK: - Audio Interruption Handling

    private func setupInterruptionHandling() {
        interruptionObserver = NotificationCenter.default.addObserver(
            forName: AVAudioSession.interruptionNotification,
            object: nil,
            queue: .main
        ) { notification in
            guard let info = notification.userInfo,
                  let typeValue = info[AVAudioSessionInterruptionTypeKey] as? UInt,
                  let type = AVAudioSession.InterruptionType(rawValue: typeValue) else { return }

            switch type {
            case .began:
                // Phone call or Siri interrupted
                break
            case .ended:
                // Interruption ended — RTC SDK should handle audio session recovery
                break
            @unknown default:
                break
            }
        }
    }

    // MARK: - Now Playing (Lock Screen Controls)

    private func setupNowPlaying() {
        let center = MPNowPlayingInfoCenter.default()
        center.nowPlayingInfo = [
            MPMediaItemPropertyTitle: "MetaBot Voice Call",
            MPMediaItemPropertyArtist: botName,
            MPNowPlayingInfoPropertyElapsedPlaybackTime: 0,
            MPNowPlayingInfoPropertyPlaybackRate: 1.0,
        ]

        // Handle remote command: pause = end call
        let commandCenter = MPRemoteCommandCenter.shared()
        commandCenter.pauseCommand.isEnabled = true
        commandCenter.pauseCommand.addTarget { _ in
            endCall()
            return .success
        }
        commandCenter.playCommand.isEnabled = false
    }

    private func updateNowPlayingTime() {
        MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPNowPlayingInfoPropertyElapsedPlaybackTime] = callDuration
    }

    private func clearNowPlaying() {
        MPNowPlayingInfoCenter.default().nowPlayingInfo = nil
        let commandCenter = MPRemoteCommandCenter.shared()
        commandCenter.pauseCommand.removeTarget(nil)
    }
}
