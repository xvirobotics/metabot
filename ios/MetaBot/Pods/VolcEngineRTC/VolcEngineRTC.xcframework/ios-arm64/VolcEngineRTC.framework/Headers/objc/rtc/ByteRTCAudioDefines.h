/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief Objective-C VolcEngineRTC Audio Defines
*/

#import "ByteRTCMediaDefines.h"

#pragma mark - ByteRTCAudioSource
BYTERTC_APPLE_EXPORT @interface ByteRTCAudioSource : NSObject

@end

/**
 * 
 * @hidden(macOS)
 * @type keytype
 * @brief 音频场景类型。 <br>
 *        选择音频场景后，SDK 会自动根据客户端音频采集播放设备和状态，适用通话音量/媒体音量。 
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioScenarioType) {
    /** 
     *  
     * @brief 默认场景，适用大部分场景。 
     */
     
    ByteRTCAudioScenarioTypeDefault = 0,
    /**
     *  
     * @brief 聊天室场景。通话清晰度较高，适用于会议，聊天室场景。 
     */
     
    ByteRTCAudioScenarioTypeChatRoom = 1,
    /** 
     *  
     * @brief 游戏语音场景。
     */ 
     
    ByteRTCAudioScenarioTypeGameStreaming = 2,
    /**
     *  
     * @brief 合唱场景。延迟较低。
     */
    
    ByteRTCAudioScenarioTypeChorus = 3,
    /**
     *  
     * @brief 教育场景。适用于以人声教学内容为主的高音质场景。
     */
    
    ByteRTCAudioScenarioTypeEducation = 4,
    /**
     *  
     * @brief AI 对话场景。适用于真人与 AI 智能体互动的场景。
     */
    
    ByteRTCAudioScenarioTypeAiClient = 5,
};

/**
 * 
 * @type keytype
 * @brief 变声特效类型。如需更多类型，联系技术支持。
 */

typedef NS_ENUM(NSInteger, ByteRTCVoiceChangerType) {
    /**
     * 
     * @brief 原声，不含特效
     */
    
    ByteRTCVoiceChangerOriginal = 0,
    /**
     * 
     * @brief 巨人
     */
    
    ByteRTCVoiceChangerGiant = 1,
    /**
     * 
     * @brief 花栗鼠
     */
    
    ByteRTCVoiceChangerChipmunk = 2,
    /**
     * 
     * @brief 小黄人
     */
    
    ByteRTCVoiceChangerMinionst = 3,
    /**
     * 
     * @brief 颤音
     */
    
    ByteRTCVoiceChangerVibrato = 4,
    /**
     * 
     * @brief 机器人
     */
    
    ByteRTCVoiceChangerRobot = 5,
};

/**
 * 
 * @type keytype
 * @brief 混响特效类型
 */

typedef NS_ENUM(NSInteger, ByteRTCVoiceReverbType) {
    /**
     * 
     * @brief 原声，不含特效
     */
    
    ByteRTCVoiceReverbOriginal = 0,
    /**
     * 
     * @brief 回声
     */
    
    ByteRTCVoiceReverbEcho = 1,
    /**
     * 
     * @brief 演唱会
     */
    
    ByteRTCVoiceReverbConcert = 2,
    /**
     * 
     * @brief 空灵
     */
    
    ByteRTCVoiceReverbEthereal = 3,
    /**
     * 
     * @brief KTV
     */
    
    ByteRTCVoiceReverbKTV = 4,
    /**
     * 
     * @brief 录音棚
     */
    
    ByteRTCVoiceReverbStudio = 5,
    /**
     * 
     * @brief 虚拟立体声
     */
    
    ByteRTCVoiceReverbVirtualStereo = 6,
    /**
     * 
     * @brief 空旷
     */
    
    ByteRTCVoiceReverbSpacious = 7,
    /**
     * 
     * @brief 3D 人声
     */
    
    ByteRTCVoiceReverb3D = 8,
    /**
     * 
     * @hidden internal use
     * @brief 流行
     */
    
    ByteRTCVoiceReverbPop = 9,
    /**
     * 
     * @hidden internal use
     * @brief 蹦迪
     */
    
    ByteRTCVoiceReverbDisco = 10,
    /**
     * 
     * @hidden internal use
     * @brief 老唱片
     */
    
    ByteRTCVoiceReverbOldRecord = 11,
    /**
     * 
     * @hidden internal use
     * @brief 和声
     */
    
    ByteRTCVoiceReverbHarmony = 12,
    /**
     * 
     * @hidden internal use
     * @brief 摇滚
     */
    
    ByteRTCVoiceReverbRock = 13,
    /**
     * 
     * @hidden internal use
     * @brief 蓝调
     */
    
    ByteRTCVoiceReverbBlues = 14,
    /**
     * 
     * @hidden internal use
     * @brief 爵士
     */
    
    ByteRTCVoiceReverbJazz = 15,
    /**
     * 
     * @hidden internal use
     * @brief 电子
     */
    
    ByteRTCVoiceReverbElectronic = 16,
    /**
     * 
     * @hidden internal use
     * @brief 黑胶
     */
    
    ByteRTCVoiceReverbVinyl = 17,
    /**
     * 
     * @hidden internal use
     * @brief 密室
     */
    
    ByteRTCVoiceReverbChamber = 18,

    /**
     * 
     * @hidden internal use
     * @brief 增强原声
     */
    
    ByteRTCVoiceReverbEnhanceOriginal = 19,

    /**
    * 
    * @hidden internal use
    * @brief 浴室
    */
    
    ByteRTCVoiceReverbBathroom = 20,

    /**
    * 
    * @hidden internal use
    * @brief 自然
    */
    
    ByteRTCVoiceReverbNatural = 21,

    /**
    * 
    * @hidden internal use
    * @brief 楼道
    */
    
    ByteRTCVoiceReverbHallway = 22,
};
/**
 * 
 * @type keytype
 * @brief 音频均衡效果。
 */

typedef NS_ENUM(NSInteger, ByteRTCBandFrequency) {
    /**
     * 
     * @brief 中心频率为 31Hz 的频带。
     */
    
    ByteRTCBandFrequency31 = 0,
    /**
     * 
     * @brief 中心频率为 62Hz 的频带。
     */
    
    ByteRTCBandFrequency62 = 1,
    /**
     * 
     * @brief 中心频率为 125Hz 的频带。
     */
    
    ByteRTCBandFrequency125 = 2,
    /**
     * 
     * @brief 中心频率为 250Hz 的频带。
     */
    
    ByteRTCBandFrequency250 = 3,
    /**
     * 
     * @brief 中心频率为 500Hz 的频带。
     */
    
    ByteRTCBandFrequency500 = 4,
    /**
     * 
     * @brief 中心频率为 1kHz 的频带。
     */
    
    ByteRTCBandFrequency1k = 5,
    /**
     * 
     * @brief 中心频率为 2kHz 的频带。
     */
    
    ByteRTCBandFrequency2k = 6,
    /**
     * 
     * @brief 中心频率为 4kHz 的频带。
     */
    
    ByteRTCBandFrequency4k = 7,
    /**
     * 
     * @brief 中心频率为 8kHz 的频带。
     */
    
    ByteRTCBandFrequency8k = 8,
    /**
     * 
     * @brief 中心频率为 16kHz 的频带。
     */
    
    ByteRTCBandFrequency16k = 9,
};
/**
 * 
 * @type keytype
 * @brief 语音均衡效果。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCVoiceEqualizationConfig : NSObject
/**
 * 
 * @brief 频带。参看 ByteRTCBandFrequency{@link #ByteRTCBandFrequency}。
 */

@property(assign, nonatomic) ByteRTCBandFrequency frequency;
/**
 * 
 * @brief 频带增益（dB）。取值范围是 `[-15, 15]`。
 */

@property(assign, nonatomic) int gain;
@end

/**
 * 
 * @type keytype
 * @brief 音频混响效果。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCVoiceReverbConfig : NSObject
/**
 * 
 * @brief 混响模拟的房间大小，取值范围 `[0.0, 100.0]`。默认值为 `50.0f`。房间越大，混响越强。
 */

@property(assign, nonatomic) float roomSize;
/**
 * 
 * @brief 混响的拖尾长度，取值范围 `[0.0, 100.0]`。默认值为 `50.0f`。
 */

@property(assign, nonatomic) float decayTime;
/**
 * 
 * @brief 混响的衰减阻尼大小，取值范围 `[0.0, 100.0]`。默认值为 `50.0f`。
 */

@property(assign, nonatomic) float damping;
/**
 * 
 * @brief 早期反射信号强度。取值范围 `[-20.0, 10.0]`，单位为 dB。默认值为 `0.0f`。
 */

@property(assign, nonatomic) float wetGain;
/**
 * 
 * @brief 原始信号强度。取值范围 `[-20.0, 10.0]`，单位为 dB。默认值为 `0.0f`。
 */

@property(assign, nonatomic) float dryGain;
/**
 * 
 * @brief 早期反射信号的延迟。取值范围 `[0.0, 200.0]`，单位为 ms。默认值为 `0.0f`。
 */

@property(assign, nonatomic) float preDelay;
@end

/**
 * 
 * @type keytype
 * @brief 音质档位
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioProfileType) {
    /**
     * 
     * @brief 默认音质 <br>
     *        服务器下发或客户端已设置的 ByteRTCRoomProfile{@link #ByteRTCRoomProfile} 的音质配置
     */
    
    ByteRTCAudioProfileDefault = 0,
    /**
     * 
     * @brief 流畅 <br>
     *        单声道，采样率为 16 kHz，编码码率为 32 Kbps。 <br>
     *        流畅优先、低功耗、低流量消耗，适用于大部分游戏场景，如小队语音、组队语音、国战语音等。
     */
    
    ByteRTCAudioProfileFluent = 1,
    /**
     * 
     * @brief 单声道标准音质。 <br>
     *        采样率为 24 kHz，编码码率为 48 Kbps。 <br>
     *        适用于对音质有一定要求的场景，同时延时、功耗和流量消耗相对适中，适合教育场景和狼人杀等游戏。
     */
    
    ByteRTCAudioProfileStandard = 2,
    /**
     * 
     * @brief 双声道音乐音质 <br>
     *        采样率为 48 kHz，编码码率为 128 kbps。 <br>
     *        超高音质，同时延时、功耗和流量消耗相对较大，适用于连麦 PK 等音乐场景。 <br>
     *        游戏场景不建议使用。
     */
    
    ByteRTCAudioProfileHD = 3,
    /**
     * 
     * @brief 双声道标准音质。采样率为 48 KHz，编码码率最大值为 80 Kbps
     */
    
    ByteRTCAudioProfileStandardStereo = 4,
    /**
     * 
     * @brief 单声道音乐音质。采样率为 48 kHz，编码码率最大值为 64 Kbps
     */
    
    ByteRTCAudioProfileHDMono = 5,
};

