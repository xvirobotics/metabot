/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Audio Defines
 */

#pragma once

#include "bytertc_media_defines.h"
#include <string.h>
#include <math.h>

#define SPECTRUM_SIZE 257

namespace bytertc {
/**
 * 
 * @type keytype
 * @brief 音频采样率，单位为 Hz。
 */

enum AudioSampleRate {
    /**
     * 
     * @brief 默认设置。48000Hz。
     */
    
    kAudioSampleRateAuto = -1,
    /**
     * 
     * @brief 8000Hz
     */
    
    kAudioSampleRate8000 = 8000,
    /**
     * 
     * @brief 11025Hz
     */
    
    kAudioSampleRate11025 = 11025,
    /**
     * 
     * @brief 16000Hz
     */
    
    kAudioSampleRate16000 = 16000,
    /**
     * 
     * @brief 22050Hz
     */
    
    kAudioSampleRate22050 = 22050,
    /**
     * 
     * @brief 24000Hz
     */
    
    kAudioSampleRate24000 = 24000,
    /**
     * 
     * @brief 32000Hz
     */
    
    kAudioSampleRate32000 = 32000,
    /**
     * 
     * @brief 44100Hz
     */
    
    kAudioSampleRate44100 = 44100,
    /**
     * 
     * @brief 48000Hz
     */
    
    kAudioSampleRate48000 = 48000
};
/**
 * 
 * @type keytype
 * @brief 音频声道。
 */

enum AudioChannel {
    /**
     * 
     * @brief 默认设置。
     */
    
    kAudioChannelAuto = -1,
    /**
     * 
     * @brief 单声道
     */
    
    kAudioChannelMono = 1,
    /**
     * 
     * @brief 双声道
     */
    
    kAudioChannelStereo = 2
};

/**
 * 
 * @type keytype
 * @brief 音频格式
 */

struct AudioFormat {
    /**
     * 
     * @brief 音频采样率，详见 AudioSampleRate{@link #AudioSampleRate}
     */
    
    AudioSampleRate sample_rate = kAudioSampleRateAuto;
    /**
     * 
     * @brief 音频声道，详见 AudioChannel{@link #AudioChannel}
     */
    
    AudioChannel channel = kAudioChannelAuto;
    /**
     * 
     * @brief 单次回调的音频帧中包含的采样点数。默认值为 `0`，此时，采样点数取最小值。 <br>
     *        最小值为回调间隔是 0.01s 时的值，即 `sampleRate * channel * 0.01s`。 <br>
     *        最大值是 `2048`。超出取值范围时，采样点数取默认值。 <br>
     *        该参数仅在设置读写回调时生效，调用 enableAudioFrameCallback{@link #IRTCEngine#enableAudioFrameCallback} 开启只读模式回调时设置该参数不生效。
     */
    
    int samples_per_call = 0;
};
/**
 * 
 * @type keytype
 * @brief 返回给音频处理器的音频类型。
 */

enum AudioProcessorMethod{
    /**
     * 
     * @brief 本地采集的音频。
     */
    
    kAudioProcessorMethodRecord = 0,
    /**
     * 
     * @brief 远端音频流的混音音频。
     */
    
    kAudioProcessorMethodPlayback = 1,
    /**
     * 
     * @brief 各个远端音频流。
     */
    
    kAudioProcessorMethodRemoteUser = 2,
    /**
     * 
     * @hidden(Windows,Linux,macOS)
     * @brief 软件耳返音频。
     */
    
    kAudioProcessorMethodEarMonitor = 3,
    /**
     * 
     * @hidden(Linux)
     * @brief 屏幕共享音频。
     */
    
    kAudioProcessorMethodScreen = 4,
};
/**
 * 
 * @type keytype
 * @brief 音频设备类型
 */

enum RTCAudioDeviceType {
    /**
     * 
     * @brief 未知设备类型
     */
    
    kRTCAudioDeviceTypeUnknown = -1,
    /**
     * 
     * @brief 音频渲染设备
     */
    
    kRTCAudioDeviceTypeRenderDevice = 0,
    /**
     * 
     * @brief 音频采集设备
     */
    
    kRTCAudioDeviceTypeCaptureDevice = 1,
    /**
     * 
     * @brief 屏幕流音频设备
     */
    
    kRTCAudioDeviceTypeScreenCaptureDevice = 2,
};

/**
 * 
 * @type keytype
 * @brief 音频播放设备
 */

enum AudioRoute {
    /**
     * 
     * @brief 默认设备
     */
    
    kAudioRouteDefault = -1,
    /**
     * 
     * @brief 有线耳机
     */
    
    kAudioRouteHeadset = 1,
    /**
     * 
     * @brief 听筒
     */
    
    kAudioRouteEarpiece = 2,
    /**
     * 
     * @brief 扬声器
     */
    
    kAudioRouteSpeakerphone = 3,
    /**
     * 
     * @brief 蓝牙耳机
     */
    
    kAudioRouteHeadsetBluetooth = 4,
    /**
     * 
     * @brief USB 设备
     */
    
    kAudioRouteHeadsetUSB = 5
};

/**
 * 
 * @type keytype
 * @brief 音频播放设备
 */

enum AudioPlaybackDevice {
    /**
     * 
     * @brief 有线耳机
     */
    
    kAudioPlaybackDeviceHeadset = 1,
    /**
     * 
     * @brief 听筒
     */
    
    kAudioPlaybackDeviceEarpiece = 2,
    /**
     * 
     * @brief 扬声器
     */
    
    kAudioPlaybackDeviceSpeakerphone = 3,
    /**
     * 
     * @brief 蓝牙耳机
     */
    
    kAudioPlaybackDeviceHeadsetBluetooth = 4,
    /**
     * 
     * @brief USB 设备
     */
    
    kAudioPlaybackDeviceHeadsetUSB = 5
};

/**
 * 
 * @type keytype
 * @brief 音频场景类型。 <br>
 *        选择音频场景后，SDK 会根据操作系统和开麦、闭麦状态，应用通话音量或媒体音量。参看[通话音量与媒体音量](https:
 *        你可以调用 `setAudioScenario` 设置音频场景。 <br>
 *        如果以下音频场景类型无法满足你的业务需要，请联系技术支持人员。
 */

enum AudioScenarioType {
    /** 
     *  
     * @brief 默认场景，适用大部分场景。 
     */
     
