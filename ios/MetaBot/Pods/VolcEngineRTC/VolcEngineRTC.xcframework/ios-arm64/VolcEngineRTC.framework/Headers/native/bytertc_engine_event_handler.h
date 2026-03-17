/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Event Handler Lite
*/

#pragma once

#include "rtc/bytertc_defines.h"

namespace bytertc {
/**
 * 
 * @type keytype
 * @hidden 
 * @brief 该接口为预留接口，还未实现。
 */

class IVideoSource;
/**
 * 
 * @type keytype
 * @hidden 
 * @brief 该接口为预留接口，还未实现。
 */

class IAudioSource;
/**
 * 
 * @type callback
 * @brief 音视频引擎事件回调接口 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

class IRTCEngineEventHandler {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IRTCEngineEventHandler() {
    }
    /**
     * 
     * @type callback
     * @brief 发生警告回调。 <br>
     *        SDK 运行时出现了警告。SDK 通常会自动恢复，警告信息可以忽略。 <br>
     *        你可能需要干预.
     * @param warn 警告标识码，详见:WarningCode{@link #WarningCode}
     * @list 引擎管理
     */
    
    virtual void onWarning(int warn) {
        (void)warn;
    }
    /**
     * 
     * @type callback
     * @brief 发生错误回调。 <br>
     *        SDK 运行时出现了网络或媒体相关的错误，且无法自动恢复时触发此回调。
     * @param err 错误标识码，参看 ErrorCode{@link #ErrorCode}
     * @list 引擎管理
     */
    
    virtual void onError(int err) {
        (void)err;
    }
    /**
     * 
     * @hidden internal use only
     * @valid since 3.57
     * @type callback
     * @brief 当内部线程发生 block 时，将收到此回调。
     * @param msg block 线程的线程名和 block 检测次数。参看 DeadLockMsg{#DeadLockMsg}。
     * @list Engine Management
     */
    
    virtual void onDeadLockError(const DeadLockMsg& msg) {
        (void)msg;
    }
    /**
     * 
     * @valid since 3.52
     * @type callback
     * @brief 当访问插件失败时，收到此回调。 <br>
     *        RTC SDK 将一些功能封装成插件。当使用这些功能时，如果插件不存在，功能将无法使用。
     * @param extension_name 插件名字
     * @param msg 失败说明
     * @list 高级功能
     */
    
    virtual void onExtensionAccessError(const char* extension_name, const char* msg) {

    }
    /**
     * 
     * @type callback
     * @deprecated since 3.54, use onMediaPlayerPlayingProgress{@link #IMediaPlayerEventHandler#onMediaPlayerPlayingProgress} instead.
     * @brief 混音音频文件播放进度回调
     * @param mix_id 混音 ID
     * @param progress 当前混音音频文件播放进度，单位毫秒
     * @note 调用 setAudioMixingProgressInterval 将时间间隔设为大于 0 的值后，或调用 startAudioMixing 将 AudioMixingConfig 中的时间间隔设为大于 0 的值后，SDK 会按照设置的时间间隔回调该事件。
     * @list 混音
     */
    
    virtual void onAudioMixingPlayingProgress(int mix_id, int64_t progress) {
    }
    /**
     * 
     * @type callback
     * @brief 上报日志时回调该事件。
     * @param log_type <br>
     *        事件类型。目前类型固定为 "live_webrtc_monitor_log"。
     * @param log_content <br>
     *        端监控日志内容。
     * @list 引擎管理
     */
    