/**
 * 
 * @type keytype
 * @brief 降噪模式。降噪算法受调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 时设置的房间模式影响。
 */

typedef NS_ENUM(NSInteger, ByteRTCAnsMode) {
    /**
     * 
     * @brief 关闭所有音频降噪能力。
     */
    
    ByteRTCAnsModeDisable = 0,
   /**
     * 
     * @brief 适用于微弱降噪。
     */
    
    ByteRTCAnsModeLow = 1,
    /**
     * 
     * @brief 适用于抑制中度平稳噪声，如空调声和风扇声。
     */
    
    ByteRTCAnsModeMedium = 2,
     /**
     * 
     * @brief 适用于抑制嘈杂非平稳噪声，如键盘声、敲击声、碰撞声、动物叫声。
     */
    
    ByteRTCAnsModeHigh = 3,
    /**
     * 
     * @brief 启用音频降噪能力。具体的降噪算法由 RTC 智能决策。
     */
    
    ByteRTCAnsModeAutomatic = 4,
};

/**
 * 
 * @hidden
 * @deprecated since 3.45 and will be deleted in 3.51.
 * @type keytype
 * @brief 语音通话模式，在小队语音里用这个接口设置自己的发送模式
 */

typedef NS_ENUM(NSUInteger, ByteRTCRangeAudioMode) {
    /**
     * 
     * @brief 默认模式
     */
    
    ByteRTCRangeAudioModeUndefined = 0,
    /**
     * 
     * @brief 小队模式
     */
    
    ByteRTCRangeAudioModeTeam = 1,
    /**
     * 
     * @brief 世界模式
     */
    
    ByteRTCRangeAudioModeWorld = 2,
};

/**
 * 
 * @type keytype
 * @brief 当前音频设备类型
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioDeviceType) {
    /**
     * 
     * @brief 未知音频设备
     */
    
    ByteRTCAudioDeviceTypeUnknown = -1,
    /**
     * 
     * @brief 音频渲染设备
     */
    
    ByteRTCAudioDeviceTypeRenderDevice = 0,
    /**
     * 
     * @brief 音频采集设备
     */
    
    ByteRTCAudioDeviceTypeCaptureDevice = 1,
    /**
     * 
     * @brief 屏幕流音频设备
     */
    
    ByteRTCAudioDeviceTypeScreenCaptureDevice = 2
};
/**
 * 
 * @type keytype
 * @brief 音频采样率，单位为 Hz。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioSampleRate) {
    /**
     * 
     * @brief 默认设置。48000Hz。
     */
    
    ByteRTCAudioSampleRateAuto = -1,
    /**
     * 
     * @brief 8000Hz
     */
    
    ByteRTCAudioSampleRate8000 = 8000,
    /**
     * 
     * @brief 11025Hz
     */
    
    ByteRTCAudioSampleRate11025 = 11025,
    /**
     * 
     * @brief 16000Hz
     */
    
    ByteRTCAudioSampleRate16000 = 16000,
    /**
     * 
     * @brief 22050Hz
     */
    
    ByteRTCAudioSampleRate22050 = 22050,
    /**
     * 
     * @brief 24000Hz
     */
    
    ByteRTCAudioSampleRate24000 = 24000,
    /**
     * 
     * @brief 32000Hz
     */
    
    ByteRTCAudioSampleRate32000 = 32000,
    /**
     * 
     * @brief 44100Hz
     */
    
    ByteRTCAudioSampleRate44100 = 44100,
    /**
     * 
     * @brief 48000Hz
     */
    
    ByteRTCAudioSampleRate48000 = 48000
};
/**
 * 
 * @type keytype
 * @brief 音频声道。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioChannel) {
    /**
     * 
     * @brief 默认设置。默认值为 `2`。
     */
    
    ByteRTCAudioChannelAuto = -1,
    /**
     * 
     * @brief 单声道
     */
    
    ByteRTCAudioChannelMono = 1,
    /**
     * 
     * @brief 双声道
     */
    
    ByteRTCAudioChannelStereo = 2,
};
/**
 * 
 * @type keytype
 * @brief 音频编码类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioCodecType) {
    /**
     * 
     * @brief 未知编码类型
     */
    
    ByteRTCAudioCodecTypeNone = 0,
    /**
     * 
     * @brief Opus 编码类型
     */
    
    ByteRTCAudioCodecTypeOpus,
    /**
     * 
     * @hidden currently not available
     */
    
    ByteRTCAudioCodecTypeAAC,
    /**
     * 
     * @hidden currently not available
     */
    
    ByteRTCAudioCodecTypeAACLC = 2,
    /**
     * 
     * @hidden currently not available
     */
    
    ByteRTCAudioCodecTypeAACHEv1,
    /**
     * 
     * @hidden currently not available
     */
    
    ByteRTCAudioCodecTypeAACHEv2,
    /**
     * 
     * @hidden currently not available
     */
    
    ByteRTCAudioCodecTypeAACLCadts,
};

/**
 * 
 * @deprecated since 3.52, use ByteRTCMixedStreamAudioCodecType{@link #ByteRTCMixedStreamAudioCodecType} instead.
 * @type keytype
 * @brief 音频编码类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCTranscodingAudioCodec) {
    /**
     * 
     * @type keytype
     * @brief AAC 格式。
     */
    
    ByteRTCTranscodingAudioCodecAAC = 0,
};

/**
 * 
 * @type keytype
 * @brief 音频编码类型。(新)
 */

typedef NS_ENUM(NSInteger, ByteRTCMixedStreamAudioCodecType) {
    /**
     * 
     * @type keytype
     * @brief AAC 格式。
     */
    
    ByteRTCMixedStreamAudioCodecTypeAAC = 0,
};

/**
 * 
 * @type keytype
 * @brief AAC 编码规格。
 */

typedef NS_ENUM(NSInteger, ByteRTCMixedStreamAudioProfile) {
    /**
     * 
     * @brief AAC-LC 规格，默认值。
     */
    
    ByteRTCMixedStreamAudioProfileLC   = 0,
    /**
     * 
     * @brief HE-AAC v1 规格。
     */
    
    ByteRTCMixedStreamAudioProfileHEv1 = 1,
    /**
     * 
     * @brief HE-AAC v2 规格。
     */
    
    ByteRTCMixedStreamAudioProfileHEv2 = 2,
};

/**
 * 
 * @deprecated since 3.52, use ByteRTCMixedStreamAudioProfile{@link #ByteRTCMixedStreamAudioProfile} instead.
 * @type keytype
 * @brief AAC 编码规格。
 */

typedef NS_ENUM(NSInteger, ByteRTCAACProfile) {
    /**
     * 
     * @brief AAC-LC 规格，默认值。
     */
    
    ByteRTCAACProfileLC   = 0,
    /**
     * 
     * @brief HE-AAC v1 规格。
     */
    
    ByteRTCAACProfileHEv1 = 1,
    /**
     * 
     * @brief HE-AAC v2 规格。
     */
    
    ByteRTCAACProfileHEv2 = 2,
};
/**
 * 
 * @type keytype
 * @brief 音频播放路由
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioRoute) {
    /**
     * 
     * @brief 通过 `setDefaultAudioRoute:` 设置的音频路由
     */
    
    ByteRTCAudioRouteDefault = -1,
    /**
     * 
     * @brief 有线耳机
     */
    
    ByteRTCAudioRouteHeadset = 1,
    /**
     * 
     * @brief 听筒。设备自带的，一般用于通话的播放硬件。
     */
    
    ByteRTCAudioRouteEarpiece = 2,
    /**
     * 
     * @brief 扬声器。设备自带的，一般用于免提播放的硬件。
     */
    
    ByteRTCAudioRouteSpeakerphone = 3,
    /**
     * 
     * @brief 蓝牙耳机
     */
    
    ByteRTCAudioRouteHeadsetBluetooth = 4,
    /**
     * 
     * hidden(macOS)
     * @brief USB 设备或通过 Lightning 转接器连接的 3.5mm 耳机 <br>
     * 其中，仅在客户端为 iOS 16 及以上版本时可识别出 3.5mm 耳机
     */
    
    ByteRTCAudioRouteHeadsetUSB = 5,
};

/**
 * 
 * @type keytype
 * @brief 音频播放设备 <br>
 *        音频设备变化时 SDK 通过 `rtcEngine:onAudioPlaybackDeviceChanged:` 回调当前音频播放设备。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioPlaybackDevice) {
    /**
     * 
     * @brief 有线耳机
     */
    
    ByteRTCAudioPlaybackDeviceHeadset = 1,
    /**
     * 
     * @brief 听筒
     */
    
    ByteRTCAudioPlaybackDeviceEarpiece = 2,
    /**
     * 
     * @brief 扬声器
     */
    
    ByteRTCAudioPlaybackDeviceSpeakerphone = 3,
    /**
     * 
     * @brief 蓝牙耳机
     */
    
    ByteRTCAudioPlaybackDeviceHeadsetBluetooth = 4,
    /**
     * 
     * @brief USB 设备
     */
    
    ByteRTCAudioPlaybackDeviceHeadsetUSB = 5,
};


/**
 * 
 * @type keytype
 * @brief 混音播放类型
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioMixingType) {
    /**
     * 
     * @brief 仅本地播放
     */
    
    ByteRTCAudioMixingTypePlayout = 0,
    /**
     * 
     * @brief 仅远端播放
     */
    
    ByteRTCAudioMixingTypePublish = 1,
    /**
     * 
     * @brief 本地和远端同时播放
     */
    
    ByteRTCAudioMixingTypePlayoutAndPublish = 2
};

/**
 * 
 * @type keytype
 * @brief 混音播放声道类型
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioMixingDualMonoMode) {
    /**
     * 
     * @brief 和音频文件一致
     */
    
    ByteRTCAudioMixingDualMonoModeAuto = 0,
    /**
     * 
     * @brief 只能听到音频文件中左声道的音频
     */
    
    ByteRTCAudioMixingDualMonoModeL = 1,
    /**
     * 
     * @brief 只能听到音频文件中右声道的音频
     */
    
    ByteRTCAudioMixingDualMonoModeR = 2,
    /**
     * 
     * @brief 能同时听到音频文件中左右声道的音频
     */
    
    ByteRTCAudioMixingDualMonoModeMix = 3
};