    kAudioScenarioTypeDefault = 0,
    /**
     *  
     * @brief 聊天室场景。通话清晰度较高，适用于会议，聊天室场景。 
     */
     
    kAudioScenarioTypeChatRoom = 1,
    /** 
     *  
     * @brief 高音质场景。适用于音乐为主的场景，如 KTV，音乐聊天室等。 
     */ 
     
    kAudioScenarioTypeGameStreaming = 2,
     /** 
      *  
      * @brief 合唱场景。延迟较低。 
      */
      
    kAudioScenarioTypeChorus = 3,
    /** 
      *  
      * @brief 教育场景。适用于以人声教学内容为主的高音质场景。
      */
      
    kAudioScenarioTypeEducation = 4,
    /** 
      *  
      * @brief AI 对话场景。适用于真人与 AI 智能体互动的场景。
      */
      
    kAudioScenarioTypeAiClient = 5,

};

/**
 * 
 * @type keytype
 * @brief 变声特效类型。如需更多类型，联系技术支持。
 */

enum VoiceChangerType {
    /**
     * 
     * @brief 原声，不含特效
     */
    
    kVoiceChangerTypeOriginal = 0,
    /**
     * 
     * @brief 巨人
     */
    
    kVoiceChangerTypeGiant = 1,
    /**
     * 
     * @brief 花栗鼠
     */
    
    kVoiceChangerTypeChipmunk = 2,
    /**
     * 
     * @brief 小黄人
     */
    
    kVoiceChangerTypeMinionst = 3,
    /**
     * 
     * @brief 颤音
     */
    
    kVoiceChangerTypeVibrato = 4,
    /**
     * 
     * @brief 机器人
     */
    
    kVoiceChangerTypeRobot = 5,
};

/**
 * 
 * @type keytype
 * @brief 混响特效类型
 */

enum VoiceReverbType {
    /**
     * 
     * @brief 原声，不含特效
     */
    
    kVoiceReverbTypeOriginal = 0,
    /**
     * 
     * @brief 回声
     */
    
    kVoiceReverbTypeEcho = 1,
    /**
     * 
     * @brief 演唱会
     */
    
    kVoiceReverbTypeConcert = 2,
    /**
     * 
     * @brief 空灵
     */
    
    kVoiceReverbTypeEthereal = 3,
    /**
     * 
     * @brief KTV
     */
    
    kVoiceReverbTypeKTV = 4,
    /**
     * 
     * @brief 录音棚
     */
    
    kVoiceReverbTypeStudio = 5,
    /**
     * 
     * @brief 虚拟立体声
     */
    
    kVoiceReverbTypeVirtualStereo = 6,
    /**
     * 
     * @brief 空旷
     */
    
    kVoiceReverbTypeSpacious = 7,
    /**
     * 
     * @brief 3D 人声
     */
    
    kVoiceReverbType3D = 8,
    /**
     * 
     * @hidden for internal use only
     * @brief 流行
     */
    
    kVoiceReverbTypePop = 9,
    /**
     * 
     * @hidden for internal use only
     * @brief 蹦迪
     */
    
    kVoiceReverbTypeDisco = 10,
    /**
     * 
     * @hidden for internal use only
     * @brief 老唱片
     */
    
    kVoiceReverbTypeOldRecord = 11,
    /**
     * 
     * @hidden for internal use only
     * @brief 和声
     */
    
    kVoiceReverbTypeHarmony = 12,
    /**
     * 
     * @hidden for internal use only
     * @brief 摇滚
     */
    
    kVoiceReverbTypeRock = 13,
    /**
     * 
     * @hidden for internal use only
     * @brief 蓝调
     */
    
    kVoiceReverbTypeBlues = 14,
    /**
     * 
     * @hidden for internal use only
     * @brief 爵士
     */
    
    kVoiceReverbTypeJazz = 15,
    /**
     * 
     * @hidden for internal use only
     * @brief 电子
     */
    
    kVoiceReverbTypeElectronic = 16,
    /**
     * 
     * @hidden for internal use only
     * @brief 黑胶
     */
    
    kVoiceReverbTypeVinyl = 17,
    /**
     * 
     * @hidden for internal use only
     * @brief 密室
     */
    
    kVoiceReverbTypeChamber = 18,
    /**
    * 
    * @hidden for internal use only
    * @brief 增强原声
    */
    
    kVoiceReverbTypeEnhanceOriginal = 19,
    /**
    * 
    * @hidden for internal use only
    * @brief 浴室
    */
    
    kVoiceReverbTypeBathroom = 20,
    /**
    * 
    * @hidden for internal use only
    * @brief 自然
    */
    
    kVoiceReverbTypeNatural = 21,

    /**
    * 
    * @hidden for internal use only
    * @brief 楼道
    */
    
    kVoiceReverbTypeHallway = 22,
};
/**
 * 
 * @type keytype
 * @brief 音频均衡效果。
 */

enum VoiceEqualizationBandFrequency {
    /**
     * 
     * @brief 中心频率为 31Hz 的频带。
     */
    
    kVoiceEqualizationBandFrequency31 = 0,
    /**
     * 
     * @brief 中心频率为 62Hz 的频带。
     */
    
    kVoiceEqualizationBandFrequency62 = 1,
    /**
     * 
     * @brief 中心频率为 125Hz 的频带。
     */
    
    kVoiceEqualizationBandFrequency125 = 2,
        /**
     * 
     * @brief 中心频率为 250Hz 的频带。
     */
    
    kVoiceEqualizationBandFrequency250 = 3,
        /**
     * 
     * @brief 中心频率为 500Hz 的频带。
     */
    
    kVoiceEqualizationBandFrequency500 = 4,
        /**
     * 
     * @brief 中心频率为 1kHz 的频带。
     */
    
    kVoiceEqualizationBandFrequency1k = 5,
    /**
     * 
     * @brief 中心频率为 2kHz 的频带。
     */
    
    kVoiceEqualizationBandFrequency2k = 6,
    /**
     * 
     * @brief 中心频率为 4kHz 的频带。
     */
    
    kVoiceEqualizationBandFrequency4k = 7,
    /**
     * 
     * @brief 中心频率为 8kHz 的频带。
     */
    
    kVoiceEqualizationBandFrequency8k = 8,
    /**
     * 
     * @brief 中心频率为 16kHz 的频带。
     */
    