    virtual void onLogReport(const char* log_type, const char* log_content) {
        (void)log_type;
        (void)log_content;
    }
    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type callback
     * @brief 音频播放设备变化时回调该事件。
     * @param device 变化后的音频播放设备，参看 AudioRoute{@link #AudioRoute}。
     * @list 音频管理
     */
    
    virtual void onAudioRouteChanged(AudioRoute route) {
        (void)route;
    };
    /**
     * 
     * @type callback
     * @brief SDK 与信令服务器连接状态改变回调。连接状态改变时触发。
     * @param state 当前 SDK 与信令服务器的连接状态，详见 ConnectionState{@link #ConnectionState}。
     * @note 更多信息参见 [连接状态提示](https:
     * @list 网络管理
     */
    
    virtual void onConnectionStateChanged(bytertc::ConnectionState state) {
    }
    /**
     * 
     * @type callback
     * @brief SDK 当前网络连接类型改变回调。当 SDK 的当前网络连接类型发生改变时回调该事件。
     * @param type <br>
     *        SDK 当前的网络连接类型，详见：NetworkType{@link #NetworkType} 。
     * @list 网络管理
     */
    
    virtual void onNetworkTypeChanged(bytertc::NetworkType type) {
    }
    /**
     * 
     * @type callback
     * @brief 音视频流因网络环境变化等原因发生回退，或从回退中恢复时，触发该回调。
     * @param stream_id 流 ID
     * @param stream_info 流信息, 参看 StreamInfo{@link #StreamInfo}。
     * @param event 音视频流发生变化的信息。参看 RemoteStreamSwitch{@link #RemoteStreamSwitch}。
     * @list 网络管理
     */
    
     virtual void onSimulcastSubscribeFallback(const char* stream_id, const StreamInfo& stream_info, const bytertc::RemoteStreamSwitch& event) {
         (void)stream_id;
         (void)stream_info;
         (void)event;
     }
    /**
     * 
     * @type callback
     * @brief 本地未通过 setPublishFallbackOption{@link #IRTCEngine#setPublishFallbackOption} 开启发布性能回退，检测到设备性能不足时，收到此回调。 <br>
     *        本地通过 setPublishFallbackOption{@link #IRTCEngine#setPublishFallbackOption} 开启发布性能回退，因设备性能/网络原因，造成发布性能回退/恢复时，收到此回调。
     * @param stream_id 流 ID
     * @param stream_info 流信息，参看 StreamInfo{@link #StreamInfo}
     * @param mode 指示本地是否开启发布性能回退功能。参看 PerformanceAlarmMode{@link #PerformanceAlarmMode} <br>
     *                  - 当发布端未开启发布性能回退时，mode 值为 kPerformanceAlarmModeNormal。
     *                  - 当发布端开启发布性能回退时，mode 值为 kPerformanceAlarmModeSimulcast。
     * @param room_id 房间 ID <br>
     *                - 未开启发布性能回退时，room_id 为空。
     *                - 开启发布性能回退时，room_id 是告警影响的房间 ID。
     * @param reason 告警原因，参看 PerformanceAlarmReason{@link #PerformanceAlarmReason}
     * @param data 性能回退相关数据，详见 SourceWantedData{@link #SourceWantedData}。
     * @list 网络管理
     */
    
    virtual void onPerformanceAlarms(const char* stream_id, const StreamInfo& stream_info, bytertc::PerformanceAlarmMode mode,
            bytertc::PerformanceAlarmReason reason, const bytertc::SourceWantedData& data) {
        (void)stream_id;
        (void)stream_info;
        (void)mode;
        (void)reason;
        (void)data;
    }
    /**
     * 
     * @type callback
     * @brief 音频设备状态回调。提示音频采集、音频播放等设备设备的状态。
     * @param device_id 设备 ID。
     * @param device_type 设备类型，详见 RTCAudioDeviceType{@link #RTCAudioDeviceType}
     * @param device_state 设备状态，详见 MediaDeviceState{@link #MediaDeviceState}
     * @param device_error 设备错误类型，详见 MediaDeviceError{@link #MediaDeviceError}
     * @list 音频管理
     */
    
    virtual void onAudioDeviceStateChanged(const char* device_id, bytertc::RTCAudioDeviceType device_type,
            bytertc::MediaDeviceState device_state, bytertc::MediaDeviceError device_error) {
        (void)device_id;
        (void)device_type;
        (void)device_state;
        (void)device_error;
    }
    /**
     * 
     * @type callback
     * @brief 视频频设备状态回调。提示摄像头视频采集、屏幕视频采集等设备的状态。
     * @param device_id 设备 ID。采集屏幕共享流时，设备 ID 为固定字符串 `screen_capture_video`
     * @param device_type 设备类型，详见 RTCVideoDeviceType{@link #RTCVideoDeviceType}
     * @param device_state 设备状态，详见 MediaDeviceState{@link #MediaDeviceState}
     * @param device_error 设备错误类型，详见 MediaDeviceError{@link #MediaDeviceError}
     * @list 视频管理
     */
    
    virtual void onVideoDeviceStateChanged(const char* device_id, bytertc::RTCVideoDeviceType device_type,
            bytertc::MediaDeviceState device_state, bytertc::MediaDeviceError device_error) {
        (void)device_id;
        (void)device_type;
        (void)device_state;
        (void)device_error;
    }
    /**
     * 
     * @type callback
     * @brief 音频设备警告回调。音频设备包括音频采集设备和音频渲染设备。
     * @param device_id 设备 ID
     * @param device_type 详见 RTCAudioDeviceType{@link #RTCAudioDeviceType}
     * @param device_warning 详见 MediaDeviceWarning{@link #MediaDeviceWarning}
     * @list 音频管理
     */
     
    virtual void onAudioDeviceWarning(const char* device_id, bytertc::RTCAudioDeviceType device_type,
            bytertc::MediaDeviceWarning device_warning) {
        (void)device_id;
        (void)device_type;
        (void)device_warning;
    }
    /**
     * 
     * @type callback
     * @brief 视频设备警告回调，包括视频采集设备等。
     * @param device_id 设备 ID
     * @param device_type 详见 RTCVideoDeviceType{@link #RTCVideoDeviceType}
     * @param device_warning 详见 MediaDeviceWarning{@link #MediaDeviceWarning}
     * @list 视频管理
     */
     
    virtual void onVideoDeviceWarning(const char* device_id, bytertc::RTCVideoDeviceType device_type,
            bytertc::MediaDeviceWarning device_warning) {
        (void)device_id;
        (void)device_type;
        (void)device_warning;
    }
    /**
     * 
     * @type callback
     * @brief 周期性（2s）发出回调，报告当前 CPU 与内存的相关信息。
     * @param stats 包含当前 CPU 与内存相关信息的结构体。详见 SysStats{@link #SysStats}。
     * @list 引擎管理
     */
    
    virtual void onSysStats(const bytertc::SysStats& stats) {
    }
    /**
     * 
     * @type callback
     * @brief 获取本地录制状态回调。 <br>
     *        该回调由 startFileRecording{@link #IRTCEngine#startFileRecording} 或 stopFileRecording{@link #IRTCEngine#stopFileRecording} 触发。
     * @param video_source 预留参数
     * @param state 录制状态，参看 RecordingState{@link #RecordingState}
     * @param error_code 录制错误码，参看 RecordingErrorCode{@link #RecordingErrorCode}
     * @param info 录制文件的详细信息，参看 RecordingInfo{@link #RecordingInfo}
     * @list 高级功能
     */
    
    virtual void onRecordingStateUpdate(
        IVideoSource* video_source, RecordingState state, RecordingErrorCode error_code, RecordingInfo info) {
    }
    /**
     * 
     * @type callback
     * @brief 本地录制进度回调。 <br>
     *        该回调由 startFileRecording{@link #IRTCEngine#startFileRecording} 触发，录制状态正常时，系统每秒钟都会通过该回调提示录制进度。
     * @param video_source 预留参数。
     * @param process 录制进度，参看 RecordingProgress{@link #RecordingProgress}
     * @param info 录制文件的详细信息，参看 RecordingInfo{@link #RecordingInfo}
     * @list 高级功能
     */
    
    virtual void onRecordingProgressUpdate(IVideoSource* video_source, RecordingProgress process, RecordingInfo info) {
    }
    /**
     * 
     * @type callback
     * @brief 调用 startAudioRecording{@link #IRTCEngine#startAudioRecording} 或 stopAudioRecording{@link #IRTCEngine#stopAudioRecording} 改变音频文件录制状态时，收到此回调。
     * @param state 录制状态，参看 AudioRecordingState{@link #AudioRecordingState}
     * @param error_code 录制错误码，参看 AudioRecordingErrorCode{@link #AudioRecordingErrorCode}
     * @list 高级功能
     */
    
    virtual void onAudioRecordingStateUpdate(AudioRecordingState state, AudioRecordingErrorCode error_code) {
    }
    /**
     * 
     * @type callback
     * @brief 登录结果回调
     * @param uid <br>
     *        登录用户 ID
     * @param error_code <br>
     *        登录结果 <br>
     *        详见 LoginErrorCode{@link #LoginErrorCode}。
     * @param elapsed <br>
     *        从调用 login{@link #IRTCEngine#login} 接口开始到返回结果所用时长 <br>
     *        单位为 ms。
     * @note 调用 login{@link #IRTCEngine#login} 后，会收到此回调。
     * @list 消息
     */
    
    virtual void onLoginResult(const char* uid, int error_code, int elapsed) {
        (void)uid;
        (void)error_code;
        (void)elapsed;
    }
    /**
     * 
     * @type callback
     * @brief 登出结果回调
     * @param reason 用户登出的原因，参看 LogoutReason{@link #LogoutReason}
     * @note 在以下两种情况下会收到此回调：调用 logout{@link #IRTCEngine#logout} 接口主动退出；或其他用户以相同 UserId 进行 `login` 导致本地用户被动登出。
     * @list 消息
     */
    
    virtual void onLogout(LogoutReason reason) {
    }
    /**
     * 
     * @type callback
     * @brief 设置应用服务器参数的返回结果
     * @param error <br>
     *        设置结果 <br>
     *        - 返回 200，设置成功
     *        - 返回其他，设置失败，详见 UserMessageSendResult{@link #UserMessageSendResult}
     * @note 调用 setServerParams{@link #IRTCEngine#setServerParams} 后，会收到此回调。
     * @list 消息
     */
    
    virtual void onServerParamsSetResult(int error) {
        (void)error;
    }
    /**
     * 
     * @type callback
     * @brief 查询对端或本端用户登录状态的返回结果
     * @param peer_user_id <br>
     *        需要查询的用户 ID
     * @param status <br>
     *        查询的用户登录状态 <br>
     *        详见 UserOnlineStatus{@link #UserOnlineStatus}.
     * @note 必须先调用 getPeerOnlineStatus{@link #IRTCEngine#getPeerOnlineStatus}，才能收到此回调。
     * @list 消息
     */
    
    virtual void onGetPeerOnlineStatus(const char* peer_user_id, int status) {
        (void)peer_user_id;
        (void)status;
    }
    /**
     * 
     * @type callback
     * @brief > 该接口将于 3.64 版本废弃，请使用 onUserMessageReceivedOutsideRoom{@link #IRTCEngineEventHandler#onUserMessageReceivedOutsideRoom-2} 代替。
     * @brief 收到房间外用户调用 sendUserMessageOutsideRoom{@link #IRTCEngine#sendUserMessageOutsideRoom} 发来的文本消息时，会收到此回调。
     * @param uid 消息发送者 ID。
     * @param message 收到的文本消息内容。
     * @list 消息
     */
    
    virtual void onUserMessageReceivedOutsideRoom(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @brief > 该接口将于 3.64 onUserBinaryMessageReceivedOutsideRoom{@link #IRTCEngineEventHandler#onUserBinaryMessageReceivedOutsideRoom-2} 代替。
     * @brief 收到房间外用户调用 sendUserBinaryMessageOutsideRoom{@link #IRTCEngine#sendUserBinaryMessageOutsideRoom} 发来的二进制消息时，会收到此回调
     * @param uid 消息发送者 ID。
     * @param size 收到的二进制消息长度。
     * @param message 收到的二进制消息内容。
     * @list 消息
     */
    
    virtual void onUserBinaryMessageReceivedOutsideRoom(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @brief 给房间外指定的用户发送消息的结果回调。<br>
     *        当调用 sendUserMessageOutsideRoom{@link #IRTCEngine#sendUserMessageOutsideRoom} 或 sendUserBinaryMessageOutsideRoom{@link #IRTCEngine#sendUserBinaryMessageOutsideRoom} 发送消息后，会收到此回调。
     * @param msgid 本条消息的 ID。 <br>
     *        所有的 P2P 和 P2Server 消息共用一个 ID 序列。
     * @param error 消息发送结果。详见 UserMessageSendResult{@link #UserMessageSendResult}。
     * @list 消息
     */
    
    virtual void onUserMessageSendResultOutsideRoom(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }
    /**
     * 
     * @type callback
     * @brief 收到房间外用户调用 sendUserMessageOutsideRoom{@link #IRTCEngine#sendUserMessageOutsideRoom} 发来的文本消息时，会收到此回调。
     * @param msgid 收到的文本消息编号。
     * @param uid 消息发送者 ID。
     * @param message 收到的文本消息的内容。
     * @list 消息
     */
    
    virtual void onUserMessageReceivedOutsideRoom(int64_t msgid, const char* uid, const char* message) {
        (void)msgid;
        (void)uid;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @brief 当收到来自房间外用户通过 sendUserBinaryMessageOutsideRoom{@link #IRTCEngine#sendUserBinaryMessageOutsideRoom} 发送的二进制消息时，触发此回调。
     * @param msgid 本条消息的 ID。
     * @param uid 消息发送者的用户 ID。
     * @param message 收到的二进制消息内容。
     * @param size 二进制消息的长度，单位为字节。
     * @list 消息
     */
    
    virtual void onUserBinaryMessageReceivedOutsideRoom(int64_t msgid, const char* uid, const uint8_t* message, int size) {
        (void)msgid;
        (void)uid;
        (void)message;
        (void)size;
    }
    /**
     * 
     * @type callback
     * @brief 给应用服务器发送消息的回调。
     * @param msgid 本条消息的 ID。 <br>
     *        所有的 P2P 和 P2Server 消息共用一个 ID 序列。
     * @param error 消息发送结果，详见 UserMessageSendResult{@link #UserMessageSendResult}。
     * @param msg 应用服务器收到 HTTP 请求后，在 ACK 中返回的信息。消息不超过 64 KB。
     * @note 本回调为异步回调。当调用 sendServerMessage{@link #IRTCEngine#sendServerMessage} 或 sendServerBinaryMessage{@link #IRTCEngine#sendServerBinaryMessage} 接口发送消息后，会收到此回调。
     * @list 消息
     */
    
    virtual void onServerMessageSendResult(int64_t msgid, int error, const bytertc::ServerACKMsg& msg) {
        (void)msgid;
        (void)error;
        (void)msg;
    }
    /**
     * 
     * @type callback
     * @brief 收到通过调用 sendSEIMessage{@link #IRTCEngine#sendSEIMessage} 发送带有 SEI 消息的视频帧时，收到此回调。
     * @param stream_id 收到 SEI 消息的视频流 ID
     * @param stream_info 包含 SEI 发送者的用户名，所在的房间名和媒体流, 参看 StreamInfo{@link #StreamInfo}
     * @param message 收到的 SEI 消息内容
     * @param length 收到的 SEI 消息长度
     * @list 网络管理
     */
    
    virtual void onSEIMessageReceived(const char* stream_id, const StreamInfo& stream_info, const uint8_t* message, int length){
        (void)stream_id;
        (void)stream_info;
        (void)message;
        (void)length;
    }
    /**
     * 
     * @type callback
     * @brief 黑帧视频流发布状态回调。 <br>
     *        在语音通话场景下，本地用户调用 sendSEIMessage{@link #IRTCEngine#sendSEIMessage} 通过黑帧视频流发送 SEI 数据时，流的发送状态会通过该回调通知远端用户。 <br>
     *        你可以通过此回调判断携带 SEI 数据的视频帧为黑帧，从而不对该视频帧进行渲染。
     * @param stream_id 收到 SEI 消息的视频流 ID
     * @param stream_info 包含 SEI 发送者的用户名，所在的房间名和媒体流, 参看 StreamInfo{@link #StreamInfo}
     * @param type 黑帧视频流状态，参看 SEIStreamEventType{@link #SEIStreamEventType}
     * @list 消息
     */
    
    virtual void onSEIStreamUpdate(const char* stream_id, const StreamInfo& stream_info, SEIStreamEventType type) {
        (void)stream_id;
        (void)stream_info;
        (void)type;
    }

    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @region 音频管理
     * @brief 音乐场景检测结果回调。 <br>
     *        调用 enableAudioAEDReport{@link #IRTCEngine#enableAudioAEDReport} 后，根据设置的interval值，本地会周期性地收到此回调 <br>
     * @param state 音乐场景检测结果，参看 AudioAEDType{@link #AudioAEDType}。
     * @list 音频管理
     */
    
    virtual void onAudioAEDStateUpdate(const AudioAEDType state) {
        (void)state;
    }

     /**
      * 
      * @type callback
      * @brief 远端用户进房后，本地调用 enableAudioPropertiesReport{@link #IRTCEngine#enableAudioPropertiesReport}，根据设置的 interval 值，本地会周期性地收到此回调，了解订阅的远端用户的瞬时音频信息。 <br>
      *        远端用户的音频包括使用 RTC SDK 内部机制/自定义机制采集的麦克风音频和屏幕音频。
      * @param audio_properties_infos 远端音频信息，其中包含音频流属性、房间 ID、用户 ID ，详见 RemoteAudioPropertiesInfo{@link #RemoteAudioPropertiesInfo}。
      * @param audio_properties_info_number 数组长度
      * @param total_remote_volume 所有订阅的远端流混音后的音量，范围是 [0,255]。 <br>
      *       - [0,25] 接近无声；
      *       - [25,75] 为低音量；
      *       - [76,204] 为中音量；
      *       - [205,255] 为高音量。
      * @list 音频管理
      */
     
    virtual void onRemoteAudioPropertiesReport(const RemoteAudioPropertiesInfo* audio_properties_infos, int audio_properties_info_number, int total_remote_volume) {
        (void)audio_properties_infos;
        (void)audio_properties_info_number;
        (void)total_remote_volume;
    }
     /**
      * 
      * @type callback
      * @brief 调用 enableAudioPropertiesReport{@link #IRTCEngine#enableAudioPropertiesReport} 后，根据设置的 interval 值，你会周期性地收到此回调，了解本地音频的瞬时相关信息。 <br>
      *        本地音频包括使用 RTC SDK 内部机制采集的麦克风音频，屏幕音频和本地混音音频信息。
      * @param audio_properties_infos 本地音频信息，详见 LocalAudioPropertiesInfo{@link #LocalAudioPropertiesInfo}。本地音量可通过 setAudioCaptureDeviceVolume{@link #IAudioDeviceManager#setAudioCaptureDeviceVolume} 设置。
      * @param audio_properties_info_number 数组长度
      * @list 音频管理
      */
     
    virtual void onLocalAudioPropertiesReport(const LocalAudioPropertiesInfo * audio_properties_infos, int audio_properties_info_number) {
        (void)audio_properties_infos;
        (void)audio_properties_info_number;
    }
    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @brief 音量调节前的音量大小
     * @list Audio Management
     */
    
    virtual void onRemoteAudioPropertiesReportEx(const RemoteAudioPropertiesInfo* audio_properties_infos, int audio_properties_info_number) {
        (void)audio_properties_infos;
        (void)audio_properties_info_number;
    }
    
    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @region 音频管理
     * @brief 人声检测结果回调。 <br>
     *        调用 enableAudioVADReport{@link #IRTCEngine#enableAudioVADReport} 后，根据设置的interval值，本地会周期性地收到此回调 <br>
     * @param state 人声检测结果，参看 AudioVADType{@link #AudioVADType}。
     * @list 音频管理
     */
    
    virtual void onAudioVADStateUpdate(AudioVADType state) {
        (void)state;
    }
    /**
     * 
     * @type callback
     * @brief 回调音频设备测试时的播放音量
     * @param volume 音频设备测试播放音量。取值范围：[0,255]
     * @note 调用 startAudioPlaybackDeviceTest{@link #IAudioDeviceManager#startAudioPlaybackDeviceTest} 或 startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest}，开始播放音频文件或录音时，将开启该回调。本回调为周期性回调，回调周期由上述接口的 `interval` 参数指定。
     * @list 音频管理
     */
    
    virtual void onAudioPlaybackDeviceTestVolume(int volume) {
        (void)volume;
    }
    /**
     * 
     * @hidden(Android,iOS)
     * @type callback
     * @brief 音频设备音量改变回调。当通过系统设置，改变音频设备音量或静音状态时，触发本回调。本回调无需手动开启。
     * @param device_type 设备类型，包括麦克风和扬声器，参阅 RTCAudioDeviceType{@link #RTCAudioDeviceType}。
     * @param volume 音量值，[0, 255]。当 volume 变为 0 时，muted 会变为 True。注意：在 Windows 端，当麦克风 volume 变为 0 时，muted 值不变。
     * @param muted 是否禁音状态。扬声器被设置为禁音时，muted 为 True，但 volume 保持不变。
     * @list 音频管理
     */
    
    virtual void onAudioDeviceVolumeChanged(bytertc::RTCAudioDeviceType device_type, int volume, bool muted) {
        (void)device_type;
        (void)volume;
        (void)muted;
    }
    /**
     * 
     * @type callback
     * @brief 本地音频流的状态发生改变时，收到此回调。
     * @param audio_source 本地音频流对象，详见：IAudioSource{@link #IAudioSource}
     * @param state 本地音频流的状态，详见： LocalAudioStreamState{@link #LocalAudioStreamState}
     * @param error 本地音频状态改变时的错误码，详见：LocalAudioStreamError{@link #LocalAudioStreamError}
     * @list 音频管理
     */
    
    virtual void onLocalAudioStateChanged(IAudioSource* audio_source, LocalAudioStreamState state, LocalAudioStreamError error) {
        (void)audio_source;
        (void)state;
        (void)error;
    }
    /**
     * 
     * @type callback
     * @brief 房间内的用户调用 startAudioCapture{@link #IRTCEngine#startAudioCapture} 开启音频采集时，房间内其他用户会收到此回调。
     * @param stream_id 开启音频采集的远端用户发布的流 ID
     * @param stream_info 开启音频采集的远端流信息，详见 StreamInfo{@link #StreamInfo}
     * @list 音频管理
     */
    
    virtual void onUserStartAudioCapture(const char* stream_id, const StreamInfo& stream_info) {
        (void)stream_id;
        (void)stream_info;
    }
    /**
     * 
     * @type callback
     * @brief 房间内的用户调用 stopAudioCapture{@link #IRTCEngine#stopAudioCapture} 关闭音频采集时，房间内其他用户会收到此回调。
     * @param stream_id 关闭音频采集的远端用户发布的流 ID
     * @param stream_info 关闭音频采集的远端流信息，详见 StreamInfo{@link #StreamInfo}
     * @list 音频管理
     */
    
    virtual void onUserStopAudioCapture(const char* stream_id, const StreamInfo& stream_info) {
        (void)stream_id;
        (void)stream_info;
    }
    /**
     * 
     * @type callback
     * @brief 调用 enableAudioPropertiesReport{@link #IRTCEngine#enableAudioPropertiesReport} 后，根据设置的 `config.interval`，你会周期性地收到此回调，获取房间内的最活跃用户信息。
     * @param room_id 房间 ID
     * @param uid 最活跃用户（ActiveSpeaker）的用户 ID
     * @list 音频管理
     */
    
    virtual void onActiveSpeaker(const char* room_id, const char* uid) {
        (void)room_id;
        (void)uid;
    }
    /**
     * 
     * @type callback
     * @brief 音频流同步信息回调。可以通过此回调，在远端用户调用 sendStreamSyncInfo{@link #IRTCEngine#sendStreamSyncInfo} 发送音频流同步消息后，收到远端发送的音频流同步信息。
     * @param stream_id 远端流 ID
     * @param stream_info 远端流信息，详见 StreamInfo{@link #StreamInfo}。
     * @param stream_type 媒体流类型，详见 SyncInfoStreamType{@link #SyncInfoStreamType} 。
     * @param data 消息内容。
     * @param length 消息长度。
     * @list 音频管理
     */
    
    virtual void onStreamSyncInfoReceived(const char* stream_id, const StreamInfo& stream_info, SyncInfoStreamType stream_type,
                                         const uint8_t* data, int32_t length) {
        (void)stream_id;
        (void)stream_info;        
        (void)stream_type;
        (void)data;
        (void)length;
    }
    /**
     * 
     * @type callback
     * @brief 通话前网络探测结果。 <br>
     *        成功调用 startNetworkDetection{@link #IRTCEngine#startNetworkDetection} 接口开始探测后，会在 3s 内首次收到该回调，之后每 2s 收到一次该回调。
     * @param type 探测网络类型为上行/下行
     * @param quality 探测网络的质量，参看 NetworkQuality{@link #NetworkQuality}。
     * @param rtt 探测网络的 RTT，单位：ms
     * @param lost_rate 探测网络的丢包率
     * @param bitrate 探测网络的带宽，单位：kbps
     * @param jitter 探测网络的抖动,单位：ms
     * @list 网络管理
     */
    
    virtual void onNetworkDetectionResult(NetworkDetectionLinkType type, NetworkQuality quality, int rtt, double lost_rate,
                                      int bitrate, int jitter){
        (void)type;
        (void)quality;
        (void)rtt;
        (void)lost_rate;
        (void)bitrate;
        (void)jitter;
    }
    /**
     * 
     * @type callback
     * @brief 通话前网络探测结束 <br>
     *        以下情况将停止探测并收到本一次本回调： <br>
     *        1. 当调用 stopNetworkDetection{@link #IRTCEngine#stopNetworkDetection} 接口停止探测后，会收到一次该回调； <br>
     *        2. 当收到远端/本端音频首帧后，停止探测； <br>
     *        3. 当探测超过 3 分钟后，停止探测； <br>
     *        4. 当探测链路断开一定时间之后，停止探测。
     * @param reason <br>
     *        停止探测的原因类型,参考 NetworkDetectionStopReason{@link #NetworkDetectionStopReason}
     * @list 网络管理
     */
    
    virtual void onNetworkDetectionStopped(NetworkDetectionStopReason reason){
        (void)reason;
    }
    /**
     * 
     * @type callback
     * @brief 房间内的可见用户调用 startVideoCapture{@link #IRTCEngine#startVideoCapture} 开启内部视频采集时，房间内其他用户会收到此回调。
     * @param stream_id 视频流 ID
     * @param stream_info 视频流信息，详见 StreamInfo{@link #StreamInfo}。
     * @list 视频管理
     */
    
    virtual void onUserStartVideoCapture(const char* stream_id, const StreamInfo& stream_info) {
        (void)stream_id;
        (void)stream_info;
    }
    /**
     * 
     * @type callback
     * @brief 房间内的可见用户调用 stopVideoCapture{@link #IRTCEngine#stopVideoCapture} 关闭内部视频采集时，房间内其他用户会收到此回调。 <br>
     *        若发布视频数据前未开启采集，房间内所有可见用户会收到此回调。
     * @param stream_id 视频流 ID
     * @param stream_info 视频流信息，详见 StreamInfo{@link #StreamInfo}。
     * @list 视频管理
     */
    
    virtual void onUserStopVideoCapture(const char* stream_id, const StreamInfo& stream_info) {
        (void)stream_id;
        (void)stream_info;
    }
    /**
     * 
     * @type callback
     * @brief 本地视频大小或旋转信息发生改变时，收到此回调。
     * @param video_source 预留参数。
     * @param info 视频帧信息。参看 VideoFrameInfo{@link #VideoFrameInfo}。
     * @list 音视频传输
     */
    
    virtual void onLocalVideoSizeChanged(IVideoSource* video_source, const VideoFrameInfo& info) {
        (void)video_source;
        (void)info;
    }
    /**
     * 
     * @type callback
     * @brief 远端视频大小或旋转信息发生改变时，房间内订阅此视频流的用户会收到此回调。
     * @param stream_id 远端视频流 ID。
     * @param stream_info 远端视频流信息。参看 StreamInfo{@link #StreamInfo}。
     * @param info 视频帧信息。参看 VideoFrameInfo{@link #VideoFrameInfo}。
     * @list 音视频传输
     */
    
    virtual void onRemoteVideoSizeChanged(const char* stream_id, const StreamInfo& stream_info, const VideoFrameInfo& info) {
        (void)stream_id;
        (void)stream_info;
        (void)info;
    }
    /**
     * 
     * @type callback
     * @brief RTC SDK 在本地完成第一帧视频帧或屏幕视频帧采集时，收到此回调。
     * @param video_source 视频源对象。参看 IVideoSource{@link #IVideoSource}。
     * @param info 视频信息。参看 VideoFrameInfo{@link #VideoFrameInfo}。
     * @note 对于采集到的本地视频帧，你可以调用 setLocalVideoCanvas{@link #IRTCEngine#setLocalVideoCanvas} 或 setLocalVideoSink{@link #IRTCEngine#setLocalVideoSink} 在本地渲染。
     * @list 音视频传输
     */
    
    virtual void onFirstLocalVideoFrameCaptured(IVideoSource* video_source, const VideoFrameInfo& info) {
        (void)video_source;
        (void)info;
    }
    /**
     * 
     * @type callback
     * @brief 订阅端接收并解码远端音频流首帧时，收到此回调。包含以下情况： <br>
     *        1. 发布端发布音频，包含首次发布和取消后再次发布。取消发布还包括发布端主动退房后或掉线。<br>
     *        2. 发布端关闭音频采集后，再次打开采集。使用外部源时，停止推流后再次推流。<br>
     *        3. 发布端发布音频后，订阅端取消订阅音频后，又再次订阅音频。
     * @param stream_id 远端音频流 ID。
     * @param stream_info 远端音频流信息，参看 StreamInfo{@link #StreamInfo}。
     * @note
     *        - 用户刚收到房间内每一路音频流时，都会收到该回调。
     *        - 摄像头流、屏幕流，内部采集、外部源、自动订阅和手动订阅的音频流，都符合上述策略。
     * @list 音视频传输
     */
    
    virtual void onFirstRemoteAudioFrame(const char* stream_id, const StreamInfo& stream_info) {
        (void)stream_id;
        (void)stream_info;
    }
    /**
     * 
     * @type callback
     * @brief 用户订阅来自远端的音频流状态发生改变时，会收到此回调，了解当前的远端音频流状态。
     * @param stream_id 远端音频流 ID。
     * @param stream_info 远端音频流信息，参看 StreamInfo{@link #StreamInfo}。
     * @param state 远端音频流状态，参看 RemoteAudioState{@link #RemoteAudioState}
     * @param reason 远端音频流状态改变的原因，参看 RemoteAudioStateChangeReason{@link #RemoteAudioStateChangeReason}
     * @list 音视频传输
     */
    
    virtual void onRemoteAudioStateChanged(
            const char* stream_id, const StreamInfo& stream_info, RemoteAudioState state, RemoteAudioStateChangeReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)state;
        (void)reason;
    }
    /**
     * 
     * @type callback
     * @brief 本地视频流的状态发生改变时，收到该事件。
     * @param video_source 本地视频流对象。参看 IVideoSource{@link #IVideoSource}。
     * @param state 本地视频流状态，参看 LocalVideoStreamState{@link #LocalVideoStreamState}
     * @param error 本地视频状态改变时的错误码，参看 LocalVideoStreamError{@link #LocalVideoStreamError}
     * @list 视频管理
     */
    
    virtual void onLocalVideoStateChanged(IVideoSource* video_source, LocalVideoStreamState state, LocalVideoStreamError error) {
        (void)video_source;
        (void)state;
        (void)error;
    }
    /**
     * 
     * @type callback
     * @brief 远端视频流的状态发生改变时，房间内订阅此流的用户会收到该事件。
     * @param stream_id 远端视频流 ID。
     * @param stream_info 远端视频流信息，包括房间 ID、用户 ID、流属性，等。参看 StreamInfo{@link #StreamInfo}。
     * @param state 远端视频流状态，参看 RemoteVideoState{@link #RemoteVideoState}。
     * @param reason 远端视频流状态改变的原因，参看 RemoteVideoStateChangeReason{@link #RemoteVideoStateChangeReason}。
     * @note 本回调仅适用于主流，不适用于屏幕流。
     * @list 音视频传输
     */
    
    virtual void onRemoteVideoStateChanged(
            const char* stream_id, const StreamInfo& stream_info, RemoteVideoState state, RemoteVideoStateChangeReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)state;
        (void)reason;
    }
    /**
     * 
     * @hidden for internal use only
     * @valid since 3.54
     * @type callback
     * @brief 远端视频流的超分状态发生改变时，房间内订阅此流的用户会收到该回调。
     * @param stream_id 远端视频流 ID。
     * @param stream_info 远端视频流信息，包括房间 ID、用户 ID、流属性，等。参看 StreamInfo{@link #StreamInfo}。
     * @param mode 超分模式，参看 VideoSuperResolutionMode{@link #VideoSuperResolutionMode}。
     * @param reason 超分模式改变原因，参看 VideoSuperResolutionModeChangedReason{@link #VideoSuperResolutionModeChangedReason}。
     * @list 视频处理
     */
    
    virtual void onRemoteVideoSuperResolutionModeChanged(
            const char* stream_id, const StreamInfo& stream_info, VideoSuperResolutionMode mode, VideoSuperResolutionModeChangedReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)mode;
        (void)reason;
    }
    /**
     * 
     * @hidden for internal use only
     * @valid since 3.54
     * @type callback
     * @brief 降噪模式状态变更回调。当降噪模式的运行状态发生改变，SDK 会触发该回调，提示用户降噪模式改变后的运行状态及状态发生改变的原因。
     * @param mode 视频降噪模式，参看 VideoDenoiseMode{@link #VideoDenoiseMode}。
     * @param reason 视频降噪模式改变的原因，参看 VideoDenoiseModeChangedReason{@link #VideoDenoiseModeChangedReason}。
     * @list 视频处理
     */
    
    virtual void onVideoDenoiseModeChanged(VideoDenoiseMode mode, VideoDenoiseModeChangedReason reason) {
       (void)mode;
       (void)reason;
    }
    /**
     * 
     * @type callback
     * @brief 本地音频首帧发送状态发生改变时，收到此回调。
     * @param stream_id 音频发布用户所在的房间 ID
     * @param stream_info 音频流信息，详见 StreamInfo{@link #StreamInfo}
     * @param user 本地用户信息，详见 RtcUser{@link #RtcUser}
     * @param state 首帧发送状态，详见 FirstFrameSendState{@link #FirstFrameSendState}
     * @list 音视频传输
     */
    
    virtual void onAudioFrameSendStateChanged(const char* stream_id, const StreamInfo& stream_info, const RtcUser& user, FirstFrameSendState state) {
        (void)stream_id;
        (void)stream_info;
        (void)user;
        (void)state;
    }
    /**
     * 
     * @type callback
     * @brief 本地视频首帧发送状态发生改变时，收到此回调。
     * @param stream_id 流 ID。
     * @param stream_info 视频流信息，详见 StreamInfo{@link #StreamInfo}
     * @param user 本地用户信息，详见 RtcUser{@link #RtcUser}
     * @param state 首帧发送状态，详见 FirstFrameSendState{@link #FirstFrameSendState}
     * @list 音视频传输
     */
    
    virtual void onVideoFrameSendStateChanged(const char* stream_id, const StreamInfo& stream_info, const RtcUser& user, FirstFrameSendState state) {
        (void)stream_id;
        (void)stream_info;
        (void)user;
        (void)state;
    }
    /**
     * 
     * @type callback
     * @brief SDK 内部渲染成功远端视频流首帧后，收到此回调。包含以下情况： <br>
     *        1. 发布端首次发布视频 <br>
     *        2. 在 1 条件下，发布端取消发布视频后，再次发布视频 <br>
     *        3. 在 1 条件下，发布端关闭视频采集后，再次打开采集（或使用外部源时，停止推流后再次推流） <br>
     *        4. 在 1 条件下，订阅端取消订阅视频后，再次订阅视频（调用接口包括 subscribeAllStreamsVideo{@link #IRTCRoom#subscribeallstreamsVideo}，pauseAllSubscribedStreamVideo{@link #IRTCRoom#pauseAllSubscribedStreamVideo}/resumeAllSubscribedStreamVideo{@link #IRTCRoom#resumeAllSubscribedStreamVideo}）
     * @param stream_id 远端视频流 ID。
     * @param stream_info 远端视频流信息。参看 StreamInfo{@link #StreamInfo}。
     * @param info 视频帧信息。参看 VideoFrameInfo{@link #VideoFrameInfo}。
     * @note 仅在采用内部渲染时，符合上述策略。
     * @list 音视频传输
     */
    
    virtual void onFirstRemoteVideoFrameRendered(const char* stream_id, const StreamInfo& stream_info, const VideoFrameInfo& info) {
        (void)stream_id;
        (void)stream_info;
        (void)info;
    }
    /**
     * 
     * @type callback
     * @brief 订阅端接收并解码远端视频流首帧时，收到此回调。包含以下情况： <br>
     *        1. 发布端发布视频，包含首次发布和取消后再次发布。取消发布还包括发布端主动退房后或掉线。<br>
     *        2. 发布端关闭视频采集后，再次打开采集。使用外部源时，停止推流后再次推流。<br>
     *        3. 发布端发布视频后，订阅端取消订阅视频后，又再次订阅视频。
     * @param stream_id 远端视频流 ID。
     * @param stream_info 远端视频流信息。参看 StreamInfo{@link #StreamInfo}。
     * @param info 视频帧信息。参看 VideoFrameInfo{@link #VideoFrameInfo}。
     * @note
     *       - 用户刚收到房间内订阅的每一路视频流时，都会收到该回调。
     *       - 摄像头流、屏幕流，内部采集、外部源，自动订阅和手动订阅的视频流，都符合上述策略。
     * @list 音视频传输
     */
    
    virtual void onFirstRemoteVideoFrameDecoded(const char* stream_id, const StreamInfo& stream_info, const VideoFrameInfo& info) {
        (void)stream_id;
        (void)stream_info;
        (void)info;
    }
    /**
     * 
     * @type callback
     * @brief 音频首帧播放状态发生改变时，收到此回调。
     * @param stream_id 音频首帧播放状态发生改变的流 ID
     * @param stream_info 音频首帧播放状态发生改变的流信息，详见 StreamInfo{@link #StreamInfo}
     * @param user 远端用户信息，详见 RtcUser{@link #RtcUser}
     * @param state 首帧播放状态，详见 FirstFramePlayState{@link #FirstFramePlayState}
     * @list 音视频传输
     */
    
    virtual void onAudioFramePlayStateChanged(const char* stream_id, const StreamInfo& stream_info, const RtcUser& user, FirstFramePlayState state) {
        (void)stream_id;
        (void)stream_info;
        (void)user;
        (void)state;
    }
    /**
     * 
     * @type callback
     * @brief 远端视频流的首帧播放状态改变时，收到此回调。
     * @param stream_id 视频首帧播放状态发生改变的流 ID
     * @param stream_info 视频首帧播放状态发生改变的流信息，详见 StreamInfo{@link #StreamInfo}
     * @param user 远端用户信息，详见 RtcUser{@link #RtcUser}
     * @param state 首帧播放状态，详见 FirstFramePlayState{@link #FirstFramePlayState}
     * @list 音视频传输
     */
    
    virtual void onVideoFramePlayStateChanged(const char* stream_id, const StreamInfo& stream_info, const RtcUser& user, FirstFramePlayState state) {
        (void)stream_id;
        (void)stream_info;
        (void)user;
        (void)state;
    }
    /**
     * 
     * @type callback
     * @brief 发布音频流时，采集到第一帧音频帧，收到该回调。
     * @param audio_source 音频源，详见 IAudioSource{@link #IAudioSource}。
     * @note 如果发布音频流时，未开启本地音频采集，SDK 会推送静音帧，也会收到此回调。
     * @list 音视频传输
     */
    
    virtual void onFirstLocalAudioFrame(IAudioSource* audio_source) {
        (void)audio_source;
    }
    /**
     * 
     * @deprecated since 3.60, use onMixedStreamEvent{@link #IRTCEngineEventHandler#onMixedStreamEvent} instead.
     * @type callback
     * @hidden(Linux)
     * @brief WTN 流发布结果回调。 <br>
     *        调用 startPushMixedStream 接口发布 WTN 流后，启动结果通过此回调方法通知用户。
     * @param room_id WTN 流的发布房间的 ID
     * @param public_streamid WTN 流 ID
     * @param error_code WTN 流发布结果状态码。详见 PublicStreamErrorCode{@link #PublicStreamErrorCode}
     * @list 音视频传输
     * @order 24
     */
    
    virtual void onPushPublicStreamResult(const char* room_id, const char* public_streamid, PublicStreamErrorCode error_code) {
        (void)room_id;
        (void)public_streamid;
        (void)error_code;
    }

    /**
     * 
     * @type callback
     * @brief 调用 startCloudProxy{@link #IRTCEngine#startCloudProxy} 开启云代理，SDK 首次成功连接云代理服务器时，回调此事件。
     * @param interval 从开启云代理到连接成功经过的时间，单位为 ms
     * @list 通话加密
     */
    
    virtual void onCloudProxyConnected(int interval) {
        (void)interval;
    }
    /**
     * 
     * @hidden(Linux)
     * @type callback
     * @brief 关于音视频回路测试结果的回调。
     * @param result 测试结果，参看 EchoTestResult{@link #EchoTestResult}
     * @note 该回调触发的时机包括： <br>
     *        - 检测过程中采集设备发生错误时；
     *        - 检测成功后；
     *        - 非设备原因导致检测过程中未接收到音/视频回放，停止检测后。
     * @list 网络管理
     */
    
    virtual void onEchoTestResult(EchoTestResult result) {
        (void)result;
    };
    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @brief 音频 dump 状态改变回调
     * @param status 音频 dump 状态，参见 AudioDumpStatus{@link #AudioDumpStatus}
     * @note 本回调用于内部排查音质相关异常问题，开发者无需关注。
     * @list Network Processing
     */
    
    virtual void onAudioDumpStateChanged(AudioDumpStatus status) {
        (void)status;
    }
    /**
     * 
     * @hidden(Linux)
     * @type callback
     * @brief 首次调用 getNetworkTimeInfo{@link #IRTCEngine#getNetworkTimeInfo} 后，SDK 内部启动网络时间同步，同步完成时会触发此回调。
     * @list 网络管理
     */
    
    virtual void onNetworkTimeSynchronized() {
    }
    /**
     * 
     * @hidden internal use only
     * @type callback
     * @brief license 过期提醒。在剩余天数低于 30 天时，收到此回调。
     * @param days license 剩余有效天数
     * @list Network Processing
     */
    
    virtual void onLicenseWillExpire(int days) {
        (void)days;
    }
    /**
     * 
     * @type callback
     * @brief AIGC使用license计费的场景下，当license配额用尽时，会收到该回调，请及时续费。 <br>
     *        收到该回调时，AIGC服务会中止，请及时续费，并且建议离开房间、销毁SDK。
     * @param message 回调描述信息
     * @list 引擎管理
     */
    
    virtual void onLicenseQuotaExceeded(const char* message) {
        (void)message;
    }
    /**
     * 
     * @hidden
     * @type callback
     * @brief 外部采集时，调用 setOriginalScreenVideoInfo设置屏幕或窗口大小改变前的分辨率后，若屏幕采集模式为智能模式，你将收到此回调，根据 RTC 智能决策合适的帧率和分辨率积（宽*高）重新采集。
     * @param frame_update_info RTC 智能决策后合适的帧率和分辨率积（宽*高）。参看 FrameUpdateInfo{@link #FrameUpdateInfo}。
     * @list 屏幕共享
     */
    
    virtual void onExternalScreenFrameUpdate(FrameUpdateInfo frame_update_info) {
        (void)frame_update_info;
    }
    /**
     * 
     * @hidden(Linux)
     * @type callback
     * @brief 通话前回声检测结果回调。
     * @param hardware_echo_detection_result 参见 HardwareEchoDetectionResult{@link #HardwareEchoDetectionResult}
     * @note
     *        - 通话前调用 startHardwareEchoDetection{@link #IRTCEngine#startHardwareEchoDetection} 后，将触发本回调返回检测结果。
     *        - 建议在收到检测结果后，调用 stopHardwareEchoDetection{@link #IRTCEngine#stopHardwareEchoDetection} 停止检测，释放对音频设备的占用。
     *        - 如果 SDK 在通话中检测到回声，将通过 onAudioDeviceWarning{@link #IRTCEngineEventHandler#onAudioDeviceWarning} 回调 `kMediaDeviceWarningDetectLeakEcho`。
     * @list 音频管理
     */
    
    virtual void onHardwareEchoDetectionResult(HardwareEchoDetectionResult hardware_echo_detection_result) {
        (void)hardware_echo_detection_result;
    }
    /**
     * 
     * @type callback
     * @brief 本地代理状态发生改变回调。调用 setLocalProxy{@link #IRTCEngine#setLocalProxy} 设置本地代理后，SDK 会触发此回调，返回代理连接的状态。
     * @param local_proxy_type 本地代理类型。参看 LocalProxyType{@link #LocalProxyType}。
     * @param local_proxy_state 本地代理状态。参看 LocalProxyState{@link #LocalProxyState}。
     * @param local_proxy_error 本地代理错误。参看 LocalProxyError{@link #LocalProxyError}。
     * @list 通话加密
     */
    
    virtual void onLocalProxyStateChanged(LocalProxyType local_proxy_type, LocalProxyState local_proxy_state, LocalProxyError local_proxy_error) {
        (void)local_proxy_type;
        (void)local_proxy_state;
        (void)local_proxy_error;
    }
    /**
     * 
     * @hidden internal use only
     * @type callback
     * @brief 当特效设置失败时，收到此回调。
     * @param error 特效错误类型。参看 EffectErrorType{@link #EffectErrorType}。
     * @param msg 错误信息。
     * @list Encryption
     */
    
    virtual void onEffectError(EffectErrorType error, const char* msg) {
        (void)error;
        (void)msg;
    }
    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @brief SDK 远端视频渲染发生错误时收到此回调
     * @param stream_id 远端流 ID
     * @param stream_info 远端流信息。参看 StreamInfo{@link #StreamInfo}。
     * @param error 渲染错误类型。参看 RenderError{@link #RenderError}。
     * @param message 错误信息。
     * @note 本回调暂时仅内部使用，开发者无需关注。
     * @list 视频管理
     */
    
    virtual void onRemoteRenderError(const char* stream_id, const StreamInfo& stream_info, RenderError error, const char* message) {
        (void)stream_id;
        (void)stream_info;
        (void)error;
        (void)message;
    }
    /**
     * 
     * @hidden(Linux)
     * @valid since 3.60. 自 3.60 起，该回调替代了 `onStreamMixingEvent` 和 `onPushPublicStreamResult` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，并且仍在使用这两个方法，请迁移到此回调。
     * @type callback
     * @brief 合流转推 CDN / WTN 流状态回调
     * @param info 任务详情，参看 MixedStreamTaskInfo{@link #MixedStreamTaskInfo}。
     * @param event 任务事件，参看 MixedStreamTaskEvent{@link #MixedStreamTaskEvent}。
     * @param error_code 任务错误码，参看 MixedStreamTaskErrorCode{@link #MixedStreamTaskErrorCode}
     * @list CDN 推流
     */
    
    virtual void onMixedStreamEvent(MixedStreamTaskInfo info, MixedStreamTaskEvent event, MixedStreamTaskErrorCode error_code) {
        (void)info;
        (void)event;
        (void)error_code;
    }
    /**
     * 
     * @valid since 3.60.
     * @type callback
     * @hidden(Linux)
     * @brief 单流转推 CDN 状态回调
     * @param task_id 任务 ID
     * @param event 任务状态, 参看 SingleStreamTaskEvent{@link #SingleStreamTaskEvent}
     * @param error_code 错误码，参看 SingleStreamTaskErrorCode{@link #SingleStreamTaskErrorCode}
     * @list CDN 推流
     */
   
    virtual void onSingleStreamEvent(const char *task_id, SingleStreamTaskEvent event, SingleStreamTaskErrorCode error_code) {
        (void)task_id;
        (void)event;
        (void)error_code;
    }    
    /**
     * 
     * @hidden internal use only
     * @valid since 3.60.
     * @type callback
     * @brief 试验性接口回调
     * @param param 回调内容(JSON string)
     * @list 引擎管理
     */
    
    virtual void onExperimentalCallback(const char* param) {
        (void)param;
    }
    
    /**
     * 
     * @valid since 3.60.
     * @hidden(Linux)
     * @type callback
     * @brief 调用 takeLocalSnapshotToFile{@link #IRTCEngine#takeLocalSnapshotToFile} 截取视频画面时，会收到此回调报告截图是否成功，以及截取的图片信息。
     * @param video_source 被截图的视频流，参看 IVideoSource{@link #IVideoSource}。
     * @param file_path 截图文件的保存路径。
     * @param width 截图图像的宽度。单位：像素。
     * @param height 截图图像的高度。单位：像素。
     * @param error_code 截图错误码。请参看 SnapshotErrorCode{@link #SnapshotErrorCode}。
     * @param task_id 截图任务的编号，和 takeLocalSnapshotToFile{@link #IRTCEngine#takeLocalSnapshotToFile} 的返回值一致。
     * @list 高级功能
     */
    
    virtual void onLocalSnapshotTakenToFile(IVideoSource* video_source, const char* file_path, int width, int height, SnapshotErrorCode error_code, long task_id) {
        (void)video_source;
        (void)file_path;
        (void)width;
        (void)height;
        (void)error_code;
        (void)task_id;
    }

    /**
     * 
     * @type callback
     * @valid since 3.60.
     * @brief 调用 takeRemoteSnapshotToFile{@link #IRTCEngine#takeRemoteSnapshotToFile} 截取视频画面时，会收到此回调报告截图是否成功，以及截取的图片信息。
     * @param stream_id 被截图的视频流 ID。
     * @param stream_info 被截图的视频流信息，参看 StreamInfo{@link #StreamInfo}。
     * @param file_path 截图文件的保存路径。
     * @param width 截图图像的宽度。单位：像素。
     * @param height 截图图像的高度。单位：像素。
     * @param error_code 截图错误码。请参看 SnapshotErrorCode{@link #SnapshotErrorCode}。
     * @param task_id 截图任务的编号，和 takeRemoteSnapshotToFile{@link #IRTCEngine#takeRemoteSnapshotToFile} 的返回值一致。
     * @list 高级功能
     */
    
    virtual void onRemoteSnapshotTakenToFile(const char* stream_id, const StreamInfo& stream_info, const char* file_path, int width, int height, SnapshotErrorCode error_code, long task_id) {
        (void)stream_id;
        (void)stream_info;
        (void)file_path;
        (void)width;
        (void)height;
        (void)error_code;
        (void)task_id;
    }

};

} 