/**
 * 
 * @type keytype
 * @brief 音频混音文件播放状态。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioMixingState) {
    /**
     * 
     * @brief 混音已加载
     */
    
    ByteRTCAudioMixingStatePreloaded = 0,
    /**
     * 
     * @brief 混音正在播放
     */
    
    ByteRTCAudioMixingStatePlaying,
    /**
     * 
     * @brief 混音暂停
     */
    
    ByteRTCAudioMixingStatePaused,
    /**
     * 
     * @brief 混音停止
     */
    
    ByteRTCAudioMixingStateStopped,
    /**
     * 
     * @brief 混音播放失败
     */
    
    ByteRTCAudioMixingStateFailed,
    /**
     * 
     * @brief 混音播放结束
     */
    
    ByteRTCAudioMixingStateFinished,
    /**
     * 
     * @brief 准备 PCM 混音
     */
    
    ByteRTCAudioMixingStatePCMEnabled,
    /**
     * 
     * @brief PCM 混音播放结束
     */
    
    ByteRTCAudioMixingStatePCMDisabled,
};
/**
 * 
 * @type keytype
 * @brief 混音错误码。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioMixingError) {
    /**
     * 
     * @brief 正常
     */
    
    ByteRTCAudioMixingErrorOk = 0,
    /**
     * 
     * @brief 预加载失败，找不到混音文件或者文件长度超出 20s
     */
    
    ByteRTCAudioMixingErrorPreloadFailed,
    /**
     * 
     * @brief 混音开启失败，找不到混音文件或者混音文件打开失败
     */
    
    ByteRTCAudioMixingErrorStartFailed,
    /**
     * 
     * @brief 混音 ID 异常
     */
    
    ByteRTCAudioMixingErrorIdNotFound,
    /**
     * 
     * @brief 设置混音文件的播放位置出错
     */
    
    ByteRTCAudioMixingErrorSetPositionFailed,
    /**
     * 
     * @brief 音量参数不合法，仅支持设置的音量值为[0, 400]
     */
    
    ByteRTCAudioMixingErrorInValidVolume,
    /**
     * 
     * @brief 播放的文件与预加载的文件不一致。请先使用 unloadAudioMixing: 卸载此前的文件。
     */
    
    ByteRTCAudioMixingErrorLoadConflict,
    /**
     * 
     * @brief 不支持此混音类型。
     */
    
    ByteRTCAudioMixingErrorIdTypeNotMatch,
    /**
     * 
     * @brief 设置混音文件的音调不合法
     */
    
    ByteRTCAudioMixingErrorInValidPitch,
    /**
     * 
     * @brief 设置混音文件的音轨不合法
     */
    
    ByteRTCAudioMixingErrorInValidAudioTrack,
    /**
     * 
     * @brief 混音文件正在启动中
     */
    
    ByteRTCAudioMixingErrorIsStarting,
    /**
     * 
     * @brief 设置混音文件的播放速度不合法
     */
    
    ByteRTCAudioMixingErrorInValidPlaybackSpeed,
};
/**
 * 
 * @type keytype
 * @brief 自定义音频源模式
 */

typedef NS_ENUM(NSInteger, ByteRTCMediaPlayerCustomSourceMode) {
    /**
     * 
     * @brief 当播放来自本地的 PCM 数据时，使用此选项。
     */
    
    ByteRTCMediaPlayerCustomSourceModePush = 0,
    /**
     * 
     * @brief 当播放来自内存的音频数据时，使用此选项。
     */
    
    ByteRTCMediaPlayerCustomSourceModePull,
};
/**
 * 
 * @type keytype
 * @brief 自定义音频流类型
 */

typedef NS_ENUM(NSInteger, ByteRTCMediaPlayerCustomSourceStreamType) {
    /**
     * 
     * @brief 当播放来自本地的 PCM 数据时，使用此选项。
     */
    
    ByteRTCMediaPlayerCustomSourceStreamTypeRaw = 0,
    /**
     * 
     * @brief 当播放来自内存的音频数据时，使用此选项。
     */
    
    ByteRTCMediaPlayerCustomSourceStreamTypeEncoded,
};
/**
 * 
 * @type keytype
 * @brief 音频数据的起始读取位置。
 */

typedef NS_ENUM(NSInteger, ByteRTCMediaPlayerCustomSourceSeekWhence) {
    /**
     * 
     * @brief 从音频数据的头开始读取，读取后的位置为参数 offset 的值。
     */
    
    ByteRTCMediaPlayerCustomSourceSeekWhenceSet = 0,
    /**
     * 
     * @brief 从音频数据的某一位置开始读取，读取后的位置为音频数据当前的读取位置加上参数 offset 的值。
     */
    
    ByteRTCMediaPlayerCustomSourceSeekWhenceCur = 1,
    /**
     * 
     * @brief 从音频数据的尾开始读取，读取后的位置为用户传入的音频数据大小加上参数 offset 的值。
     */
    
    ByteRTCMediaPlayerCustomSourceSeekWhenceEnd = 2,
    /**
     * 
     * @brief 返回音频数据的大小。
     */
    
    ByteRTCMediaPlayerCustomSourceSeekWhenceSize = 3,
};
/**
 * 
 * @type keytype
 * @brief 播放状态。
 */

typedef NS_ENUM(NSInteger, ByteRTCPlayerState) {
    /**
     * 
     * @brief 播放未启动
     */
    
    ByteRTCPlayerStateIdle = 0,
    /**
     * 
     * @brief 已加载
     */
    
    ByteRTCPlayerStatePreloaded,
    /**
     * 
     * @brief 已打开
     */
    
    ByteRTCPlayerStateOpened,
    /**
     * 
     * @brief 正在播放
     */
    
    ByteRTCPlayerStatePlaying,
    /**
     * 
     * @brief 播放已暂停
     */
    
    ByteRTCPlayerStatePaused,
    /**
     * 
     * @brief 播放已被主动停止
     */
    
    ByteRTCPlayerStateStopped,
    /**
     * 
     * @brief 播放失败
     */
    
    ByteRTCPlayerStateFailed,
    /**
     * 
     * @brief 播放自然结束
     */
    
    ByteRTCPlayerStateFinished,
    /** 
     * 
     * @brief 循环播放已结束
     */
    
    ByteRTCPlayerStateLoopFinished,
};

/**
 * 
 * @type keytype
 * @brief 播放事件
 */

typedef NS_ENUM(NSInteger, ByteRTCPlayerEvent) {

    /** 
    * 
    * @brief 开始切换音轨 <br>
    *        开始调用 selectAudioTrack:{@link #ByteRTCMediaPlayer#selectAudioTrack:} 时，返回此状态。
    */
    
    ByteRTCPlayerEventSelectAudioTrackBegin,
    /** 
    * 
    * @brief 切换音轨成功 <br>
    *        调用 selectAudioTrack:{@link #ByteRTCMediaPlayer#selectAudioTrack:} 成功后，播放器切换到指定音轨播放，返回此状态。
    */
    
    ByteRTCPlayerEventSelectAudioTrackCompleted,
    /** 
    * 
    * @brief 切换音轨失败 <br>
    *        调用 selectAudioTrack:{@link #ByteRTCMediaPlayer#selectAudioTrack:} 失败后，播放器无法切换到指定音轨，继续之前的音轨播放过程，返回此状态。
    */
    
    ByteRTCPlayerEventSelectAudioTrackFailed,
    /**
    * 
    * @brief 试图移动播放位置 <br>
    *        开始调用 setPosition:{@link #ByteRTCMediaPlayer#setPosition:} 时，返回此状态。
    */
    
    ByteRTCPlayerEventSeekBegin,
    /**
    * 
    * @brief 移动播放位置成功 <br>
    *        调用 setPosition:{@link #ByteRTCMediaPlayer#setPosition:} 成功后，返回此状态。
    */
    
    ByteRTCPlayerEventSeekCompleted,
    /**
    * 
    * @brief 移动播放位置失败 <br>
    *        调用 setPosition:{@link #ByteRTCMediaPlayer#setPosition:} 失败时，返回此状态。
    */
    
    ByteRTCPlayerEventSeekFailed,
};

/**
 * 
 * @type keytype
 * @brief 播放错误码。
 */

typedef NS_ENUM(NSInteger, ByteRTCPlayerError) {
    /**
     * 
     * @brief 正常
     */
    
    ByteRTCPlayerErrorOK = 0,
    /**
     * 
     * @brief 不支持此类型
     */
    
    ByteRTCPlayerErrorFormatNotSupport,
    /**
     * 
     * @brief 无效的播放路径
     */
    
    ByteRTCPlayerErrorInvalidPath,
    /**
     * 
     * @brief 未满足前序接口调用的要求。请查看具体接口文档。
     */
    
    ByteRTCPlayerErrorInvalidState,
    /**
     * 
     * @brief 设置播放位置出错。
     */
    
    ByteRTCPlayerErrorInvalidPosition,
    /**
     * 
     * @brief 音量参数不合法。
     */
    
    ByteRTCPlayerErrorInvalidVolume,
    /**
     * 
     * @brief 音调参数设置不合法。
     */
    
    ByteRTCPlayerErrorInvalidPitch,
    /**
     * 
     * @brief 音轨参数设置不合法。
     */
    
    ByteRTCPlayerErrorInvalidAudioTrackIndex,
    /**
     * 
     * @brief 播放速度参数设置不合法
     */
    
    ByteRTCPlayerErrorInvalidPlaybackSpeed,
    /**
     * 
     * @brief 音效 ID 异常。还未加载或播放文件，就调用其他 API。
     */
    
    ByteRTCPlayerErrorInvalidEffectId,
    /**
    * 
    * @brief 资源正在播放中
    */
    
    ByteRTCPlayerErrorCurrentlyPlaying,
};

/**
 * 
 * @type keytype
 * @brief 音频输入源类型
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioSourceType) {
    /**
     * 
     * @brief 自定义采集音频源
     */
    
    ByteRTCAudioSourceTypeExternal = 0,
    /**
     * 
     * @brief RTC SDK 内部采集音频源
     */
    
    ByteRTCAudioSourceTypeInternal,
};

/**
 * 
 * @type keytype
 * @brief 音频输出源类型
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioRenderType) {
    /**
     * 
     * @brief 自定义渲染音频
     */
    
    ByteRTCAudioRenderTypeExternal = 0,
    /**
     * 
     * @brief RTC SDK 内部渲染音频
     */
    
    ByteRTCAudioRenderTypeInternal,
};

/**
 * 
 * @type keytype
 * @brief 是否开启耳返功能。
 */

typedef NS_ENUM(NSInteger, ByteRTCEarMonitorMode) {
    /**
     * 
     * @brief 关闭。
     */
    
    ByteRTCEarMonitorModeOff = 0,
    /**
     * 
     * @brief 开启。
     */
    
    ByteRTCEarMonitorModeOn = 1,
};