    kVoiceEqualizationBandFrequency16k = 9,
};
/**
 * 
 * @type keytype
 * @brief 语音均衡效果。
 */

struct VoiceEqualizationConfig {
    /**
     * 
     * @brief 频带。参看 VoiceEqualizationBandFrequency{@link #VoiceEqualizationBandFrequency}。
     */
    
    VoiceEqualizationBandFrequency frequency;
    /**
     * 
     * @brief 频带增益（dB）。取值范围是 `[-15, 15]`。
     */
    
    int gain;
};
/**
 * 
 * @type keytype
 * @brief 音频混响效果。
 */

struct VoiceReverbConfig {
    /**
     * 
     * @brief 混响模拟的房间大小，取值范围 `[0.0, 100.0]`。默认值为 `50.0f`。房间越大，混响越强。
     */
    
    float room_size = 50.0f;
    /**
     * 
     * @brief 混响的拖尾长度，取值范围 `[0.0, 100.0]`。默认值为 `50.0f`。
     */
    
    float decay_time = 50.0f;
    /**
     * 
     * @brief 混响的衰减阻尼大小，取值范围 `[0.0, 100.0]`。默认值为 `50.0f`。
     */
    
    float damping = 50.0f;
    /**
     * 
     * @brief 早期反射信号强度。取值范围 `[-20.0, 10.0]`，单位为 dB。默认值为 `0.0f`。
     */
    
    float wet_gain = 0.0f;
    /**
     * 
     * @brief 原始信号强度。取值范围 `[-20.0, 10.0]`，单位为 dB。默认值为 `0.0f`。
     */
    
    float dry_gain = 0.0f;
    /**
     * 
     * @brief 早期反射信号的延迟。取值范围 `[0.0, 200.0]`，单位为 ms。默认值为 `0.0f`。
     */
    
    float pre_delay = 0.0f;
};

/**
 * 
 * @type keytype
 * @brief 音频流来源的用户 ID 及对应的音量。
 */

struct AudioVolumeInfo {
    /**
     * 
     * @brief 线性音量，取值范围是：[0, 255]，与原始音量呈线性关系，数值越大，音量越大。 <br>
     *        无声为 25 以下（绝对无声为 0，25 以下可视为无声），低音量为 25～76，中音量为 76～204，高音量为 204 以上。
     */
    
    int linear_volume;
    /**
     * 
     * @brief 非线性音量，取值范围是：[–127, 0]，单位 dB。此音量范围是将原始音量进行对数转化，在中低音量时分辨更为灵敏，适用于 Active Speaker（房间内最活跃用户）的识别。 <br>
     *        高音量为 0～–20 dB，中音量为 –20～–40dB，低音量为 –40～–60dB，若低于 –60 dB，则为无声。
     */
    
    int nonlinear_volume;
    /**
     * 
     * @brief 音频流来源的用户 ID
     */
    
    const char* uid;
};

/**
 * 
 * @type keytype
 * @brief 音频混音文件播放状态。
 */

enum AudioMixingState {
    /**
     * 
     * @brief 混音已加载
     */
    
    kAudioMixingStatePreloaded = 0,
    /**
     * 
     * @brief 混音正在播放
     */
    
    kAudioMixingStatePlaying,
    /**
     * 
     * @brief 混音暂停
     */
    
    kAudioMixingStatePaused,
    /**
     * 
     * @brief 混音停止
     */
    
    kAudioMixingStateStopped,
    /**
     * 
     * @brief 混音播放失败
     */
    
    kAudioMixingStateFailed,
    /**
     * 
     * @brief 混音播放结束
     */
    
    kAudioMixingStateFinished,
    /**
     * 
     * @brief 准备 PCM 混音
     */
    
    kAudioMixingStatePCMEnabled,
    /**
     * 
     * @brief PCM 混音播放结束
     */
    
    kAudioMixingStatePCMDisabled,
};
/**
 * 
 * @type keytype
 * @brief 混音错误码。
 */

enum AudioMixingError {
    /**
     * 
     * @brief 正常
     */
    
    kAudioMixingErrorOk = 0,
    /**
     * 
     * @brief 预加载失败。找不到混音文件或者文件长度超出 20s
     */
    
    kAudioMixingErrorPreloadFailed,
    /**
     * 
     * @brief 混音开启失败。找不到混音文件或者混音文件打开失败
     */
    
    kAudioMixingErrorStartFailed,
    /**
     * 
     * @brief 混音 ID 异常
     */
    
    kAudioMixingErrorIdNotFound,
    /**
     * 
     * @brief 设置混音文件的播放位置出错
     */
    
    kAudioMixingErrorSetPositionFailed,
    /**
     * 
     * @brief 音量参数不合法，仅支持设置的音量值为[0, 400]
     */
    
    kAudioMixingErrorInValidVolume,
    /**
     * 
     * @brief 播放的文件与预加载的文件不一致。请先使用 unloadAudioMixing 卸载此前的文件。
     */
    
    kAudioMixingErrorLoadConflict,
    /**
     * 
     * @brief 不支持此混音类型。
     */
    
    kAudioMixingErrorIdTypeNotMatch,
    /**
     * 
     * @brief 设置混音文件的音调不合法
     */
    
    kAudioMixingErrorInValidPitch,
    /**
     * 
     * @brief 设置混音文件的音轨不合法
     */
    
    kAudioMixingErrorInValidAudioTrack,
    /**
     * 
     * @brief 混音文件正在启动中
     */
    
    kAudioMixingErrorIsStarting,
    /**
     * 
     * @brief 设置混音文件的播放速度不合法
     */
    
    kAudioMixingErrorInValidPlaybackSpeed,
    /**
     * 
     * @deprecated since 3.25 and will be deleted in 3.51
     * @brief 混音错误码，失败，已废弃
     */
    
    kAudioMixingErrorCanNotOpen = 701,
};
/**
 * 
 * @type keytype
 * @brief 播放状态。
 */

enum PlayerState {
    /**
     * 
     * @brief 播放未启动
     */
    
    kPlayerStateIdle = 0,
    /**
     * 
     * @brief 已加载
     */
    
    kPlayerStatePreloaded,
    /**
     * 
     * @brief 已打开
     */
    
    kPlayerStateOpened,
    /**
     * 
     * @brief 正在播放
     */
    
    kPlayerStatePlaying,
    /**
     * 
     * @brief 播放已暂停
     */
    