/**
 * 
 * @type keytype
 * @brief 耳返音频是否经过本地音频处理。
 */

typedef NS_ENUM(NSInteger, ByteRTCEarMonitorAudioFilter) {
    /**
     * 
     * @brief 无音频处理。
     */
    
    ByteRTCEarMonitorAudioFilterNone = 0x0001,
    /**
     * 
     * @brief 经本地音频处理后的音频，包括 3A、变声、混响等 SDK 提供处理能力以及自定义音频处理。
     */
    
    ByteRTCEarMonitorAudioFilterReuseAudioProcessing = 0x8000,
};

/**
 * 
 * @type keytype
 * @brief 音频回调方法
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioFrameCallbackMethod) {
    /**
     * 
     * @brief 本地麦克风录制的音频数据回调
     */
    
    ByteRTCAudioFrameCallbackRecord = 0,
    /**
     * 
     * @brief 订阅的远端所有用户混音后的音频数据回调
     */
    
    ByteRTCAudioFrameCallbackPlayback = 1,
    /**
     * 
     * @brief 本地麦克风录制和订阅的远端所有用户混音后的音频数据回调
     */
    
    ByteRTCAudioFrameCallbackMixed = 2,
    /**
     * 
     * @brief 订阅的远端每个用户混音前的音频数据回调
     */
    
    ByteRTCAudioFrameCallbackRemoteUser = 3,
    /**
     * 
     * @brief 本地麦克风录制和本地 `MediaPlayer`, `EffectPlayer` 播放的音频混音后的音频数据回调
     */
    
    ByteRTCAudioFrameCallbackCaptureMixed = 5
};
/**
 * 
 * @type keytype
 * @brief 返回给音频处理器的音频类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioFrameMethod) {
    /**
     * 
     * @brief 本地采集的音频。
     */
    
    ByteRTCAudioFrameProcessorRecord = 0,
    /**
     * 
     * @brief 远端音频流的混音音频。
     */
    
    ByteRTCAudioFrameProcessorPlayback = 1,
    /**
     * 
     * @brief 各个远端音频流。
     */
    
    ByteRTCAudioFrameProcessorRemoteUser = 2,
    /**
     * 
     * @hidden(macOS)
     * @brief 软件耳返音频。
     */
    
    ByteRTCAudioFrameProcessorEarMonitor = 3,
     /**
     * 
     * @brief 屏幕共享音频。
     */
    
    ByteRTCAudioFrameProcessorScreen = 4,
};

/**
 * 
 * @type keytype
 * @brief 音量回调模式。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioReportMode) {
     /**
     * 
     * @brief 默认始终开启音量回调。
     */
    
    ByteRTCAudioReportModeNormal = 0,
   /**
     * 
     * @brief 可见用户进房并停止推流后，关闭音量回调。
     */
    
    ByteRTCAudioReportModeDisconnect = 1,
     /**
     * 
     * @brief 可见用户进房并停止推流后，开启音量回调，回调值重置为 0。
     */
    
    ByteRTCAudioReportModeReset = 2,
};

/**
 * 
 * @type keytype
 * @brief rtcEngine:onLocalAudioPropertiesReport:{@link #ByteRTCEngineDelegate#rtcEngine:onLocalAudioPropertiesReport:} 中包含的音频信息的范围。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioPropertiesMode) {
    /**
     * 
     * @brief 仅包含本地麦克风采集的音频数据和本地屏幕音频采集数据。
     */
    
    ByteRTCAudioPropertiesModeMicrohone = 0,
    /**
     * 
     * @brief 包含以下信息： <br>
     *        - 本地麦克风采集的音频数据和本地屏幕音频采集数据；
     *        - 本地混音的音频数据。
     */
    
    ByteRTCAudioPropertiesModeAudioMixing = 1
};

/**
 * 
 * @type keytype
 * @brief 音频流来源的用户 ID, 及对应的音量。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioVolumeInfo : NSObject
/**
 * 
 * @brief 音频流来源的用户 ID
 */

@property(copy, nonatomic) NSString * _Nonnull uid;
/**
 * 
 * @brief 线性音量，取值范围为：[0,255]
 */

@property(assign, nonatomic) NSUInteger linearVolume;
/**
 * 
 * @brief 非线性音量，取值范围为：[-127,0]
 */

@property(assign, nonatomic) NSUInteger nonlinearVolume;
@end

/**
 * 
 * @type keytype
 * @brief 音频参数格式
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioFormat : NSObject
/**
 * 
 * @brief 音频采样率，详见 ByteRTCAudioSampleRate{@link #ByteRTCAudioSampleRate}
 */

@property(nonatomic, assign) ByteRTCAudioSampleRate sampleRate;
/**
 * 
 * @brief 音频声道，详见 ByteRTCAudioChannel{@link #ByteRTCAudioChannel}
 */

@property(nonatomic, assign) ByteRTCAudioChannel channel;
/**
 * 
 * @brief 单次回调的音频帧中包含的采样点数。默认值为 `0`，此时，采样点数取最小值。 <br>
 *        最小值为回调间隔是 0.01s 时的值，即 `sampleRate * channel * 0.01s`。 <br>
 *        最大值是 `2048`。超出取值范围时，采样点数取默认值。 <br>
 *        该参数仅在设置读写回调时生效，调用 enableAudioFrameCallback:format:{@link #ByteRTCEngine#enableAudioFrameCallback:format:} 开启只读模式回调时设置该参数不生效。
 */

@property(nonatomic, assign) int samplesPerCall;
@end

/**
 * 
 * @type keytype
 * @brief PCM 音频帧
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioFrame : NSObject
/**
 * 
 * @brief PCM 数据
 */

@property(strong, nonatomic) NSData * _Nonnull buffer;
/**
 * 
 * @brief 采样点个数
 */

@property(assign, nonatomic) int samples;
/**
 * 
 * @brief 音频声道，参看 ByteRTCAudioChannel{@link #ByteRTCAudioChannel}。 <br>
 *        双声道的情况下，左右声道的音频帧数据以 LRLRLR 形式排布。
 */

@property(assign, nonatomic) ByteRTCAudioChannel channel;
/**
 * 
 * @brief 采样率，参看 ByteRTCAudioSampleRate{@link #ByteRTCAudioSampleRate}。
 */

@property(assign, nonatomic) ByteRTCAudioSampleRate sampleRate;
/**
 * 
 * @hidden for internal use only
 * @brief 额外信息数据
 */

@property(strong, nonatomic) NSData * extraInfo;
@end
/**
 * 
 * @type callback
 * @brief 音频数据回调观察者 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。 <br>
 * 本接口类中的回调周期均为 20 ms。
 * @list 
 */

@protocol ByteRTCAudioFrameObserver <NSObject>
/**
 * 
 * @type callback
 * @brief 返回麦克风录制的音频数据
 * @param audioFrame 音频数据, 详见： ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @list 自定义流处理
 */

- (void)onRecordAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 返回订阅的所有远端用户混音后的音频数据。
 * @param audioFrame 音频数据, 详见： ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @list 自定义流处理
 */

- (void)onPlaybackAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 返回远端单个用户的音频数据
 * @param streamId 远端流对应的唯一标识
 * @param info 远端流详细信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}
 * @param audioFrame 音频数据，参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @note 此回调在播放线程调用。不要在此回调中做任何耗时的事情，否则可能会影响整个音频播放链路。
 * @list 自定义流处理
 */

- (void)onRemoteUserAudioFrame:(NSString * _Nonnull)streamId
                          info:(ByteRTCStreamInfo * _Nonnull)info
                    audioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 返回本地麦克风录制和订阅的所有远端用户混音后的音频数据
 * @param audioFrame 音频数据, 详见： ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @list 自定义流处理
 */

- (void)onMixedAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 返回本地麦克风录制的音频数据，本地 `MediaPlayer` / `EffectPlayer` 播放音频文件混音后的音频数据
 * @param audioFrame 音频数据, 详见： ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @list 自定义流处理
 */

- (void)onCaptureMixedAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
@end
/**
 * 
 * @type callback
 * @brief 自定义音频处理器 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list Custom Stream Processing
 */

@protocol ByteRTCAudioFrameProcessor <NSObject>
/**
 * 
 * @type callback
 * @brief 回调本地采集的音频帧地址，供自定义音频处理。
 * @param audioFrame 音频帧地址，参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}。
 * @return
 *        - 0： 成功。
 *        - < 0： 失败。
 * @note
 *        - 完成自定义音频处理后，SDK 会对处理后的音频帧进行编码，并传输到远端。此音频处理不会影响软件耳返音频数据。
 *        - 要启用此回调，必须调用 enableAudioProcessor:audioFormat:{@link #ByteRTCEngine#enableAudioProcessor:audioFormat:}，并在参数中选择本地采集的音频，每 10 ms 收到此回调。
 * @list 自定义流处理
 */

- (int)onProcessRecordAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 回调远端音频混音的音频帧地址，供自定义音频处理。
 * @param audioFrame 音频帧地址，参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @note 调用 enableAudioProcessor:audioFormat:{@link #ByteRTCEngine#enableAudioProcessor:audioFormat:},并在参数中选择远端音频流的的混音音频时，每 10 ms 收到此回调。
 * @list 自定义流处理
 */

- (int)onProcessPlayBackAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 回调单个远端用户的音频帧地址，供自定义音频处理。
 * @param streamId 音频流对应的唯一标识
 * @param info 音频流详细信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}
 * @param audioFrame 音频帧地址，参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}
 * @note 调用 enableAudioProcessor:audioFormat:{@link #ByteRTCEngine#enableAudioProcessor:audioFormat:},并在参数中选择各个远端音频流时，每 10 ms 收到此回调。
 * @list 自定义流处理
 */

- (int)onProcessRemoteUserAudioFrame:(NSString * _Nonnull)streamId info:(ByteRTCStreamInfo * _Nonnull)info audioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @hidden(macOS)
 * @valid since 3.50
 * @type callback
 * @brief 软件耳返音频数据的回调。你可根据此回调自定义处理音频。 <br>
 *        软件耳返音频中包含通过调用 `setVoiceReverbType:` 和 `setVoiceChangerType:` 设置的音频特效。
 * @param audioFrame 音频帧地址。参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}。
 * @return
 *        - 0： 成功。
 *        - < 0： 失败。
 * @note
 *        - 此数据处理只影响软件耳返音频数据。
 *        - 要启用此回调，必须调用 enableAudioProcessor:audioFormat:{@link #ByteRTCEngine#enableAudioProcessor:audioFormat:}，并选择耳返音频。每 10 ms 收到此回调。
 * @list 自定义流处理
 */

- (int)onProcessEarMonitorAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
/**
 * 
 * @type callback
 * @brief 回调屏幕共享的音频帧地址，供自定义音频处理。
 * @param audioFrame 音频帧地址，参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}。
 * @note 调用 `enableAudioProcessor:`，把返回给音频处理器的音频类型设置为屏幕共享音频后，每 10 ms 收到此回调。
 * @list 自定义流处理
 */

- (int)onProcessScreenAudioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;
@end

/**
 * 
 * @type keytype
 * @hidden for internal use only
 * @region 音频管理
 * @brief 音频帧构建类
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCEncodedAudioFrameData : NSObject
/**
 * 
 * @hidden for internal use only
 * @brief 音频编码类型，参看 ByteRTCAudioCodecType{@link #ByteRTCAudioCodecType}。
 */

@property(assign, nonatomic) ByteRTCAudioCodecType codecType;
/**
 * 
 * @hidden for internal use only
 * @brief 数据
 */

@property(strong, nonatomic) NSData * _Nonnull buffer;
/**
 * 
 * @hidden for internal use only
 * @brief 音频声道，参看 ByteRTCAudioChannel{@link #ByteRTCAudioChannel}。 <br>
 *        双声道的情况下，左右声道的音频帧数据以 LRLRLR 形式排布。
 */

@property(assign, nonatomic) ByteRTCAudioChannel channel;
/**
 * 
 * @hidden for internal use only
 * @brief 采样率，参看 ByteRTCAudioSampleRate{@link #ByteRTCAudioSampleRate}。
 */

@property(assign, nonatomic) ByteRTCAudioSampleRate sampleRate;
/**
 * 
 * @hidden for internal use only
 * @brief 数据大小
 */

@property(assign, nonatomic) int size;
/**
 * 
 * @hidden for internal use only
 * @brief 时间戳，单位为微秒。
 */

@property(assign, nonatomic) long timestampUs;
/**
 * 
 * @hidden for internal use only
 * @brief 音频帧时长，单位为毫秒。
 */

@property(assign, nonatomic) int frameSizeMs;
/**
 * 
 * @hidden for internal use only
 * @brief 额外信息数据
 */

@property(strong, nonatomic) NSData * extraInfo;
@end
/**
 * 
 * @type callback
 * @hidden for internal use only
 * @brief 远端音频帧监测器
 * @list 自定义流处理
 */

@protocol ByteRTCRemoteEncodedAudioFrameObserver <NSObject>
/**
 * 
 * @type callback
 * @hidden for internal use only
 * @brief 调用 registerRemoteEncodedAudioFrameObserver:{@link #ByteRTCEngine#registerRemoteEncodedAudioFrameObserver:} 后，SDK 收到远端音频帧信息时，回调该事件
 * @param streamId 收到的远端音频流对应的唯一标识
 * @param info 收到的远端音频流的详细信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}
 * @param audioFrame 远端音频帧信息，参看 ByteRTCEncodedAudioFrameData{@link #ByteRTCEncodedAudioFrameData}
 * @list 自定义流处理
 */

- (void)onRemoteEncodedAudioFrame:(NSString * _Nonnull)streamId info:(ByteRTCStreamInfo * _Nonnull)info audioFrame:(ByteRTCEncodedAudioFrameData * _Nonnull)audioFrame;
@end



#pragma mark - AudioDeviceManager
/**
 * 
 * @hidden(iOS)
 * @type api
 * @brief 音频设备管理类
 * @list Custom Stream Processing
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioDeviceManager : NSObject
/**
 * 
 * @type api
 * @brief 获取当前系统内音频播放设备列表。
 * @return 所有音频播放设备的列表，参看 ByteRTCDeviceCollection{@link #ByteRTCDeviceCollection}。 <br>
 * 等待超时后会返回空列表。超时时间默认为 10 s。建议通过 rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:} 监听到 `ByteRTCMediaDeviceListUpdated` 后，再次调用本接口获取。
 * @note 你可以在收到 rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:} 了解设备变更后，重新调用本接口以获得新的设备列表。 <br>
 * @list 音频管理
 */

- (ByteRTCDeviceCollection * _Nonnull)enumerateAudioPlaybackDevices;
/**
 * 
 * @type api
 * @brief 获取音频采集设备列表。
 * @return 音频采集设备列表。详见 ByteRTCDeviceCollection{@link #ByteRTCDeviceCollection}。 <br>
 * 等待超时后会返回空列表。超时时间默认为 10 s。建议通过 rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:} 监听到 `ByteRTCMediaDeviceListUpdated` 后，再次调用本接口获取。
 * @note 你可以在收到 rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:} 了解设备变更后，重新调用本接口以获得新的设备列表。
 * @list 音频管理
 */

- (ByteRTCDeviceCollection * _Nonnull)enumerateAudioCaptureDevices;
/**
 * 
 * @type api
 * @brief 设置音频采集路由是否跟随系统。
 * @param followed <br>
 *        - true: 跟随。此时，调用 setAudioCaptureDevice:{@link #ByteRTCAudioDeviceManager#setAudioCaptureDevice:} 会失败。默认值。
 *        - false: 不跟随系统。此时，可以调用 setAudioCaptureDevice:{@link #ByteRTCAudioDeviceManager#setAudioCaptureDevice:} 进行设置。
 * @list 音频管理
 */

- (void)followSystemCaptureDevice:(BOOL)followed;
/**
 * 
 * @type api
 * @brief 设置音频播放路由是否跟随系统。
 * @param followed <br>
 *        - true: 跟随。此时，调用 setAudioPlaybackDevice:{@link #ByteRTCAudioDeviceManager#setAudioPlaybackDevice:} 会失败。默认值。
 *        - false: 不跟随系统。此时，可以调用 setAudioPlaybackDevice:{@link #ByteRTCAudioDeviceManager#setAudioPlaybackDevice:} 进行设置。
 * @list 音频管理
 */

- (void)followSystemPlaybackDevice:(BOOL)followed;
/**
 * 
 * @type api
 * @brief 设置音频播放设备。
 * @param deviceID 音频播放设备 ID，可通过 enumerateAudioPlaybackDevices{@link #ByteRTCAudioDeviceManager#enumerateAudioPlaybackDevices} 获取。
 * @return
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note 当你调用 followSystemPlaybackDevice:{@link #ByteRTCAudioDeviceManager#followSystemPlaybackDevice:} 设置音频播放设备跟随系统后，将无法调用此接口设置音频播放设备。
 * @list 音频管理
 */

- (int)setAudioPlaybackDevice:(NSString * _Nonnull)deviceID;
/**
 * 
 * @type api
 * @brief 获取当前音频播放设备。
 * @param deviceID 设备 ID
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int)getAudioPlaybackDevice:(NSString * _Nonnull * _Nonnull) deviceID;
/**
 * 
 * @type api
 * @brief 设置音频采集设备。
 * @param deviceID 音频采集设备 ID。你可调用 enumerateAudioCaptureDevices{@link #ByteRTCAudioDeviceManager#EnumerateAudioCaptureDevices} 获取可用设备列表。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note 当你调用 followSystemCaptureDevice:{@link #ByteRTCAudioDeviceManager#followSystemCaptureDevice:} 设置音频采集设备跟随系统后，将无法调用此接口设置音频采集设备。
 * @list 音频管理
 */

- (int)setAudioCaptureDevice:(NSString * _Nonnull)deviceID;
/**
 * 
 * @type api
 * @brief 获取当前音频采集设备。
 * @param deviceID 音频采集设备 ID。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int)getAudioCaptureDevice:(NSString *_Nonnull * _Nonnull) deviceID;
/**
 * 
 * @type api
 * @brief 设置当前音频采集设备静音状态，默认为非静音。
 * @param mute <br>
 *       - true：静音
 *       - false：非静音
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *      - 该方法用于静音整个系统的音频采集。你也可以仅对麦克风采集到的音频信号做静音处理，而不影响媒体播放器的音乐声音，具体参看 muteAudioCapture:{@link #ByteRTCEngine#muteAudioCapture:} 方法说明。
 *      - 设该方法为 `true` 静音后仍可通过 setAudioCaptureDeviceVolume:{@link #ByteRTCAudioDeviceManager#setAudioCaptureDeviceVolume:} 调整采集音量，调整后的音量会在取消静音后生效。
 * @list 音频管理
 */

- (int) setAudioCaptureDeviceMute:(bool)mute;
/**
 * 
 * @type api
 * @brief 获取当前音频采集设备是否静音的信息。
 * @param mute <br>
 *       - true：静音
 *       - false：非静音
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int) getAudioCaptureDeviceMute:(bool * _Nonnull)mute;
/**
 * 
 * @type api
 * @brief 设置当前音频播放设备静音状态，默认为非静音。
 * @param mute <br>
 *       - true：静音
 *       - false：非静音
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int) setAudioPlaybackDeviceMute:(bool)mute;
/**
 * 
 * @type api
 * @brief 获取当前音频播放设备是否静音的信息。
 * @param mute <br>
 *       - true：静音
 *       - false：非静音
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int) getAudioPlaybackDeviceMute:(bool * _Nonnull)mute;
/**
 * 
 * @type api
 * @brief 设置当前音频采集设备音量
 * @param volume 音频采集设备音量，取值范围为 [0,255]。 <br>
 *       - [0,25] 接近无声；
 *       - [25,75] 为低音量；
 *       - [76,204] 为中音量；
 *       - [205,255] 为高音量。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功。将影响 rtcEngine:onLocalAudioPropertiesReport:{@link #ByteRTCEngineDelegate#rtcEngine:onLocalAudioPropertiesReport:} 回调的音量信息。
 *        - < 0：方法调用失败
 * @note 调用 setAudioCaptureDeviceMute:{@link #ByteRTCAudioDeviceManager#setAudioCaptureDeviceMute:} 设为 `true` 静音采集设备后的音量调节会在取消静音后生效。
 * @list 音频管理
 */

- (int) setAudioCaptureDeviceVolume:(unsigned int)volume;
/**
 * 
 * @type api
 * @brief 获取当前音频采集设备音量
 * @param volume 音频采集设备音量，取值范围是 [0,255] <br>
 *       - [0,25] 接近无声；
 *       - [25,75] 为低音量；
 *       - [76,204] 为中音量；
 *       - [205,255] 为高音量。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int) getAudioCaptureDeviceVolume:(unsigned int * _Nonnull)volume;
/**
 * 
 * @type api
 * @brief 设置当前音频播放设备音量
 * @param volume 音频播放设备音量，取值范围为 [0,255] <br>
 *       - [0,25] 接近无声；
 *       - [25,75] 为低音量；
 *       - [76,204] 为中音量；
 *       - [205,255] 为高音量。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int) setAudioPlaybackDeviceVolume:(unsigned int)volume;
/**
 * 
 * @type api
 * @brief 获取当前音频播放设备音量
 * @param volume 音频播放设备音量，取值范围是 [0,255] <br>
 *       - [0,25] 接近无声；
 *       - [25,75] 为低音量；
 *       - [76,204] 为中音量；
 *       - [205,255] 为高音量。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @list 音频管理
 */

- (int) getAudioPlaybackDeviceVolume:(unsigned int * _Nonnull)volume;
/**
 * 
 * @type api
 * @brief 启动音频播放设备测试。 <br>
 *        该方法测试播放设备是否能正常工作。SDK 播放指定的音频文件，测试者如果能听到声音，说明播放设备能正常工作。
 * @param testAudioFilePath 音频文件的绝对路径，路径字符串使用 UTF-8 编码格式，支持以下音频格式: mp3，aac，m4a，3gp，wav。
 * @param interval 音频设备播放测试音量回调的间隔
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *       - 该方法必须在进房前调用，且不可与其它音频设备测试功能同时应用。
 *       - 调用 stopAudioPlaybackDeviceTest{@link #ByteRTCAudioDeviceManager#stopAudioPlaybackDeviceTest} 停止测试。
 * @list 音频管理
 */

- (int)startAudioPlaybackDeviceTest:(NSString *_Nonnull)testAudioFilePath interval:(int)interval;
/**
 * 
 * @type api
 * @brief 停止音频播放设备测试。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note 调用 startAudioPlaybackDeviceTest:interval:{@link #ByteRTCAudioDeviceManager#startAudioPlaybackDeviceTest:interval:} 后，需调用本方法停止测试。
 * @list 音频管理
 */

- (int)stopAudioPlaybackDeviceTest;
/**
 * 
 * @hidden(iOS)
 * @type api
 * @brief 开始音频采集设备和音频播放设备测试。
 * @param interval 测试中会收到 `rtcEngine:onLocalAudioPropertiesReport:` 回调，本参数指定了该周期回调的时间间隔，单位为毫秒。建议设置到大于 200 毫秒。最小不得少于 10 毫秒。
 * @return 方法调用结果 <br>
 *       - 0：方法调用成功
 *       - < 0：方法调用失败
 * @note
 *       - 该方法在进房前后均可调用。且不可与其它音频设备测试功能同时应用。
 *       - 调用本接口 30 s 后，采集自动停止，并开始播放采集到的声音。录音播放完毕后，设备测试流程自动结束。你也可以在 30 s 内调用 stopAudioDeviceRecordAndPlayTest{@link #ByteRTCAudioDeviceManager#stopAudioDeviceRecordAndPlayTest}  来停止采集并开始播放此前采集到的声音。
 *       - 调用 stopAudioDevicePlayTest{@link #ByteRTCAudioDeviceManager#stopAudioDevicePlayTest} 可以停止音频设备采集和播放测试。
 *       - 你不应在测试过程中，调用 `enableAudioPropertiesReport:` 注册音量提示回调。
 *       - 该方法仅在本地进行音频设备测试，不涉及网络连接。
 * @list 音频管理
 */

- (int)startAudioDeviceRecordTest:(int)interval;
/**
 * 
 * @hidden(iOS)
 * @type api
 * @brief 停止采集本地音频，并开始播放采集到的声音。录音播放完毕后，设备测试流程结束。 <br>
 * 调用 startAudioDeviceRecordTest:{@link #ByteRTCAudioDeviceManager#startAudioDeviceRecordTest:} 30s 内调用本接口来停止采集并开始播放此前采集到的声音。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *        - 该方法依赖 rtc 引擎，只有通过成员方法 getAudioDeviceManager{@link #ByteRTCEngine#getAudioDeviceManager} 创建的 ByteRTCAudioDeviceManager，该方法才是有效的
 *        - 调用本接口开始播放录音后，可以在播放过程中调用 stopAudioDevicePlayTest{@link #ByteRTCAudioDeviceManager#stopAudioDevicePlayTest} 停止播放。
 * @list 音频管理
 */

- (int)stopAudioDeviceRecordAndPlayTest;
/**
 * 
 * @hidden(iOS)
 * @type api
 * @brief 停止由调用 startAudioDeviceRecordTest:{@link #ByteRTCAudioDeviceManager#startAudioDeviceRecordTest:} 开始的音频播放设备测试。 <br>
 *        在音频播放设备测试自动结束前，可调用本接口停止音频采集与播放测试。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *        - 该方法依赖 rtc 引擎，只有通过成员方法 getAudioDeviceManager{@link #ByteRTCEngine#getAudioDeviceManager} 创建的 ByteRTCAudioDeviceManager，该方法才是有效的
 * @list 音频管理
 */

- (int)stopAudioDevicePlayTest;
/**
 * 
 * @type api
 * @brief 尝试初始化音频播放设备，以检测设备不存在、权限被拒绝/禁用等异常问题。
 * @param deviceID 设备索引号
 * @return 设备状态错误码 <br>
 *        - 0: 设备检测结果正常
 *        - -1: 接口调用失败
 *        - -2: 设备无权限，尝试初始化设备失败
 *        - -3: 设备不存在，当前没有设备或设备被移除时返回
 *        - -4: 设备音频格式不支持
 *        - -5: 其它原因错误
 * @note
 *        - 该接口需在进房前调用；
 *        - 检测成功不代表设备一定可以启动成功，还可能因设备被其他应用进程独占，或 CPU/内存不足等原因导致启动失败。
 * @list 音频管理
 */

- (int)initAudioPlaybackDeviceForTest:(NSString * _Nonnull)deviceID;
/**
 * 
 * @type api
 * @brief 尝试初始化音频采集设备，以检测设备不存在、权限被拒绝/禁用等异常问题。
 * @param deviceID 设备索引号
 * @return 设备状态错误码 <br>
 *        - 0: 设备检测结果正常
 *        - -1: 接口调用失败
 *        - -2: 设备无权限，尝试初始化设备失败
 *        - -3: 设备不存在，当前没有设备或设备被移除时返回
 *        - -4: 设备音频格式不支持
 *        - -5: 其它原因错误
 * @note
 *        - 该接口需在进房前调用；
 *        - 检测成功不代表设备一定可以启动成功，还可能因设备被其他应用进程独占，或 CPU/内存不足等原因导致启动失败。
 * @list 音频管理
 */

- (int)initAudioCaptureDeviceForTest:(NSString * _Nonnull)deviceID;
/**
 * 
 * @type api
 * @brief 启动音频采集设备测试。 <br>
 *        该方法测试音频采集设备是否能正常工作。启动测试后，会收到 rtcEngine:onLocalAudioPropertiesReport:{@link #ByteRTCEngineDelegate#rtcEngine:onLocalAudioPropertiesReport:} 回调上报的音量信息。
 * @param indicationInterval 获取回调的时间间隔，单位为毫秒。建议设置到大于 200 毫秒。最小不得少于 10 毫秒。小于 10 毫秒行为未定义。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *       - 该方法不依赖 rtc 引擎
 *       - 该方法必须在进房前调用，且不可与其它音频设备测试功能同时应用。
 *       - 你需调用 stopAudioRecordingDeviceTest{@link #ByteRTCAudioDeviceManager#stopAudioRecordingDeviceTest} 停止测试。
 * @list 音频管理
 */

- (int)startAudioRecordingDeviceTest:(unsigned int)indicationInterval;
/**
 * 
 * @type api
 * @brief 停止音频采集设备测试。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *        - 该方法不依赖 rtc 引擎
 *        - 调用 startAudioRecordingDeviceTest:{@link #ByteRTCAudioDeviceManager#startAudioRecordingDeviceTest:} 后，需调用本方法停止测试。
 * @list 音频管理
 */

- (int)stopAudioRecordingDeviceTest;
/**
 * 
 * @type api
 * @brief 开始音频设备回路测试。 <br>
 *        该方法测试音频采集设备和音频播放设备是否能正常工作。一旦测试开始，音频采集设备会采集本地声音并通过音频播放设备播放出来，同时会收到 rtcEngine:onLocalAudioPropertiesReport:{@link #ByteRTCEngineDelegate#rtcEngine:onLocalAudioPropertiesReport:}。
 * @param indicationInterval 收到回调的时间间隔，单位为 ms。建议设置到大于 200 ms。最小不得少于 10 ms。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *       - 该方法不依赖 rtc 引擎
 *       - 该方法必须在进房前调用。且不可与其它音频设备测试功能同时应用。
 *       - 你需调用 stopAudioDeviceLoopbackTest{@link #ByteRTCAudioDeviceManager#stopAudioDeviceLoopbackTest} 停止测试。
 *       - 该方法仅在本地进行音频设备测试，不涉及网络连接。
 * @list 音频管理
 */

- (int)startAudioDeviceLoopbackTest:(unsigned int)indicationInterval;
/**
 * 
 * @type api
 * @brief 停止音频设备回路测试。
 * @return 方法调用结果 <br>
 *        - 0：方法调用成功
 *        - < 0：方法调用失败
 * @note
 *        - 该方法不依赖 rtc 引擎
 *        - 调用 startAudioDeviceLoopbackTest:{@link #ByteRTCAudioDeviceManager#startAudioDeviceLoopbackTest:} 后，需调用本方法停止测试。
 * @list 音频管理
 */

- (int)stopAudioDeviceLoopbackTest;
@end
/**
 * 
 * @type keytype
 * @brief 混音配置
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioEffectPlayerConfig : NSObject
/**
 * 
 * @type keytype
 * @brief 混音播放类型，详见 ByteRTCAudioMixingType{@link #ByteRTCAudioMixingType}
 */ 

@property(assign, nonatomic) ByteRTCAudioMixingType type;
/**
 * 
 * @brief 与音乐文件原始音调相比的升高/降低值，取值范围为 `[-12，12]`，默认值为 0。每相邻两个值的音高距离相差半音，正值表示升调，负值表示降调。
 */

@property(assign, nonatomic) NSInteger pitch;
/**
 * 
 * @brief 混音播放次数 <br>
 *       - play_count <= 0: 无限循环
 *       - play_count == 1: 播放一次（默认）
 *       - play_count > 1: 播放 play_count 次
 */

@property(assign, nonatomic) NSInteger playCount;
/**
 * 
 * @brief 混音起始位置。默认值为 0，单位为毫秒。
 */

@property(assign, nonatomic) NSInteger startPos;
@end
/**
 * 
 * @type keytype
 * @brief 混音配置
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCMediaPlayerConfig : NSObject
/**
 * 
 * @brief 混音播放类型，详见 ByteRTCAudioMixingType{@link #ByteRTCAudioMixingType}
 */

@property(assign, nonatomic) ByteRTCAudioMixingType type;
/**
 * 
 * @brief 混音播放次数 <br>
 *       - play_count <= 0: 无限循环
 *       - play_count == 1: 播放一次（默认）
 *       - play_count > 1: 播放 play_count 次
 */

@property(assign, nonatomic) NSInteger playCount;
/**
 * 
 * @brief 混音起始位置。默认值为 0，单位为毫秒。
 */

@property(assign, nonatomic) NSInteger startPos;
/**
 * 
 * @brief 设置音频文件混音时，收到 onMediaPlayerPlayingProgress:progress:{@link #ByteRTCMediaPlayerEventHandler#onMediaPlayerPlayingProgress:progress:} 的间隔。单位毫秒。 <br>
 *       - interval > 0 时，触发回调。实际间隔为 10 的倍数。如果输入数值不能被 10 整除，将自动向上取整。例如传入 `52`，实际间隔为 60 ms。
 *       - interval <= 0 时，不会触发回调。
 */
 
@property(assign, nonatomic) NSInteger callbackOnProgressInterval;
/**
 * 
 * @brief 在采集音频数据时，附带本地混音文件播放进度的时间戳。启用此功能会提升远端人声和音频文件混音播放时的同步效果。 <br>
 *        - 仅在单个音频文件混音时使用有效。
 *        - `true` 时开启此功能，`false` 时关闭此功能，默认为关闭。
 */

@property(assign, nonatomic) BOOL syncProgressToRecordFrame;
/**
* 
* @brief 是否自动播放。如果不自动播放，调用 start{@link #ByteRTCMediaPlayer#start} 播放音乐文件。默认为 True。
*/

@property(assign, nonatomic) BOOL autoPlay;
@end
/**
 * 
 * @type callback
 * @brief 内存播放数据源回调
 * @list Audio Management
 */

BYTERTC_APPLE_EXPORT @protocol ByteRTCMediaPlayerCustomSourceProvider <NSObject>
/**
 * 
 * @valid since 3.53
 * @type callback
 * @brief 调用 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:} 接口播放用户传入的内存音频数据时，会触发此回调，用户需要写入音频数据。
 * @param buffer 内存地址。在该地址中写入音频数据，写入音频数据的大小不超过 bufferSize 中填入的数值。支持的音频数据格式有: mp3，aac，m4a，3gp，wav。
 * @param bufferSize 音频数据大小，单位为字节。如果你想停止播放内存音频数据，可在 bufferSize 中填入小于或等于 0 的数，此时 SDK 会停止调用此接口。
 * @return 返回实际读取的音频数据大小。
 * @note 若 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:} 接口调用失败，请在 buffer 和 bufferSize 两个参数中填入 0。 此时 SDK 会停止调用此接口。
 * @list 混音
 */

-(int)onReadData:(uint8_t*_Nonnull) buffer bufferSize:(int)bufferSize;
/**
 * 
 * @valid since 3.53
 * @type callback
 * @brief 根据设置好的内存音频数据的读取位置和读取偏移量对音频数据进行偏移，以便 SDK 读取和分析音频数据。 <br>
 *        在调用 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:} 接口传入内存音频数据，或者调用 setPosition:position:{@link #ByteRTCAudioEffectPlayer#setPosition:position:} 设置了音频数据的起始播放位置后，SDK 会对音频数据进行读取和分析，此时会触发该回调，你需要根据参数中设置的起始读取位置和偏移量进行操作。
 * @param offset 音频数据读取偏移量，单位为字节，取值可正可负。
 * @param whence 音频数据的起始读取位置。参看 ByteRTCMediaPlayerCustomSourceSeekWhence{@link #ByteRTCMediaPlayerCustomSourceSeekWhence}
 * @return
 *         定位成功，返回偏移后的位置信息，或返回音频数据的大小。 <br>
 *         定位失败，返回 -1。
 * @list 混音
 */

-(int64_t)onSeek:(int64_t)offset whence:(ByteRTCMediaPlayerCustomSourceSeekWhence)whence;
@end
/**
 * 
 * @type keytype
 * @brief 音频源
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCMediaPlayerCustomSource : NSObject
/**
 * 
 * @type keytype
 * @brief 仅使用内存播放时，传入对应的 ByteRTCMediaPlayerCustomSourceProvider{@link #ByteRTCMediaPlayerCustomSourceProvider} 实例。
 */

@property(weak, nonatomic) id<ByteRTCMediaPlayerCustomSourceProvider> _Nullable provider;
/**
 * 
 * @type keytype
 * @brief 数据源模式，详见 ByteRTCMediaPlayerCustomSourceMode{@link #ByteRTCMediaPlayerCustomSourceMode}。默认为 `push`。
 */

@property(assign, nonatomic) ByteRTCMediaPlayerCustomSourceMode mode;
/**
 * 
 * @type keytype
 * @brief 数据源类型，详见 ByteRTCMediaPlayerCustomSourceStreamType{@link #ByteRTCMediaPlayerCustomSourceStreamType}。默认为 `raw`。
 */

@property(assign, nonatomic) ByteRTCMediaPlayerCustomSourceStreamType type;

@end
/**
 * 
 * @type keytype
 * @brief 音频属性信息提示的相关配置。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioPropertiesConfig : NSObject
/**
 * 
 * @brief 信息提示间隔，单位：ms <br>
 *       - `<= 0`: 关闭信息提示
 *       - `(0,100]`: 开启信息提示，不合法的 interval 值，SDK 自动设置为 100ms
 *       - `> 100`: 开启信息提示，并将信息提示间隔设置为此值
 */

@property(assign, nonatomic) NSInteger interval;
/**
 * 
 * @brief 是否开启音频频谱检测。
 */

@property(assign, nonatomic) BOOL enableSpectrum;
/**
 * 
 * @brief 是否开启人声检测 (VAD)。
 */

@property(assign, nonatomic) BOOL enableVad;
/**
 * 
 * @brief 音量回调模式。详见 ByteRTCAudioReportMode{@link #ByteRTCAudioReportMode}。
 */

@property(assign, nonatomic) ByteRTCAudioReportMode localMainReportMode;
/**
 * 
 * @brief 适用于音频属性信息提示的平滑系数。取值范围是 `(0.0, 1.0]`。 <br>
 *        默认值为 `1.0`，不开启平滑效果；值越小，提示音量平滑效果越明显。如果要开启平滑效果，可以设置为 `0.3`。
 */

@property(nonatomic, assign) float smooth;

/**
 * 
 * @brief rtcEngine:onLocalAudioPropertiesReport:{@link #ByteRTCEngineDelegate#rtcEngine:onLocalAudioPropertiesReport:} 中包含音频数据的范围。参看 ByteRTCAudioPropertiesMode{@link #ByteRTCAudioPropertiesMode}。 <br>
 *        默认仅包含本地麦克风采集的音频数据和本地屏幕音频采集数据。
 */

@property(assign, nonatomic) ByteRTCAudioPropertiesMode audioReportMode;

/**
 * 
 * @brief 是否回调本地用户的人声基频。
 */

@property(assign, nonatomic) BOOL enableVoicePitch;
@end

/**
 * 
 * @type keytype
 * @brief 音频属性信息
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioPropertiesInfo : NSObject
/**
 * 
 * @brief 线性音量，与原始音量呈线性关系，数值越大，音量越大。取值范围是：[0,255]。 <br>
 *        - [0, 25]: 无声
 *        - [26, 75]: 低音量
 *        - [76, 204]: 中音量
 *        - [205, 255]: 高音量
 */

@property(assign, nonatomic) NSInteger linearVolume;
/**
 * 
 * @brief 非线性音量。由原始音量的对数值转化而来，因此在中低音量时更灵敏，可以用作 Active Speaker（房间内最活跃用户）的识别。取值范围是：[-127，0]，单位 dB。 <br>
 *        - [-127, -60]: 无声
 *        - [-59, -40]: 低音量
 *        - [-39, -20]: 中音量
 *        - [-19, 0]: 高音量
 */

@property(assign, nonatomic) NSInteger nonlinearVolume;
/**
 * 
 * @brief 人声检测（VAD）结果 <br>
 *        - 1: 检测到人声。
 *        - 0: 未检测到人声。
 *        - -1: 未开启 VAD。
 */

@property(assign, nonatomic) NSInteger vad;
/**
 * 
 * @brief 频谱数组。默认长度为 257。
 */

@property(copy, nonatomic) NSArray<NSNumber*> * _Nonnull spectrum;
/**
 * 
 * @brief 本地用户的人声基频，单位为赫兹。 <br>
 *        同时满足以下两个条件时，返回的值为本地用户的人声基频： <br>
 *      - 调用 enableAudioPropertiesReport:{@link #ByteRTCEngine#enableAudioPropertiesReport:}，并设置参数 enableVoicePitch 的值为 `true`。
 *      - 本地采集的音频中包含本地用户的人声。
 *        其他情况下返回 `0`。
 */
    
@property(assign, nonatomic) NSInteger voicePitch;
@end

/**
 * 
 * @type keytype
 * @brief 本地音频属性信息
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCLocalAudioPropertiesInfo : NSObject

/**
 * 
 * @type keytype
 * @hidden 
 * @brief 音频源。预留参数。
 */

@property(assign, nonatomic) ByteRTCAudioSource* _Nullable audioSource;
/**
 * 
 * @type keytype
 * @brief 音频属性信息，详见 ByteRTCAudioPropertiesInfo{@link #ByteRTCAudioPropertiesInfo}
 */

@property(strong, nonatomic) ByteRTCAudioPropertiesInfo *_Nonnull audioPropertiesInfo;
@end

/**
 * 
 * @type keytype
 * @brief 远端音频属性信息
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCRemoteAudioPropertiesInfo : NSObject
/**
 * 
 * @type keytype
 * @brief 远端流的唯一标识
 */

@property(nonatomic) NSString *_Nonnull streamId;

/**
 * 
 * @type keytype
 * @brief 远端流详细信息，详见 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}
 */

@property(strong, nonatomic) ByteRTCStreamInfo *_Nonnull streamInfo;

/**
 * 
 * @type keytype
 * @brief 音频属性信息，详见 ByteRTCAudioPropertiesInfo{@link #ByteRTCAudioPropertiesInfo}
 */

@property(strong, nonatomic) ByteRTCAudioPropertiesInfo *_Nonnull audioPropertiesInfo;
@end

/**
 * 
 * @type keytype
 * @brief 本地用户在房间内的位置坐标，需自行建立空间直角坐标系
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCPosition : NSObject
/**
 * 
 * @brief x 坐标
 */

@property(nonatomic, assign) float x;
/**
 * 
 * @brief y 坐标
 */

@property(nonatomic, assign) float y;
/**
 * 
 * @brief z 坐标
 */

@property(nonatomic, assign) float z;
@end

/**
 * 
 * @type keytype
 * @brief 方向朝向信息
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCOrientation : NSObject
/**
 * 
 * @brief x 坐标
 */

@property(nonatomic, assign) float x;
/**
 * 
 * @brief y 坐标
 */

@property(nonatomic, assign) float y;
/**
 * 
 * @brief z 坐标
 */

@property(nonatomic, assign) float z;
@end

/**
 * 
 * @type keytype
 * @brief 三维朝向信息，三个向量需要两两垂直。参看 ByteRTCOrientation{@link #ByteRTCOrientation}。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCHumanOrientation : NSObject
/**
 * 
 * @brief 正前方朝向，默认值为 {1,0,0}，即正前方朝向 x 轴正方向
 */

@property(nonatomic, strong) ByteRTCOrientation* _Nonnull forward;
/**
 * 
 * @brief 正右方朝向，默认值为 {0,1,0}，即右手朝向 y 轴正方向
 */

@property(nonatomic, strong) ByteRTCOrientation* _Nonnull right;
/**
 * 
 * @brief 正上方朝向，默认值为 {0,0,1}，即头顶朝向 z 轴正方向
 */

@property(nonatomic, strong) ByteRTCOrientation* _Nonnull up;
@end

/**
 * 
 * @type keytype
 * @brief K 歌打分维度。
 */

typedef NS_ENUM(NSInteger, ByteRTCMulDimSingScoringMode) {
    /**
     * 
     * @brief 按照音高进行评分。
     */
    
    ByteRTCMulDimSingScoringModeNote = 0
};
/**
 * 
 * @type keytype
 * @brief 远端音频流精准对齐模式
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioAlignmentMode) {
    /**
     * 
     * @brief 不对齐
     */
    
    ByteRTCAudioAlignmentModeOff = 0,
    /**
     * 
     * @brief 远端音频流都对齐伴奏进度同步播放
     */
    
    ByteRTCAudioAlignmentModeAudioMixing = 1,
};