    kPlayerStatePaused,
    /**
     * 
     * @brief 播放已被主动停止
     */
    
    kPlayerStateStopped,
    /**
     * 
     * @brief 播放失败
     */
    
    kPlayerStateFailed,
    /**
     * 
     * @brief 播放自然结束
     */
    
    kPlayerStateFinished,
    /**
     * 
     * @brief 循环播放结束
     */
    
    kPlayerStateLoopFinished,
};

/**
 * 
 * @type keytype
 * @brief 播放事件
 */

enum PlayerEvent {
    /**
     * 
     * @brief 开始切换音轨 <br>
     *        开始调用 `selectAudioTrack`{@link #IMediaPlayer#selectAudioTrack} 时，返回此状态。
     */
    
    kPlayerEventSelectAudioTrackBegin,
    /**
     * 
     * @brief 切换音轨成功 <br>
     *        调用 `selectAudioTrack`{@link #IMediaPlayer#selectAudioTrack} 成功后，播放器切换到指定音轨播放，返回此状态。
     */
    
    kPlayerEventSelectAudioTrackCompleted,
    /**
     * 
     * @brief 切换音轨失败 <br>
     *        调用 `selectAudioTrack`{@link #IMediaPlayer#selectAudioTrack} 失败后，播放器无法切换到指定音轨，继续之前的音轨播放过程，返回此状态。
     */
    
    kPlayerEventSelectAudioTrackFailed,
    /**
     * 
     * @brief 试图移动播放位置 <br>
     *        开始调用 `setPosition`{@link #IMediaPlayer#setPosition} 时，返回此状态。
     */
    
    kPlayerEventSeekBegin,
    /**
     * 
     * @brief 移动播放位置成功 <br>
     *        调用 `setPosition`{@link #IMediaPlayer#setPosition} 成功后，返回此状态。
     */
    
    kPlayerEventSeekCompleted,
    /**
     * 
     * @brief 移动播放位置失败 <br>
     *        调用 `setPosition`{@link #IMediaPlayer#setPosition} 失败时，返回此状态。
     */
    
    kPlayerEventSeekFailed,
};

/**
 * 
 * @type keytype
 * @brief 播放错误码。
 */

enum PlayerError {
    /**
     * 
     * @brief 正常
     */
    
    kPlayerErrorOK = 0,
    /**
     * 
     * @brief 不支持此类型
     */
    
    kPlayerErrorFormatNotSupport,
    /**
     * 
     * @brief 无效的播放路径
     */
    
    kPlayerErrorInvalidPath,
    /**
     * 
     * @brief 未满足前序接口调用的要求。请查看具体接口文档。
     */
    
    kPlayerErrorInvalidState,
    /**
     * 
     * @brief 设置播放位置出错。
     */
    
    kPlayerErrorInvalidPosition,
    /**
     * 
     * @brief 音量参数不合法。
     */
    
    kPlayerErrorInvalidVolume,
    /**
     * 
     * @brief 音调参数设置不合法。
     */
    
    kPlayerErrorInvalidPitch,
    /**
     * 
     * @brief 音轨参数设置不合法。
     */
    
    kPlayerErrorInvalidAudioTrackIndex,
    /**
     * 
     * @brief 播放速度参数设置不合法
     */
    
    kPlayerErrorInvalidPlaybackSpeed,
    /**
     * 
     * @brief 音效 ID 异常。还未加载或播放文件，就调用其他 API。
     */
    
    kPlayerErrorInvalidEffectId,
    /**
     * 
     * @brief 资源正在播放中
     */
     
    kPlayerErrorCurrentlyPlaying,
};

/**
 * 
 * @type keytype
 * @brief 是否开启耳返功能。
 */

enum EarMonitorMode {
    /**
     * 
     * @brief 关闭
     */
    
    kEarMonitorModeOff = 0,
    /**
     * 
     * @brief 开启
     */
    
    kEarMonitorModeOn = 1,
};

/**
 * 
 * @type keytype
 * @brief 耳返音频是否经过本地音频处理。
 */

enum EarMonitorAudioFilter {
    /**
     * 
     * @brief 无音频处理。
     */
    
    kEarMonitorAudioFilterNone = 0x1,
    /**
     * 
     * @brief 经本地音频处理后的音频，包括 3A、变声、混响等 SDK 提供处理能力以及自定义音频处理。
     */
    
    kEarMonitorAudioFilterReuseAudioProcessing = 0x8000
};

/**
 * 
 * @hidden currently not available
 * @type keytype
 * @region 音频管理
 * @brief 音频编码类型
 */

enum AudioCodecType {
    /**
     * 
     * @brief 未知编码类型
     */
    
    kAudioCodecTypeNone = 0,
    /**
     * 
     * @brief Opus 编码类型
     */
    
    kAudioCodecTypeOpus,
    /**
     * 
     * @hidden currently not available
     */
    
    kAudioCodecTypeAAC,
    /**
     * 
     * @hidden currently not available
     */
    
    kAudioCodecTypeAACLC = 2,
    /**
     * 
     * @hidden currently not available
     */
    
    kAudioCodecTypeAACHEv1,
    /**
     * 
     * @hidden currently not available
     */
    
    kAudioCodecTypeAACHEv2,
    /**
     * 
     * @hidden currently not available
     */
    
    kAudioCodecTypeAACLCadts,
};
/**
 * 
 * @type keytype
 * @brief 音频输入格式
 */

enum class AudioFormatType {
    /**
     * 
     * @brief PCM_S16
     */
    
    kRawPCMs16 = 0,
    /**
     * 
     * @brief PCM_S32
     */
    
    kRawPCMs32 = 1,
};

/**
 * 
 * @type keytype
 * @brief 蓝牙模式, 仅能在媒体模式下被设置
 */

enum BluetoothMode {
    /**
     * 
     * @brief 自动选择模式
     */
    
    kBluetoothModeAuto = 0,
    /**
     * 
     * @brief 使用 A2DP 模式
     */
    
    kBluetoothModeA2DP,
    /**
     * 
     * @brief HFP 模式
     */
    
    kBluetoothModeHFP
};

/**
 * 
 * @type keytype
 * @brief 音频输入源类型
 */

enum AudioSourceType {
    /**
     * 
     * @brief 自定义采集音频源
     */
    