/**
 * 
 * @type keytype
 * @hidden
 * @brief 实时评分信息。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCSingScoringRealtimeInfo : NSObject
 /**
  * 
  * @brief 当前的播放进度。
  */
 
@property(nonatomic, assign) int currentPosition;
 /**
  * 
  * @brief 演唱者的音高。
  */
 
@property(nonatomic, assign) int userPitch;
 /**
  * 
  * @brief 标准音高。
  */
 
@property(nonatomic, assign) int standardPitch;
 /**
  * 
  * @brief 歌词分句索引。
  */
 
@property(nonatomic, assign) int sentenceIndex;
 /**
  * 
  * @brief 上一句歌词的评分。
  */
 
@property(nonatomic, assign) int sentenceScore;
 /**
  * 
  * @brief 当前演唱的累计分数。
  */
 
@property(nonatomic, assign) int totalScore;
 /**
  * 
  * @brief 当前演唱的平均分数。
  */
 
@property(nonatomic, assign) int averageScore;
@end

/**
 * 
 * @type keytype
 * @brief 标准音高数据组。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCStandardPitchInfo : NSObject
 /**
  * 
  * @brief 开始时间，单位 ms。
  */
 
@property(nonatomic, assign) int startTime;
 /**
  * 
  * @brief 持续时间，单位 ms。
  */
 