    kAudioSourceTypeExternal = 0,
    /**
     * 
     * @brief RTC SDK 内部采集音频源
     */
    
    kAudioSourceTypeInternal,
};

/**
 * 
 * @type keytype
 * @brief 音频输出类型
 */

enum AudioRenderType {
    /**
     * 
     * @brief 自定义渲染
     */
    
    kAudioRenderTypeExternal = 0,
    /**
     * 
     * @brief RTC SDK 内部渲染
     */
    
    kAudioRenderTypeInternal,
};

/**
 * 
 * @type keytype
 * @brief 混音播放类型
 */

enum AudioMixingType {
    /**
     * 
     * @brief 仅本地播放
     */
    
    kAudioMixingTypePlayout,
    /**
     * 
     * @brief 仅远端播放
     */
    
    kAudioMixingTypePublish,
    /**
     * 
     * @brief 本地和远端同时播放
     */
    
    kAudioMixingTypePlayoutAndPublish
};

/**
 * 
 * @type keytype
 * @brief 混音播放声道类型
 */

enum AudioMixingDualMonoMode{
    /**
     * 
     * @brief 和音频文件一致
     */
    
    kAudioMixingDualMonoModeAuto,
    /**
     * 
     * @brief 只能听到音频文件中左声道的音频
     */
    
    kAudioMixingDualMonoModeL,
    /**
     * 
     * @brief 只能听到音频文件中右声道的音频
     */
    
    kAudioMixingDualMonoModeR,
    /**
     * 
     * @brief 能同时听到音频文件中左右声道的音频
     */
    
    kAudioMixingDualMonoModeMix
};
/**
 * 
 * @type keytype
 * @brief 混音配置
 */

struct AudioEffectPlayerConfig {
    /**
     * 
     * @brief 混音播放类型，详见 AudioMixingType{@link #AudioMixingType}
     */ 
    
    AudioMixingType type = kAudioMixingTypePlayoutAndPublish;
    /**
     * 
     * @brief 混音播放次数 <br>
     *       - play_count <= 0: 无限循环
     *       - play_count == 1: 播放一次（默认）
     *       - play_count > 1: 播放 play_count 次
     */
    
    int play_count = 1;
    /**
     * 
     * @brief 与音乐文件原始音调相比的升高/降低值，取值范围为 `[-12，12]`，默认值为 0。每相邻两个值的音高距离相差半音，正值表示升调，负值表示降调。
     */
    
    int pitch = 0;
    /**
     * 
     * @brief 混音起始位置。默认值为 0，单位为毫秒。
     */
    
    int start_pos = 0;
};
/**
 * 
 * @type keytype
 * @brief 混音配置
 */

struct MediaPlayerConfig {
    /**
     * 
     * @brief 混音播放次数 <br>
     *       - play_count <= 0: 无限循环
     *       - play_count == 1: 播放一次（默认）
     *       - play_count > 1: 播放 play_count 次
     */
    
    int play_count = 1;
    /**
     * 
     * @brief 混音起始位置。默认值为 0，单位为毫秒。
     */
    
    int start_pos = 0;
    /**
     * 
     * @brief 设置音频文件混音时，收到 onMediaPlayerPlayingProgress{@link #IMediaPlayerEventHandler#onMediaPlayerPlayingProgress} 的间隔。单位毫秒。 <br>
     *       - interval > 0 时，触发回调。实际间隔为 10 的倍数。如果输入数值不能被 10 整除，将自动向上取整。例如传入 `52`，实际间隔为 60 ms。
     *       - interval <= 0 时，不会触发回调。
     */
     
    int64_t callback_on_progress_interval = 0;
    /**
     * 
     * @brief 在采集音频数据时，附带本地混音文件播放进度的时间戳。启用此功能会提升远端人声和音频文件混音播放时的同步效果。 <br>
     *        - 仅在单个音频文件混音时使用有效。
     *        - `true` 时开启此功能，`false` 时关闭此功能，默认为关闭。
     */
    
    bool sync_progress_to_record_frame = false;
    /**
    * 
    * @brief 是否自动播放。如果不自动播放，调用 start{@link #IMediaPlayer#start} 播放音乐文件。
    */
    
    bool auto_play = true;
    /**
     * 
     * @brief 混音播放类型，详见 AudioMixingType{@link #AudioMixingType}
     */
    
    AudioMixingType type = kAudioMixingTypePlayoutAndPublish;
};

/**
 * 
 * @type keytype
 * @brief 音量回调模式。
 */

enum AudioReportMode {
    /**
     * 
     * @brief 默认始终开启音量回调。
     */
    
    kAudioReportModeNormal,
   /**
     * 
     * @brief 可见用户进房并停止推流后，关闭音量回调。
     */
    
    kAudioReportModeDisconnect,
    /**
     * 
     * @brief 可见用户进房并停止推流后，开启音量回调，回调值重置为 0。
     */
    
    kAudioReportModeReset,
};

/**
 * 
 * @type keytype
 * @brief onLocalAudioPropertiesReport{@link #IRTCEngineEventHandler#onLocalAudioPropertiesReport} 中包含的音频信息的范围。
 */

enum AudioPropertiesMode {
    /**
     * 
     * @brief 仅包含本地麦克风采集的音频数据和本地屏幕音频采集数据。
     */
    
    kAudioPropertiesModeMicrophone,
    /**
     * 
     * @brief 包含以下信息： <br>
     *        - 本地麦克风采集的音频数据和本地屏幕音频采集数据；
     *        - 本地混音的音频数据。
     */
    
    kAudioPropertiesModeAudioMixing
};

/**
 * 
 * @type keytype
 * @brief 音频属性信息提示的相关配置。
 */

struct AudioPropertiesConfig {
    /**
     * 
     * @brief 信息提示间隔，单位：ms <br>
     *       - `<= 0`: 关闭信息提示
     *       - `(0,100]`: 开启信息提示，不合法的 interval 值，SDK 自动设置为 100ms
     *       - `> 100`: 开启信息提示，并将信息提示间隔设置为此值
     */
    
    int interval = 0;

    /**
     * 
     * @brief 是否开启音频频谱检测。
     */
    
    bool enable_spectrum = false;

    /**
     * 
     * @brief 是否开启人声检测 (VAD)。
     */
    
    bool enable_vad = false;

    /**
     * 
     * @brief 音量回调配置模式。详见 AudioReportMode{@link #AudioReportMode}。
     */
    
    AudioReportMode local_main_report_mode = kAudioReportModeNormal;

    /**
     * 
     * @brief 适用于音频属性信息提示的平滑系数。取值范围是 `(0.0, 1.0]`。 <br>
     *        默认值为 `1.0`，不开启平滑效果；值越小，提示音量平滑效果越明显。如果要开启平滑效果，可以设置为 `0.3`。
     */
    
    float smooth = 1.0f;   
        
    /**
     * 
     * @brief onLocalAudioPropertiesReport{@link #IRTCEngineEventHandler#onLocalAudioPropertiesReport} 中包含音频数据的范围。参看 AudioPropertiesMode{@link #AudioPropertiesMode}。 <br>
     *        默认仅包含本地麦克风采集的音频数据和本地屏幕音频采集数据。
     */
    
    AudioPropertiesMode audio_report_mode = kAudioPropertiesModeMicrophone;
    /**
     * 
     * @brief 是否回调本地用户的人声基频。
     */
    
    bool enable_voice_pitch = false;
};
/**
 * 
 * @type keytype
 * @brief 音频属性信息
 */

struct AudioPropertiesInfo {
    /**
     * 
     * @brief 线性音量，与原始音量呈线性关系，数值越大，音量越大。取值范围是：[0,255]。 <br>
     *        - [0, 25]: 无声
     *        - [26, 75]: 低音量
     *        - [76, 204]: 中音量
     *        - [205, 255]: 高音量
     */
    
    int linear_volume = 0;
    /**
     * 
     * @brief 非线性音量。由原始音量的对数值转化而来，因此在中低音量时更灵敏，可以用作 Active Speaker（房间内最活跃用户）的识别。取值范围是：[-127，0]，单位 dB。 <br>
     *        - [-127, -60]: 无声
     *        - [-59, -40]: 低音量
     *        - [-39, -20]: 中音量
     *        - [-19, 0]: 高音量
     */
    
    int nonlinear_volume = 0;
    /**
     * 
     * @brief 频谱数组。默认长度为 257。
     */
    
    float spectrum[SPECTRUM_SIZE] = {0};
    /**
     * 
     * @brief 人声检测（VAD）结果 <br>
     *        - 1: 检测到人声。
     *        - 0: 未检测到人声。
     *        - -1: 未开启 VAD。
     */
    
    int vad = -1;
    
    /**
     * 
     * @brief 本地用户的人声基频，单位为赫兹。 <br>
     *        同时满足以下两个条件时，返回的值为本地用户的人声基频： <br>
     *      - 调用 enableAudioPropertiesReport{@link #IRTCEngine#enableAudioPropertiesReport}，并设置参数 enable_voice_pitch 的值为 `true`。
     *      - 本地采集的音频中包含本地用户的人声。
     *        其他情况下返回 `0`。
     */
        
    double voice_pitch = 0;
};

/**
 * 
 * @type keytype
 * @brief 远端音频属性信息
 */

struct RemoteAudioPropertiesInfo {
    /**
     * 
     * @type keytype
     * @brief 流 ID
     */
    
    const char* stream_id;
    /**
     * 
     * @brief 流信息。参看 StreamInfo{@link #StreamInfo}
     */
    
    StreamInfo stream_info;
    /**
     * 
     * @type keytype
     * @brief 音频属性信息，详见 AudioPropertiesInfo{@link #AudioPropertiesInfo}
     */
    
    AudioPropertiesInfo audio_properties_info;
};

/**
 * 
 * @type keytype
 * @brief 本地音频属性信息
 */

struct LocalAudioPropertiesInfo {
    /**
     * 
     * @type keytype
     * @brief 音频源
     */
    
    IAudioSource* audio_source;
    /**
     * 
     * @type keytype
     * @brief 音频属性信息，详见 AudioPropertiesInfo{@link #AudioPropertiesInfo}
     */
    
    AudioPropertiesInfo audio_properties_info;
};

/**
 * 
 * @type keytype
 * @brief 音质档位
 */

enum AudioProfileType {
    /**
     * 
     * @brief 默认音质 <br>
     *        服务器下发或客户端已设置的 RoomProfileType{@link #RoomProfileType} 的音质配置
     */
    
    kAudioProfileTypeDefault = 0,
    /**
     * 
     * @brief 流畅 <br>
     *        单声道，采样率为 16 kHz，编码码率为 32 Kbps。 <br>
     *        流畅优先、低功耗、低流量消耗，适用于大部分游戏场景，如小队语音、组队语音、国战语音等。
     */
    
    kAudioProfileTypeFluent = 1,
    /**
     * 
     * @brief 单声道标准音质。 <br>
     *        采样率为 24 kHz，编码码率为 48 Kbps。 <br>
     *        适用于对音质有一定要求的场景，同时延时、功耗和流量消耗相对适中，适合教育场景和狼人杀等游戏。
     */
    
    kAudioProfileTypeStandard = 2,
    /**
     * 
     * @brief 双声道音乐音质 <br>
     *        采样率为 48 kHz，编码码率为 128 kbps。 <br>
     *        超高音质，同时延时、功耗和流量消耗相对较大，适用于连麦 PK 等音乐场景。 <br>
     *        游戏场景不建议使用。
     */
    
    kAudioProfileTypeHD = 3,
    /**
     * 
     * @brief 双声道标准音质。采样率为 48 KHz，编码码率最大值为 80 Kbps
     */
    
    kAudioProfileTypeStandardStereo = 4,
    /**
     * 
     * @brief 单声道音乐音质。采样率为 48 kHz，编码码率最大值为 64 Kbps
     */
    
    kAudioProfileTypeHDMono = 5,
};

/**
 * 
 * @type keytype
 * @brief 降噪模式。降噪算法受调用 joinRoom{@link #IRTCRoom#joinRoom} 时设置的房间模式影响。
 */

enum AnsMode {
    /**
     * 
     * @brief 禁用音频降噪。
     */
    
    kAnsModeDisable = 0,
    /**
     * 
     * @brief 适用于微弱降噪。
     */
    
    kAnsModeLow = 1,
    /**
     * 
     * @brief 适用于抑制中度平稳噪音，如空调声、风扇声。
     */
    
    kAnsModeMedium = 2,
    /**
     * 
     * @brief 适用于抑制嘈杂非平稳噪音，如键盘声、敲击声、碰撞声、动物叫声。
     */
    