@property(nonatomic, assign) int duration;
 /**
  * 
  * @brief 音高。
  */
 
@property(nonatomic, assign) int pitch;
@end

/**
 * 
 * @type keytype
 * @hidden
 * @brief K 歌评分配置。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCSingScoringConfig : NSObject
 /**
  * 
  * @brief 打分维度，详见 ByteRTCMulDimSingScoringMode{@link #ByteRTCMulDimSingScoringMode}。
  */
 
@property(nonatomic, assign) ByteRTCMulDimSingScoringMode mode;
 /**
  * 
  * @brief 音频采样率。仅支持 44100 Hz、48000 Hz。
  */
 
@property(nonatomic, assign) ByteRTCAudioSampleRate sampleRate;
 /**
  * 
  * @brief 歌词文件路径。打分功能仅支持 KRC 歌词文件。
  */
 
@property(nonatomic, copy) NSString *_Nonnull lyricsFilepath;
 /**
  * 
  * @brief 歌曲 midi 文件路径。
  */
 
@property(nonatomic, copy) NSString *_Nonnull midiFilepath;
@end

/**
 * 
 * @type keytype
 * @brief 音频文件录制内容来源。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioFrameSource) {
    /**
     * 
     * @brief 本地麦克风采集的音频数据。
     */
    
    ByteRTCAudioFrameSourceTypeMic = 0,
    /**
     * 
     * @brief 远端所有用户混音后的数据
     */
    
    ByteRTCAudioFrameSourceTypePlayback = 1,
    /**
     * 
     * @brief 本地麦克风和所有远端用户音频流的混音后的数据
     */
    
    ByteRTCAudioFrameSourceTypeMixed = 2,
};
/**
 * 
 * @type keytype
 * @brief 音频质量
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioQuality) {
    /**
     * 
     * @brief 低音质
     */
    
    ByteRTCAudioQualityLow = 0,
    /**
     * 
     * @brief 中音质
     */
    
    ByteRTCAudioQualityMedium = 1,
    /**
     * 
     * @brief 高音质
     */
    
    ByteRTCAudioQualityHigh = 2,
    /**
     * 
     * @brief 超高音质
     */
    
    ByteRTCAudioQualityUltraHigh = 3,
};
/**
 * 
 * @type keytype
 * @brief 音频录制配置
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioRecordingConfig : NSObject
/**
 * 
 * @brief 录制文件路径。一个有读写权限的绝对路径，包含文件名和文件后缀。
 * @note 录制文件的格式仅支持 .aac 和 .wav。
 */