    kAnsModeHigh = 3,
    /**
     * 
     * @brief 启用音频降噪能力。具体的降噪算法由 RTC 决定。
     */
    
    kAnsModeAutomatic = 4,
};

/**
 * 
 * @type keytype
 * @brief 本地用户在房间内的位置坐标，需自行建立空间直角坐标系
 */

struct Position {
    /**
     * 
     * @brief x 坐标
     */
    
    float x;
    /**
     * 
     * @brief y 坐标
     */
    
    float y;
    /**
     * 
     * @brief z 坐标
     */
    
    float z;
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool isEqual(const Position& pos) const {
        return (fabs(x - pos.x) < 1e-2) &&
        (fabs(y - pos.y) < 1e-2) &&
        (fabs(z - pos.z) < 1e-2);
    }
};

/**
 * 
 * @type keytype
 * @brief 方向朝向信息
 */

struct Orientation {
    /**
     * 
     * @brief x 坐标
     */
    
    float x;
    /**
     * 
     * @brief y 坐标
     */
    
    float y;
    /**
     * 
     * @brief z 坐标
     */
    
    float z;
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool isEqual(const Orientation& orientation) const {
        return x == orientation.x && y == orientation.y && z == orientation.z;
    }
};

/**
 * 
 * @type keytype
 * @brief 三维朝向信息，三个向量需要两两垂直。参看 Orientation{@link #Orientation}。
 */

struct HumanOrientation {
    /**
     * 
     * @brief 正前方朝向，默认值为 {1,0,0}，即正前方朝向 x 轴正方向
     */
    
    Orientation forward =  { 1, 0, 0 };
    /**
     * 
     * @brief 正右方朝向，默认值为 {0,1,0}，即右手朝向 y 轴正方向
     */
    
    Orientation right = { 0, 1, 0 };
    /**
     * 
     * @brief 正上方朝向，默认值为 {0,0,1}，即头顶朝向 z 轴正方向
     */
    
    Orientation up = { 0, 0, 1 };
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool isEqual(const HumanOrientation& human_orientation) const {
        return forward.isEqual(human_orientation.forward) && right.isEqual(human_orientation.right) && up.isEqual(human_orientation.up);
    }
};

/**
 * 
 * @type keytype
 * @brief 音频音量是否可设置
 */

enum AudioAbilityType {
    /**
     * 
     * @brief 未知
     */
    
    kAudioAbilityTypeUnknown = -1,
    /**
     * 
     * @brief 音量可设置
     */
    
    kAudioAbilityTypeAble = 0,
    /**
     * 
     * @brief 音量不可设置
     */
    
    kAudioAbilityTypeUnable = 1,
};
/**
 * 
 * @type keytype
 * @brief 远端音频流精准对齐模式
 */

enum AudioAlignmentMode {
    /**
     * 
     * @brief 不对齐
     */
    
    kAudioAlignmentModeOff,
    /**
     * 
     * @brief 远端音频流都对齐伴奏进度同步播放
     */
    
    kAudioAlignmentModeAudioMixing,
};

/**
 * 
 * @type keytype
 * @brief 音频设备信息
 */

struct AudioDeviceInfo {
    /**
     * 
     * @brief 设备 ID
     */
    
    char device_id[MAX_DEVICE_ID_LENGTH];
    /**
     * 
     * @brief 设备全称，包含设备类型与设备名称。例如 "扬声器 (XYZ Audio Adapter)"
     */
    
    char device_name[MAX_DEVICE_ID_LENGTH];
    /**
     * 
     * @brief 设备名称，不含设备类型。例如 "XYZ Audio Adapter"
     */
    
    char device_short_name[MAX_DEVICE_ID_LENGTH];
    /**
     * 
     * @brief 设备所连接的声卡 ID，便于选择使用同一声卡的扬声器和麦克风。
     */
    
    char device_container_id[MAX_DEVICE_ID_LENGTH];
    /**
     * 
     * @brief 设备的厂商 ID
     */
    
    int64_t device_vid;
    /**
     * 
     * @brief 设备的产品编码
     */
    
    int64_t device_pid;
    /**
     * 
     * @brief 设备的传输方式
     */
    
    DeviceTransportType transport_type;
    /**
     * 
     * @brief 是否支持设置音量
     */
    
    AudioAbilityType volume_settable;
    /**
     * 
     * @brief 是否系统默认设备
     */
    
    bool is_system_default;
    /**
     * 
     * @hidden constructor/destructor
     */
    
    AudioDeviceInfo() {
        memset(device_id, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_name, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_short_name, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_container_id, 0, MAX_DEVICE_ID_LENGTH);
        this->device_vid = 0;
        this->device_pid = 0;
        this->transport_type = DeviceTransportType::kDeviceTransportTypeUnknown;
        this->volume_settable = AudioAbilityType::kAudioAbilityTypeUnknown;
        this->is_system_default = false;
    }
    /**
     * 
     * @hidden constructor/destructor
     */
    