@property(nonatomic, copy) NSString* _Nonnull absoluteFileName;
/**
 * 
 * @brief 录音内容来源，参看 ByteRTCAudioFrameSource{@link #ByteRTCAudioFrameSource}。 <br>
 * 默认为 ByteRTCAudioFrameSourceTypeMixed = 2。
 */

@property(nonatomic, assign) ByteRTCAudioFrameSource frameSource;
/**
 * 
 * @brief 录音采样率。参看 ByteRTCAudioSampleRate{@link #ByteRTCAudioSampleRate}。
 */

@property(nonatomic, assign) ByteRTCAudioSampleRate sampleRate;
/**
 * 
 * @brief 录音音频声道。参看 ByteRTCAudioChannel{@link #ByteRTCAudioChannel}。 <br>
 *       如果录制时设置的的音频声道数与采集时的音频声道数不同： <br>
 *        - 如果采集的声道数为 1，录制的声道数为 2，那么，录制的音频为经过单声道数据拷贝后的双声道数据，而不是立体声。
 *        - 如果采集的声道数为 2，录制的声道数为 1，那么，录制的音频为经过双声道数据混合后的单声道数据。
 */

@property(nonatomic, assign) ByteRTCAudioChannel channel;
/**
 * 
 * @brief 录音音质。仅在录制文件格式为 .aac 时可以设置。参看 ByteRTCAudioQuality{@link #ByteRTCAudioQuality}。 <br>
 * 采样率为 32kHz 时，不同音质录制文件（时长为 10min）的大小分别是： <br>
 *        - 低音质：1.2MB；
 *        - 【默认】中音质：2MB；
 *        - 高音质：3.75MB；
 *        - 超高音质：7.5MB。
 */

@property(nonatomic, assign) ByteRTCAudioQuality quality;
@end

/**
 * 
 * @type keytype
 * @brief 录音配置
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioRecordingState) {
    /**
     * 
     * @brief 录制异常
     */
    
    ByteRTCAudioRecordingStateError = 0,
    /**
     * 
     * @brief 录制进行中
     */
    
    ByteRTCAudioRecordingStateProcessing = 1,
    /**
     * 
     * @brief 已结束录制，并且录制文件保存成功。
     */
    
    ByteRTCAudioRecordingStateSuccess = 2,
};
/**
 * 
 * @type errorcode
 * @brief 音频文件录制的错误码
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioRecordingErrorCode) {
    /**
     * 
     * @brief 录制正常
     */
    
    ByteRTCAudioRecordingErrorCodeOk = 0,
    /**
     * 
     * @brief 没有文件写权限
     */
    
    ByteRTCAudioRecordingErrorCodeNoPermission = -1,
    /**
     * 
     * @brief 没有进入房间
     */
    
    ByteRTCAudioRecordingErrorNotInRoom = -2,
    /**
     * 
     * @brief 录制已经开始
     */
    
    ByteRTCAudioRecordingAlreadyStarted = -3,
    /**
     * 
     * @brief 录制还未开始
     */
    
    ByteRTCAudioRecordingNotStarted = -4,
        /**
     * 
     * @brief 录制失败。文件格式不支持。
     */
    
    ByteRTCAudioRecordingErrorCodeNotSupport = -5,
    /**
     * 
     * @brief 其他异常
     */
    
    ByteRTCAudioRecordingErrorCodeOther = -6,
};
/**
 * 
 * @type keytype
 * @brief 蜂窝网络辅助增强应用的媒体模式
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioEnhancementConfig: NSObject
/**
 * 
 * @brief 对信令消息，是否启用蜂窝网络辅助增强。默认不启用。
 */

@property (assign, nonatomic) BOOL enhanceSignaling;
/**
 * 
 * @brief 对音频，是否启用蜂窝网络辅助增强。默认不启用。
 */

@property (assign, nonatomic) BOOL enhanceAudio;
@end
/**
 * 
 * @type keytype
 * @brief 用户在空间音频坐标系里的位置信息。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCPositionInfo : NSObject
/**
 * 
 * @brief 用户在空间音频坐标系里的位置，需自行建立空间直角坐标系。参看 ByteRTCPosition{@link #ByteRTCPosition}。
 */

@property(strong, nonatomic) ByteRTCPosition *_Nonnull position;
/**
 * 
 * @brief 用户在空间音频坐标系里的三维朝向信息。三个向量需要两两垂直。参看 ByteRTCHumanOrientation{@link #ByteRTCHumanOrientation}。
 */

@property(strong, nonatomic) ByteRTCHumanOrientation *_Nonnull orientation;
@end