    AudioDeviceInfo& operator=(const AudioDeviceInfo& src) {
        if (this != &src) {
            strncpy(device_id, src.device_id, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_name, src.device_name, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_short_name, src.device_short_name, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_container_id, src.device_container_id, MAX_DEVICE_ID_LENGTH - 1);
            device_id[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_name[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_short_name[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_container_id[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_vid = src.device_vid;
            device_pid = src.device_pid;
            transport_type = src.transport_type;
            volume_settable = src.volume_settable;
            is_system_default = src.is_system_default;
        }

        return *this;
    }
};
/**
 * 
 * @type keytype
 * @brief 标准音高数据
 */

struct StandardPitchInfo {
    /**
     * 
     * @brief 开始时间，单位 ms。
     */
    
    int start_time;
    /**
     * 
     * @brief 持续时间，单位 ms。
     */
    
    int duration;
    /**
     * 
     * @brief 音高。
     */
    
    int pitch;
};
/**
 * 
 * @type keytype
 * @hidden
 * @brief K 歌打分维度。
 */

enum MulDimSingScoringMode {
    /**
     * 
     * @brief 按照音高进行评分。
     */
    
    kMulDimSingScoringModeNote = 0
};
/**
 * 
 * @type keytype
 * @hidden
 * @brief K 歌评分配置。
 */

struct SingScoringConfig {
    /**
     * 
     * @brief 音频采样率。仅支持 44100 Hz、48000 Hz。
     */
    
    AudioSampleRate sample_rate;
    /**
     * 
     * @brief 打分维度，详见 MulDimSingScoringMode{@link #MulDimSingScoringMode}。
     */
    
    MulDimSingScoringMode mode;
    /**
     * 
     * @brief 歌词文件路径。打分功能仅支持 KRC 歌词文件。
     */
    
    const char* lyrics_filepath;
    /**
     * 
     * @brief 歌曲 midi 文件路径。
     */
    
    const char* midi_filepath;
};
/**
 * 
 * @type keytype
 * @hidden
 * @brief 实时评分信息。
 */

struct SingScoringRealtimeInfo {
    /**
     * 
     * @brief 当前播放进度。
     */
    
    int current_position;
    /**
     * 
     * @brief 演唱者的音高。
     */
    
    int user_pitch;
    /**
     * 
     * @brief 标准音高。
     */
    
    int standard_pitch;
    /**
     * 
     * @brief 歌词分句索引。
     */
    
    int sentence_index;
    /**
     * 
     * @brief 上一句歌词的评分。
     */
    
    int sentence_score;
    /**
     * 
     * @brief 当前演唱的累计分数。
     */
    
    int total_score;
    /**
     * 
     * @brief 当前演唱的平均分数。
     */
    
    int average_score;
};
/**
 * 
 * @type keytype
 * @brief 音频文件录制内容来源。
 */

enum AudioFrameSource {
    /**
     * 
     * @brief 本地麦克风采集的音频数据。
     */
    
    kAudioFrameSourceMic = 0,
    /**
     * 
     * @brief 远端所有用户混音后的数据
     */
    
    kAudioFrameSourcePlayback = 1,
    /**
     * 
     * @brief 本地麦克风和所有远端用户音频流的混音后的数据
     */
    
    kAudioFrameSourceMixed = 2,
};
/**
 * 
 * @type keytype
 * @brief 音频质量。
 */

enum AudioQuality {
    /**
     * 
     * @brief 低音质
     */
    
    kAudioQualityLow = 0,
    /**
     * 
     * @brief 中音质
     */
    
    kAudioQualityMedium = 1,
    /**
     * 
     * @brief 高音质
     */
    
    kAudioQualityHigh = 2,
    /**
     * 
     * @brief 超高音质
     */
    
    kAudioQualityUltraHigh = 3,
};
/**
 * 
 * @type keytype
 * @brief 录音配置
 */

struct AudioRecordingConfig {
    /**
     * 
     * @hidden constructor/destructor
     */
    
    AudioRecordingConfig() {
        absolute_file_name = nullptr;
        frame_source = kAudioFrameSourceMixed;
        sample_rate = kAudioSampleRateAuto;
        channel = kAudioChannelAuto;
        quality = kAudioQualityMedium;
    }
    /**
     * 
     * @brief 录制文件路径。一个有读写权限的绝对路径，包含文件名和文件后缀。 <br>
     * 录制文件的格式仅支持 .aac 和 .wav。
     */
    
    const char* absolute_file_name;
    /**
     * 
     * @brief 录音内容来源，参看 AudioFrameSource{@link #AudioFrameSource}。 <br>
     * 默认为 kAudioFrameSourceMixed = 2。
     */
    
    AudioFrameSource frame_source;
    /**
     * 
     * @brief 录音采样率。参看 AudioSampleRate{@link #AudioSampleRate}。
     */
    
    AudioSampleRate sample_rate;
    /**
     * 
     * @brief 录音音频声道。参看 AudioChannel{@link #AudioChannel}。 <br>
     *        如果录制时设置的的音频声道数与采集时的音频声道数不同： <br>
     *        - 如果采集的声道数为 1，录制的声道数为 2，那么，录制的音频为经过单声道数据拷贝后的双声道数据，而不是立体声。
     *        - 如果采集的声道数为 2，录制的声道数为 1，那么，录制的音频为经过双声道数据混合后的单声道数据。
     */
    
    AudioChannel channel;
    /**
     * 
     * @brief 录音音质。仅在录制文件格式为 .aac 时可以设置。参看 AudioQuality{@link #AudioQuality}。 <br>
     * 采样率为 32kHz 时，不同音质录制文件（时长为 10min）的大小分别是： <br>
     *        - 低音质：1.2MB；
     *        - 【默认】中音质：2MB；
     *        - 高音质：3.75MB；
     *        - 超高音质：7.5MB。
     */
    
    AudioQuality quality;
};
/**
 * 
 * @type keytype
 * @brief 录音配置
 */

enum AudioRecordingState {
    /**
     * 
     * @brief 录制异常
     */
    
    kAudioRecordingStateError = 0,
    /**
     * 
     * @brief 录制进行中
     */
    
    kAudioRecordingStateProcessing = 1,
    /**
     * 
     * @brief 已结束录制，并且录制文件保存成功。
     */
        
    kAudioRecordingStateSuccess = 2,
};
/**
 * 
 * @type errorcode
 * @brief 音频文件录制的错误码
 */

enum AudioRecordingErrorCode {
    /**
     * 
     * @brief 录制正常
     */
    
    kAudioRecordingErrorCodeOk = 0,
    /**
     * 
     * @brief 没有文件写权限
     */
    
    kAudioRecordingErrorCodeNoPermission = -1,
    /**
     * 
     * @brief 没有进入房间
     */
    
    kAudioRecordingErrorCodeNotInRoom = -2,
    /**
     * 
     * @brief 录制已经开始
     */
    
    kAudioRecordingErrorCodeAlreadyStarted = -3,
    /**
     * 
     * @brief 录制还未开始
     */
    
    kAudioRecordingErrorCodeNotStarted = -4,
    /**
     * 
     * @brief 录制失败。文件格式不支持。
     */
    
    kAudioRecordingErrorCodeNotSupport = -5,
    /**
     * 
     * @brief 其他异常
     */
    
    kAudioRecordingErrorCodeOther = -6,
};
/**
 * 
 * @hidden(macOS, Windows, Linux)
 * @type keytype
 * @brief 蜂窝网络辅助增强应用的媒体模式
 */

struct AudioEnhancementConfig {
    /**
     * 
     * @brief 对信令消息，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_signaling = false;
    /**
     * 
     * @brief 对音频，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_audio = false;
};

}  