/*
 *  Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 */

#pragma once
#include "bytertc_video_frame.h"
#include "bytertc_audio_frame.h"

namespace bytertc {
/**
 * 
 * @type keytype
 * @brief 转推任务事件
 */

enum MixedStreamTaskEvent {
    /**
     * 
     * @hidden for internal use only
     */
    
    kMixedStreamTaskEventBase = 0,
    /**
     * 
     * @brief 任务发起成功。
     */
    
    kMixedStreamTaskEventStartSuccess = 1,
    /**
     * 
     * @brief 任务发起失败。
     */
    
    kMixedStreamTaskEventStartFailed = 2,
    /**
     * 
     * @brief 任务更新成功。
     */
    
    kMixedStreamTaskEventUpdateSuccess = 3,
    /**
     * 
     * @brief 任务更新失败。
     */
    
    kMixedStreamTaskEventUpdateFailed = 4,
    /**
     * 
     * @brief 任务停止。
     */
    
    kMixedStreamTaskEventStopSuccess = 5,
    /**
     * 
     * @brief 结束任务失败。
     */
    
    kMixedStreamTaskEventStopFailed = 6,
    /**
     * 
     * @brief Warning 事件
     */
    
    kMixedStreamTaskEventWarning = 7,
};


/**
 * 
 * @type errorcode
 * @brief 单流转推直播事件
 */

enum SingleStreamTaskEvent {
    /**
     * 
     * @hidden for internal use only
     */
    
    kSingleStreamTaskEventBase = 0,
    /**
    * 
    * @brief 任务发起成功。
    */
   
    kSingleStreamTaskEventStartSuccess = 1,
    /**
    * 
    * @brief 任务发起失败。
    */
   
    kSingleStreamTaskEventStartFailed = 2,
    /**
    * 
    * @brief 任务停止。
    */
   
    kSingleStreamTaskEventStopSuccess = 3,
    /**
     * 
     * @brief 结束任务失败。
     */
    
    kSingleStreamTaskEventStopFailed = 4,
    /**
    * 
    * @brief Warning 事件
    */
   
    kSingleStreamTaskEventWarning = 5,
};

/**
 * 
 * @type errorcode
 * @brief 合流和 WTN 流任务错误码
 */

enum MixedStreamTaskErrorCode {
    /**
     * 
     * @brief 推流成功。
     */
    
    kMixedStreamTaskErrorCodeOK = 0,
    /**
     * 
     * @hidden currently not available
     * @brief 预留错误码，未启用
     */
    
    kMixedStreamTaskErrorCodeBase= 1090,
    /**
     * 
     * @brief 任务处理超时，请检查网络状态并重试。
     */
    
    kMixedStreamTaskErrorCodeTimeout = 1091,
    /**
     * 
     * @brief 服务端检测到错误的推流参数。
     */
    
    kMixedStreamTaskErrorCodeInvalidParamByServer = 1092,
    /**
     * 
     * @brief 对流的订阅超时
     */
    
    kMixedStreamTaskErrorCodeSubTimeoutByServer = 1093,
    /**
     * 
     * @brief 合流服务端内部错误。
     */
    
    kMixedStreamTaskErrorCodeInvalidStateByServer = 1094,
    /**
     * 
     * @brief 合流服务端推 CDN 失败。
     */
    
    kMixedStreamTaskErrorCodeAuthenticationByCDN  = 1095,
    /**
     * 
     * @brief 服务端未知错误。
     */
    
    kMixedStreamTaskErrorCodeUnKnownByServer = 1096,
    /**
     * 
     * @brief 服务端接收信令超时，请检查网络状态并重试。
     */
    
    kMixedStreamTaskErrorCodeSignalRequestTimeout = 1097,
    /**
     * 
     * @brief 图片合流失败。
     */
    
    kMixedStreamTaskErrorCodeMixImageFailed = 1098,
    /**
     * 
     * @hidden internal use only
     * @brief 缓存未同步。
     */
    
    kMixedStreamTaskErrorCodeStreamSyncWorse = 1099,
    /**
     * 
     * @brief 发布 WTN 流失败
     */
    
    kMixedStreamTaskErrorCodePushWTNFailed = 1195,
    /**
     * 
     * @hidden for internal use only
     */
    
    kMixedStreamTaskErrorCodeMax = 1199,
};

/**
 * 
 * @type errorcode
 * @brief 单流转推任务错误码
 */

enum SingleStreamTaskErrorCode {
    /**
     * 
     * @brief 推流成功。
     */
    
    kSingleStreamTaskErrorCodeOK = 0,
    /**
     * 
     * @hidden currently not available
     * @brief 预留错误码，未启用
     */
    
    kSingleStreamTaskErrorCodeBase= 1090,
    /**
     * 
     * @brief 服务端未知错误。
     */
    
    kSingleStreamTaskErrorCodeUnKnownByServer = 1091,
    /**
     * 
     * @brief 任务处理超时，请检查网络状态并重试。
     */
    
    kSingleStreamTaskErrorCodeSignalRequestTimeout = 1092,
    /**
     * 
     * @brief 服务端检测任务参数不合法
     */
    
    kSingleStreamTaskErrorCodeInvalidParamByServer = 1093,
    /**
     * 
     * @brief 转推任务在目标房间的用户ID被踢出目标房间
     */
    
    kSingleStreamTaskErrorRemoteKicked = 1094,
    /**
     * 
     * @brief 转推任务加入目标房间失败
     */
    
    kSingleStreamTaskErrorCodeJoinDestRoomFailed = 1095,
    /**
     * 
     * @brief 转推任务在源房间拉流超时
     */
    
    kSingleStreamTaskErrorCodeReceiveSrcStreamTimeout = 1096,
    /**
     * 
     * @brief 音视频编码转推任务不支持
     */
    
    kSingleStreamTaskErrorCodeNotSurportCodec = 1097,
};

/**
 * 
 * @hidden for internal use only
 * @type keytype
 * @brief 合流类型。
 */

enum MixedStreamType {
    /**
     * 
     * @brief 服务端合流
     */
    
    kMixedStreamTypeByServer = 0,
    /**
     * 
     * @brief 端云一体合流。SDK 智能决策在客户端或服务端完成合流。 <br>
     *        使用前，请联系技术支持同学开通，否则不生效。
     */
    
    kMixedStreamTypeByClient = 1,
};

/**
 * 
 * @type keytype
 * @brief 合流推送目标类型，指定混流后的媒体流推送到 CDN 还是 WTN。
 */

enum MixedStreamPushTargetType {
    /**
     * 
     * @brief 推送到 CDN
     */
    
    kMixedStreamPushTargetTypeToCDN = 0,
    /**
     * 
     * @brief 推送到 WTN
     */
    
    kMixedStreamPushTargetTypeToWTN = 1,
};

/**
 * 
 * @type keytype
 * @brief AAC 编码规格。
 */

enum MixedStreamAudioProfile {
    /**
     * 
     * @brief AAC-LC 规格，默认值。
     */
    
    kMixedStreamAudioProfileLC = 0,
    /**
     * 
     * @brief HE-AAC v1 规格。
     */
    
    kMixedStreamAudioProfileHEv1 = 1,
    /**
     * 
     * @brief HE-AAC v2 规格。
     */
    
    kMixedStreamAudioProfileHEv2 = 2,
};

/**
 * 
 * @hidden
 * @brief 遗漏的音符
 */
 
enum MixedStreamSyncStrategy {
    /**
     * 
     * @brief 不使用同步策略
     */
    
    kMixedStreamSyncStrategyNoSync = 0,
    /**
     * 
     * @brief 使用音频精准同步策略
     */
     
    kMixedStreamSyncStrategyAudioPreciseSync = 1,
    /**
     * 
     * @brief 使用单通模式同步策略
     */
     
    kMixedStreamSyncStrategySimplexModeSync = 2,
};

/**
 * 
 * @type keytype
 * @brief 音频编码格式。
 */

enum MixedStreamAudioCodecType {
    /**
     * 
     * @brief AAC 格式。
     */
    
    kMixedStreamAudioCodecTypeAAC = 0,
};

/**
 * 
 * @type keytype
 * @brief 服务端合流转推 SEI 内容。
 */

enum MixedStreamSEIContentMode {
    /**
     * 
     * @brief 视频流中包含全部的 SEI 信息。默认设置。
     */
    
    kMixedStreamSEIContentModeDefault = 0,
    /**
     * 
     * @brief 随非关键帧传输的 SEI 数据中仅包含音量信息。 <br>
     *        当设置 `MixedStreamControlConfig.enable_volume_indication` 为 True 时，此参数设置生效。
     */
    
    kMixedStreamSEIContentModeEnableVolumeIndication = 1,
};

/**
 * 
 * @type keytype
 * @brief 视频编码格式。
 */

enum MixedStreamVideoCodecType {
    /**
     * 
     * @brief H.264 格式，默认值。
     */
    
    kMixedStreamVideoCodecTypeH264 = 0,
    /**
     * 
     * @brief ByteVC1 格式。
     */
    
    kMixedStreamVideoCodecTypeByteVC1 = 1,
};

/**
 * 
 * @type keytype
 * @brief 图片或视频流的缩放模式。
 */

enum MixedStreamRenderMode {
    /**
     * 
     * @brief 视窗填满优先，默认值。 <br>
     *        视频尺寸等比缩放，直至视窗被填满。当视频尺寸与显示窗口尺寸不一致时，多出的视频将被截掉。
     */
    
    kMixedStreamRenderModeHidden = 1,
    /**
     * 
     * @brief 视频帧内容全部显示优先。 <br>
     *        视频尺寸等比缩放，优先保证视频内容全部显示。当视频尺寸与显示窗口尺寸不一致时，会把窗口未被填满的区域填充成背景颜色。
     */
    
    kMixedStreamRenderModeFit = 2,
    /**
     * 
     * @brief 视频帧自适应画布。 <br>
     *        视频尺寸非等比例缩放，把窗口充满。在此过程中，视频帧的长宽比例可能会发生变化。
     */
    
    kMixedStreamRenderModeAdaptive = 3,
};

/**
 * 
 * @type keytype
 * @brief 合流输出媒体类型。
 */

enum MixedStreamMediaType {
    /**
     * 
     * @brief 包含音频和视频
     */
    
    kMixedStreamMediaTypeAudioAndVideo = 0,
    /**
     * 
     * @brief 只包含音频
     */
    
    kMixedStreamMediaTypeAudioOnly = 1,
    /**
     * 
     * @hidden currently not available
     * @brief 只包含视频
     */
    
    kMixedStreamMediaTypeVideoOnly = 2,
};

/**
 * 
 * @type keytype
 * @brief 合流布局区域类型，视频区或水印图片区。
 */

enum MixedStreamLayoutRegionType {
    /**
     * 
     * @brief 视频。
     */
    
    kMixedStreamLayoutRegionTypeVideoStream = 0,
    /**
     * 
     * @brief 水印图片。
     */
    
    kMixedStreamLayoutRegionTypeImage = 1,
};

/**
 * 
 * @type keytype
 * @hidden for internal use only on Windows
 * @brief 客户端合流回调视频格式。 <br>
 *        设置为系统不支持的格式时，自动回调系统默认格式。
 */

enum MixedStreamClientMixVideoFormat {
    /**
     * 
     * @brief YUV I420。Android、Windows 默认回调格式。支持系统：Android、Windows。
     */
    
    kMixedStreamClientMixVideoFormatI420 = 0,
    /**
     * 
     * @brief OpenGL GL_TEXTURE_2D 格式纹理。支持系统：安卓。
     */
    
    kMixedStreamClientMixVideoFormatTexture2D = 1,
    /**
     * 
     * @brief CVPixelBuffer BGRA。iOS 默认回调格式。支持系统: iOS。
     */
    
    kMixedStreamClientMixVideoFormatCVPixelBufferBGRA = 2,
    /**
     * 
     * @brief YUV NV12。macOS 默认回调格式。支持系统: macOS。
     */
    
    kMixedStreamClientMixVideoFormatNV12 = 3,
    /**
     * 
     * @brief D3D11Texture2D, BGRA。支持系统: windows。
     */
    
    kMixedStreamClientMixVideoFormatD3D11Texture2D = 4,
};
/**
 * 
 * @type keytype
 * @brief 服务端合流转推发起模式。
 */

enum MixedStreamPushMode {
    /**
     * 
     * @brief 无用户发布媒体流时，发起合流任务无效。默认设置。 <br>
     *        当有用户发布媒体流时，才能发起合流任务。
     */
    
    kMixedStreamPushModeOnStream = 0,
    /**
     * 
     * @brief 无用户发布媒体流时，可以使用占位图发起合流任务。 <br>
     *        占位图设置参看 alternate_image_url{@link #MixedStreamLayoutRegionConfig-alternate_image_url} 和 alternate_image_fill_mode{@link #MixedStreamLayoutRegionConfig-alternate_image_fill_mode}
     */
    
    kMixedStreamPushModeOnStartRequest = 1,
};
/**
 * 
 * @type keytype
 * @brief 服务端合流占位图填充模式。
 */

enum MixedStreamAlternateImageFillMode {
    /**
     * 
     * @brief 占位图跟随用户原始视频帧相同的比例缩放。默认设置。
     */
    
    kMixedStreamAlternateImageFillModeFit = 0,
    /**
     * 
     * @brief 占位图不跟随用户原始视频帧相同的比例缩放，保持图片原有比例。
     */
    
    kMixedStreamAlternateImageFillModeFill = 1,
};
/**
 * 
 * @type keytype
 * @brief Region 中流的类型属性
 */

enum MixedStreamVideoType {
    /**
     * 
     * @brief 主流。包括： <br>
     *        - 由摄像头/麦克风通过内部采集机制，采集到的流
     *        - 通过自定义采集，采集到的流。
     */
    
    kMixedStreamVideoTypeMain = 0,
    /**
     * 
     * @brief 屏幕流。
     */
    
    kMixedStreamVideoTypeScreen = 1,
};

/**
 * 
 * @type keytype
 * @brief WTN 流任务详情 <br>
 *
 */

typedef struct MixedStreamTaskInfo {
    /**
     * 
     * @brief 任务 ID <br>
     * 对于 WTN 流任务，该值代表 WTN 流 ID。你可以通过该 ID，指定需要订阅的 WTN 流。
     */
    
    const char* task_id;
    /**
     * 
     * @brief 任务类型，合流转推 CDN 还是 WTN 流。
     */
    
    MixedStreamPushTargetType push_target_type;
} MixedStreamInfo;

/**
 * 
 * @type keytype
 * @brief 音频合流参数。 <br>
 *         值不合法或未设置时，自动使用默认值。
 */

typedef struct MixedStreamAudioConfig {
    /**
     * 
     * @brief 音频采样率，单位 Hz。可取 32000 Hz、44100 Hz、48000 Hz，默认值为 48000 Hz。建议设置。
     */
    
    int32_t sample_rate = 48000;
    /**
     * 
     * @brief 音频声道数。可取 1（单声道）、2（双声道），默认值为 2。建议设置。
     */
    
    int32_t channels = 2;
    /**
     * 
     * @brief 音频码率，单位 Kbps。可取范围 [32, 192]，默认值为 64 Kbps。建议设置。
     */
    
    int32_t bitrate = 64;
    /**
     * 
     * @brief AAC 编码规格，参看 MixedStreamAudioProfile{@link #MixedStreamAudioProfile}。默认值为 `0` 代表 AAC-LC。
     */
    
    MixedStreamAudioProfile audio_profile = MixedStreamAudioProfile::kMixedStreamAudioProfileLC;
    /**
     * 
     * @brief 音频编码格式，参看 MixedStreamAudioCodecType{@link #MixedStreamAudioCodecType}。当前只有一个值为 `0`，代表 AAC。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    MixedStreamAudioCodecType audio_codec = MixedStreamAudioCodecType::kMixedStreamAudioCodecTypeAAC;
} MixedStreamAudioConfig;

#define MIXED_STREAM_VIDEO_DEFAULT_WIDTH 360
#define MIXED_STREAM_DEFAULT_VIDEO_HEIGHT 640
/**
 * 
 * @type keytype
 * @brief 视频合流配置参数。 <br>
 *        值不合法或未设置时，自动使用默认值。
 */

typedef struct MixedStreamVideoConfig {
    /**
     * 
     * @brief 合流视频宽度。单位为 px，范围为 [2, 1920]，必须是偶数。默认值为 360 px。建议设置。 <br>
     *        设置值为非偶数时，自动向上取偶数。
     */
    
    int32_t width = MIXED_STREAM_VIDEO_DEFAULT_WIDTH;
    /**
     * 
     * @brief 合流视频高度。单位为 px，范围为 [2, 1920]，必须是偶数。默认值为 640 px。建议设置。 <br>
     *        设置值为非偶数时，自动向上取偶数。
     */
    
    int32_t height = MIXED_STREAM_DEFAULT_VIDEO_HEIGHT;
    /**
     * 
     * @brief 合流视频帧率。单位为 FPS，取值范围为 [1,60]，默认值为 15 FPS。建议设置。
     */
    
    int32_t fps = 15;
    /**
     * 
     * @brief 视频 I 帧时间间隔。单位为秒，取值范围为 [1, 5]，默认值为 2 秒。建议设置。 <br>
     *        WTN 流任务不支持设置本参数。 <br>
     *        本参数不支持过程中更新。
     */
    
    int32_t gop = 2;
    /**
     * 
     * @brief 合流视频码率。单位为 Kbps，取值范围为 [1,10000]，默认值为 `500`。建议设置。
     */
    
    int32_t bitrate = 500;
    /**
     * 
     * @brief 视频编码格式，参看 MixedStreamVideoCodecType{@link #MixedStreamVideoCodecType}。默认值为 `0`。建议设置。 <br>
     *        WTN 流的视频编码格式只能为设置为 `0`，H264。 <br>
     *        本参数不支持过程中更新。
     */
    
    MixedStreamVideoCodecType video_codec = MixedStreamVideoCodecType::kMixedStreamVideoCodecTypeH264;
    /**
      * 
      * @brief 是否在服务端合流转推中开启 B 帧。 <br>
      *        - true: 是
      *        - false: 否
      */
     
    bool enable_bframe = false;
} MixedStreamVideoConfig;

/**
 * 
 * @hidden for internal use only on Windows
 * @type keytype
 * @brief 客户端合流参数。
 */

typedef struct ClientMixedStreamConfig {
    /**
     * 
     * @brief 客户端合流是否使用混音，默认为 true。
     */
    
    bool use_audio_mixer = true;
    /**
     * 
     * @brief 客户端合流回调视频格式，参看 MixedStreamClientMixVideoFormat{@link #MixedStreamClientMixVideoFormat}。
     */
    
    MixedStreamClientMixVideoFormat video_format;
} ClientMixedStreamConfig;

/**
 * 
 * @hidden for internal use only
 * @type keytype
 * @brief 转推直播单通同步参数。
 */

typedef struct MixedStreamSyncControlConfig {
    /**
     * 
     * @brief 在进行同步处理时，缓存音视频流的最大长度。单位为毫秒。默认值为 2000。
     * @note 参与转推直播的这些媒体流延迟越高，应该将此值设置的越大。但此值越大，因缓存媒体流造成的内存占用也会更大。推荐值为 `2000`。
     */
    
    int32_t max_cache_time_ms = 2000;
    /**
     * 
     * @brief 是否在转推直播时，启用单通模式。默认为 false，不启用。 <br>
     *        启用单通模式时，RTC SDK 会对指定的多个用户的媒体流进行同步处理，再合流后推送到 CDN，但基准流所属用户不会播放来自其他用户的媒体流。你需要设定以下参数。 <br>
     *        非单通模式时，RTC SDK 不会对媒体流进行同步处理，只是简单合流后推送到 CDN。以下参数设定无效。
     */
    
    MixedStreamSyncStrategy sync_strategy = MixedStreamSyncStrategy::kMixedStreamSyncStrategyNoSync;
    /**
     * 
     * @brief 在进行同步处理时，基准流所属用户的 ID。默认为空。
     */
    
    const char* base_user_id = nullptr;
    /**
     * 
     * @brief 是否通过 RTC SDK 进行转推直播。默认为 True。 <br>
     *        如果选择 `False`，你会通过 onCacheSyncVideoFrames{@link #IClientMixedStreamObserver#onCacheSyncVideoFrames} 收到同步的帧，你可以使用此视频帧，自行实现合流转推。
     */
    
    bool video_need_sdk_mix = true;
} MixedStreamSyncControlConfig;

/**
 * 
 * @type keytype
 * @brief 图片合流水印图片分辨率。
 */

typedef struct MixedStreamLayoutRegionImageWaterMarkConfig {
    /**
     * 
     * @brief 原始图片的宽度，单位为 px。
     */
    
    int image_width = 0;
    /**
     * 
     * @brief 原始图片的高度，单位为 px。
     */
    
    int image_height = 0;
} MixedStreamLayoutRegionImageWaterMarkConfig;

/**
 * 
 * @type keytype
 * @brief 推流 CDN 的空间音频参数。
 */

typedef struct MixedStreamSpatialAudioConfig {
    /**
     * 
     * @brief 是否开启推流 CDN 时的空间音频效果。
     * @note 当你启用此效果时，你需要设定推流中各个 MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig} 的 `spatial_position` 值，实现空间音频效果。
     */
    
    bool enable_spatial_render;
    /**
     * 
     * @brief 听众的空间位置。参看 Position{@link #Position}。 <br>
     *        听众指收听来自 CDN 的音频流的用户。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    Position audience_spatial_position;
    /**
     * 
     * @brief 听众的空间朝向。参看 HumanOrientation{@link #HumanOrientation}。 <br>
     *        听众指收听来自 CDN 的音频流的用户。
     */
    
    HumanOrientation audience_spatial_orientation;
} MixedStreamSpatialAudioConfig;

/**
 * 
 * @type keytype
 * @brief 服务端合流控制参数
 */

typedef struct MixedStreamControlConfig {
    /**
     * 
     * @valid since 3.56
     * @brief 是否开启单独发送声音提示 SEI 的功能： <br>
     *        - true：开启；
     *        - false：（默认）关闭。
     *        开启后，你可以通过 `MixedStreamControlConfig.sei_content_mode` 控制 SEI 的内容是否只携带声音信息。
     */
    
    bool enable_volume_indication = false;
    /**
     * 
     * @valid since 3.56
     * @brief 声音提示间隔，单位为秒，取值范围为 [0.3,+∞)，默认值为 2。 <br>
     *        此值仅取整百毫秒。若传入两位及以上小数，则四舍五入取第一位小数的值。例如，若传入 0.36，则取 0.4。
     */
    
    float volume_indication_interval = 2.0f;
    /**
     * 
     * @valid since 3.56
     * @brief 有效音量大小，取值范围为 [0, 255]，默认值为 0。 <br>
     *        超出取值范围则自动调整为默认值，即 0。
     */
    
    int talk_volume = 0;
    /**
     * 
     * @valid since 3.56
     * @brief 声音信息 SEI 是否包含音量值： <br>
     *        - true：是。
     *        - false：（默认）否。
     */
    
    bool is_add_volume_value = false;
    /**
     * 
     * @valid since 3.56
     * @brief 设置 SEI 内容。参看 MixedStreamSEIContentMode{@link #MixedStreamSEIContentMode}。 <br>
     * 默认值为 `0`，视频流中包含全部的 SEI 信息
     */
    
    MixedStreamSEIContentMode sei_content_mode = kMixedStreamSEIContentModeDefault;
    /**
     * 
     * @valid since 3.56
     * @brief SEI 信息的 payload type。 <br>
     *        默认值为 `100`，只支持设置 `5` 和 `100`。 <br>
     *        在转推直播的过程中，该参数不支持变更。
     */
    
    int sei_payload_type = 100;
    /**
     * 
     * @valid since 3.56
     * @brief SEI 信息的 payload UUID。
     * @note PayloadType 为 `5` 时，必须填写 PayloadUUID，否则会收到错误回调，错误码为 1091。 <br>
     *         PayloadType 不是 `5` 时，不需要填写 PayloadUUID，如果填写会被后端忽略。 <br>
     *         该参数长度需为 32 位，否则会收到错误码为 1091 的回调。 <br>
     *         该参数每个字符的范围需为 [0, 9] [a, f] [A, F] <br>
     *         该参数不应带有`-`字符，如系统自动生成的 UUID 中带有`-`，则应删去。 <br>
     *         在转推直播的过程中，该参数不支持变更。
     */
     
     const char* sei_payload_uuid = nullptr;
     /**
      * 
      * @valid since 3.57
      * @brief 设置合流推到 CDN 时输出的媒体流类型。参看 MixedStreamMediaType{@link #MixedStreamMediaType}。 <br>
      *        默认输出音视频流。支持输出纯音频流，但暂不支持输出纯视频流。
      */
     
     MixedStreamMediaType media_type = MixedStreamMediaType::kMixedStreamMediaTypeAudioAndVideo;
     /**
      * 
      * @valid since 3.57
      * @brief 设置是否在没有用户发布流的情况下发起转推直播，默认不允许。具体参看 MixedStreamPushMode{@link #MixedStreamPushMode}。 <br>
      *        该参数在发起合流任务后的转推直播过程中不支持动态变更。
      */
     
     MixedStreamPushMode push_stream_mode = MixedStreamPushMode::kMixedStreamPushModeOnStream;
} MixedStreamControlConfig;

/**
 * 
 * @type keytype
 * @brief WTN 流的布局模式
 */

enum StreamLayoutMode {
    /**
     * 
     * @brief 自动布局
     */
    
    kStreamLayoutModeAuto = 0,
    /**
     * 
     * @brief 自定义
     */
    
    kStreamLayoutModeCustom = 2
};
/**
 * 
 * @type keytype
 * @brief WTN 流的补帧模式
 */

enum InterpolationMode {
    /**
     * 
     * @type keytype
     * @brief 补最后一帧
     */
    
    kInterpolationModeLastFrameFill = 0,
    /**
     * 
     * @type keytype
     * @brief 补背景图片
     */
    
    kInterpolationModeBackgroundImageFill = 1
};
/**
 * 
 * @type keytype
 * @brief WTN 流视频裁剪配置
 */

typedef struct SourceCrop {
    /**
     * 
     * @brief 裁剪后得到的视频帧左上角横坐标相对于裁剪前整体画面的归一化比例，取值范围[0.0, 1.0)。 <br>
     * 默认值为 0.0
     */
    
    float location_x = 0.0;
    /**
     * 
     * @brief 裁剪后得到的视频帧左上角纵坐标相对于裁剪前整体画面的归一化比例，取值范围[0.0, 1.0) <br>
     * 默认值为 0.0
     */
    
    float location_y = 0.0;
    /**
     * 
     * @brief 裁剪后得到的视频帧宽度相对于裁剪前整体画面的归一化比例，取值范围(0.0, 1.0] <br>
     * 默认值为 1.0
     */
    
    float width_proportion = 1.0;
    /**
     * 
     * @brief 裁剪后得到的视频帧高度相对于裁剪前整体画面的归一化比例，取值范围(0.0, 1.0] <br>
     * 默认值为 1.0
     */
    
    float height_proportion = 1.0;
} SourceCrop;
/**
 * 
 * @type keytype
 * @brief 单个图片或视频流在合流中的布局信息。 <br>
 *        开启合流功能后，在多路图片或视频流合流时，你可以设置其中一路流在合流中的预设布局信息。
 */

typedef struct MixedStreamLayoutRegionConfig {
    /**
     * 
     * @brief 合流用户的 ID。建议设置。
     */
    
    const char* user_id = nullptr;
   /**
     * 
     * @brief 图片或视频流所在房间的房间 ID。建议设置。 <br>
     *        如果此图片或视频流是通过 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} 转发到用户所在房间的媒体流时，你应将房间 ID 设置为用户所在的房间 ID。
     */
    
    const char* room_id = nullptr;
    /**
     * 
     * @brief 单个用户画面左上角在整个画布坐标系中的 X 坐标（pixel），即以画布左上角为原点，用户画面左上角相对于原点的横向位移。 <br>
     *        取值范围为 [0, 整体画布宽度)。默认值为 0。
     */
    
    int location_x = 0;
    /**
     * 
     * @brief 单个用户画面左上角在整个画布坐标系中的 Y 坐标（pixel），即以画布左上角为原点，用户画面左上角相对于原点的纵向位移。 <br>
     *        取值范围为 [0, 整体画布高度)。默认值为 0。
     */
    
    int location_y = 0;
    /**
     * 
     * @brief 单个用户画面的宽度。取值范围为 [0, 整体画布宽度]，默认值为 360。
     */
    
    int width = MIXED_STREAM_VIDEO_DEFAULT_WIDTH;
    /**
     * 
     * @brief 单个用户画面的高度。取值范围为 [0, 整体画布高度]，默认值为 640。
     */
    
    int height = MIXED_STREAM_DEFAULT_VIDEO_HEIGHT;
    /**
     * 
     * @brief 透明度，可选范围为 (0.0, 1.0]，0.0 为全透明。默认值为 1.0。
     */
    
    float alpha = 1.0f;
    /**
     * 
     * @brief 圆角半径相对画布宽度的比例。默认值为 `0.0`。 <br>
     *        做范围判定时，首先根据画布的宽高，将 `width`，`height`，和 `corner_radius` 分别转换为像素值：`width_px`，`height_px`，和 `corner_radius_px`。然后判定是否满足 `corner_radius_px < min(width_px/2, height_px/2)`：若满足，则设置成功；若不满足，则将 `corner_radius_px` 设定为 `min(width_px/2, height_px/2)`，然后将 `corner_radius` 设定为 `corner_radius_px` 相对画布宽度的比例值。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    float corner_radius = 0;

    /**
     * 
     * @brief 用户视频布局在画布中的层级。取值范围为 [0 - 100]，0 为底层，值越大越上层。默认值为 0。
     */
    
    int32_t z_order = 0;
    /**
     * 
     * @brief 是否为本地用户： <br>
     *        - true：是
     *        - false:（默认值）否
     *        WTN 流任务不支持设置本参数。
     */
    
    bool is_local_user = false;
    /**
     * 
     * @brief 是否为屏幕流。默认值为 `kMixedStreamVideoTypeMain`，主流。参看 MixedStreamVideoType{@link #MixedStreamVideoType}。
     */
    
    MixedStreamVideoType stream_type = MixedStreamVideoType::kMixedStreamVideoTypeMain;
    /**
     * 
     * @brief 合流内容控制。默认值为 `kMediaTypeAudioAndVideo`，参看 MixedStreamMediaType{@link #MixedStreamMediaType}。
     */
    
    MixedStreamMediaType media_type = MixedStreamMediaType::kMixedStreamMediaTypeAudioAndVideo;
    /**
     * 
     * @brief 图片或视频流的缩放模式，参看 MixedStreamRenderMode{@link #MixedStreamRenderMode}。默认值为 `1`，视窗填满优先。建议设置。
     */
    
    MixedStreamRenderMode render_mode = MixedStreamRenderMode::kMixedStreamRenderModeHidden;
    /**
     * 
     * @brief 合流布局区域类型，视频区或水印图片区。参看 MixedStreamLayoutRegionType{@link #MixedStreamLayoutRegionType}。默认值为 `0`，视频。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    MixedStreamLayoutRegionType region_content_type = MixedStreamLayoutRegionType::  kMixedStreamLayoutRegionTypeVideoStream;
    /**
     * 
     * @brief 水印图 RGBA 数据。当 `region_content_type` 为图片类型时需要设置。 <br>
     *        - `kMixedStreamLayoutRegionTypeImage = 1` 时，传入图片 RGBA 数据。
     *        - `kMixedStreamLayoutRegionTypeVideoStream = 0` 时传入空。
     *        WTN 流任务不支持设置本参数。
     */
    
    uint8_t* image_water_mark = nullptr;
    /**
     * 
     * @type keytype
     * @brief 水印图参数。当 `region_content_type` 为图片类型时需要设置。 <br>
     *        - `kMixedStreamLayoutRegionTypeImage = 1` 时，传入图片参数，参看 MixedStreamLayoutRegionImageWaterMarkConfig{@link #MixedStreamLayoutRegionImageWaterMarkConfig}。
     *        - `kMixedStreamLayoutRegionTypeVideoStream = 0` 时传入空。
     *        WTN 流任务不支持设置本参数。
     */
    
    MixedStreamLayoutRegionImageWaterMarkConfig image_water_mark_param;
    /**
     * 
     * @valid since 3.57
     * @brief 设置占位图的填充模式。 <br>
     *        该参数用来控制当用户停止发布视频流，画面恢复为占位图后，此时占位图的填充模式。默认值为 `kMixedStreamAlternateImageFillModeFit`，占位图跟随用户原始视频帧相同的比例缩放。 <br>
     *        参看 MixedStreamAlternateImageFillMode{@link #MixedStreamAlternateImageFillMode}。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    MixedStreamAlternateImageFillMode alternate_image_fill_mode = kMixedStreamAlternateImageFillModeFit;
    /**
     * 
     * @valid since 3.57
     * @brief 设置占位图的 URL，长度小于 1024 字符.
     */
    
    const char* alternate_image_url = nullptr;
    /**
     * 
     * @type keytype
     * @brief 空间位置。参看 Position{@link #Position}。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    Position spatial_position;
    /**
     * 
     * @type keytype
     * @brief 是否开启空间音频效果。默认值为 true。 <br>
     *        WTN 流任务不支持设置本参数。
     */
    
    bool apply_spatial_audio = true;
    /**
     * 
     * @brief 支持对每一路参与WTN 流的视频进行裁剪，参看 SourceCrop{@link #SourceCrop} <br>
     * 合流转推任务不支持。
     */
    
    SourceCrop source_crop;
} MixedStreamLayoutRegionConfig;
/**
 * 
 * @type keytype
 * @brief 数据帧类型
 */

enum DataFrameType {
    /**
     * 
     * @brief SEI 视频帧
     */
    
    kDataFrameTypeSei = 0,
};

/**
 * 
 * @hidden for internal use only
 * @type keytype
 * @brief 合流房间状态
 */

enum TranscoderRoomStatus {
    /**
     * @brief join room status
     */
    kRoomStatusJoinRoom,
    /**
     * @brief leave room status
     */
    kRoomStatusLeaveRoom,
    /**
     * @brief room status was offline
     */
    kRoomStatusOffline,
    /**
     * @brief room status was online
     */
    kRoomStatusOnline,
};

/**
 * @hidden for internal use only
 */
typedef size_t status_t;
/**
 * 
 * @hidden for internal use only
 */
 
class IVideoFrame;
/**
 * 
 * @hidden for internal use only
 */
 
class IAudioFrame;

/**
 * 
 * @hidden for internal use only
 * @type keytype
 * @brief 数据帧
 */

typedef struct IDataFrame {
    /**
     * 
     * @brief 数据帧类型，参看 DataFrameType{@link #DataFrameType}
     */
    
    DataFrameType frame_type = kDataFrameTypeSei;
    /**
     * 
     * @brief 数据帧数据
     */
    
    uint8_t* u8_data = nullptr;
    /**
     * 
     * @brief 数据帧数据大小
     */
    
    uint32_t u32_data_size = 0;
    /**
     * 
     * @brief 数据帧时间戳，单位：微秒
     */
    
    uint64_t u64_ts_us = 0;
} IDataFrame;

/**
 * 
 * @type keytype
 * @brief 合流/WTN 流的转推目标参数。
 * 如无特别说明，参数可适用于 WTN 流和合流转推任务。
 * 如无特别说明，参数可用于启动和更新任务。
 */

typedef struct MixedStreamPushTargetConfig {
    /**
     * 
     * @type keytype
     * @brief WTN 流 ID。合流任务不支持设置本参数。
     */
    
    const char* push_wtn_stream_id = nullptr;
    /**
     * 
     * @type api
     * @brief 推流 CDN 地址。仅支持 RTMP 协议，Url 必须满足正则 `/^rtmps?:\/\
     *        - 本参数不支持过程中更新。
     *        - WTN 流任务不支持设置本参数。
     */
    
    const char* push_cdn_url = nullptr;
    /**
     * 
     * @type keytype
     * @brief 推流任务类型。
     */
    
    MixedStreamPushTargetType push_target_type = kMixedStreamPushTargetTypeToCDN;
} MixedStreamPushTargetConfig;

/**
 * 
 * @type keytype
 * @brief 合流转推配置参数。
 * 如无特别说明，参数可适用于WTN 流和合流转推任务。
 * 如无特别说明，参数可用于启动和更新任务。
 */

class IMixedStreamConfig {
public:
    /**
     * 
     * @type api
     * @brief 构建合流任务参数
     * @return 合流任务参数
     */
    
    static BYTERTC_STATIC_API IMixedStreamConfig* createMixedStreamConfig();
    /**
     * 
     * @type api
     * @brief 获取发起合流任务用户所在的房间 ID。
     * @return 合流房间 ID
     */
    
    virtual const char* getRoomID() = 0;
    /**
     * 
     * @type api
     * @brief 设置合流房间 ID
     * @param room_id 发起合流的用户所在的房间 ID
     * @note
     *        本参数不支持过程中更新。
     */
    
    virtual void setRoomID(const char* room_id) = 0;
    /**
     * 
     * @type api
     * @brief 获取发起合流任务的用户 ID。
     * @return 用户 ID。
     */
    
    virtual const char* getUserID() = 0;
    /**
     * 
     * @type api
     * @brief 设置发起合流任务的用户 ID。`room_id` 和 `user_id` 长度相加不得超过 126 字节。建议设置。 <br>
     *        本参数不支持过程中更新。
     * @param user_id 推流用户 ID。
     */
    
    virtual void setUserID(const char* user_id) = 0;
    /**
     * 
     * @type api
     * @brief 获取合流音频参数。
     * @return 合流音频参数内容，参看 MixedStreamAudioConfig{@link #MixedStreamAudioConfig}。
     */
    
    virtual MixedStreamAudioConfig getAudioConfig() = 0;
    /**
     * 
     * @type api
     * @brief 设置音频参数。建议设置。
     * @param MixedStreamAudioConfig 音频参数，参看 MixedStreamAudioConfig{@link #MixedStreamAudioConfig}。
     * @note
     *      - 本参数不支持过程中更新。
     *      - WTN 流任务不支持设置本参数。
     */
    
    virtual void setAudioConfig(const MixedStreamAudioConfig&) = 0;
    /**
     * 
     * @type api
     * @brief 获取视频转码参数。
     * @return 视频转码参数内容，参看 MixedStreamVideoConfig{@link #MixedStreamVideoConfig}。
     */
    
    virtual MixedStreamVideoConfig getVideoConfig() = 0;
    /**
     * 
     * @type api
     * @brief 设置视频参数。建议设置。
     * @param MixedStreamVideoConfig 视频参数，参看 MixedStreamVideoConfig{@link #MixedStreamVideoConfig}。
     */
    
    virtual void setVideoConfig(const MixedStreamVideoConfig&) = 0;
    /**
     * 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief 获取转推 CDN 时的空间音频参数。
     * @return 参看 MixedStreamSpatialAudioConfig{@link #MixedStreamSpatialAudioConfig}。
     */
    
    virtual MixedStreamSpatialAudioConfig getSpatialAudioConfig() = 0;
    /**
     * 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief 设定转推 CDN 时的空间音频效果。参看 MixedStreamSpatialAudioConfig{@link #MixedStreamSpatialAudioConfig}。
     */
    
    virtual void setSpatialAudioConfig(const MixedStreamSpatialAudioConfig&) = 0;
    /**
     * 
     * @hidden for internal use only
     * @type api
     * @brief 获取动态扩展自定义参数
     * @return 动态扩展自定义参数
     */
    
    virtual const char* getAdvancedConfig() = 0;
    /**
     * 
     * @hidden for internal use only
     * @type api
     * @brief 设置动态扩展自定义参数
     * @param advanced_config 动态扩展自定义参数
     */
    
    virtual void setAdvancedConfig(const char* advanced_config) = 0;
    /**
     * 
     * @hidden for internal use only
     * @type api
     * @brief 获取业务透传鉴权信息
     * @return 业务透传鉴权信息
     */
    
    virtual const char* getAuthInfo() = 0;
    /**
     * 
     * @hidden for internal use only
     * @type api
     * @brief 设置业务透传鉴权信息
     * @param auth_info 业务透传鉴权信息
     */
    
    virtual void setAuthInfo(const char* auth_info) = 0;
    /**
     * 
     * @hidden for internal use only
     * @type api
     * @brief 获取转推直播同步参数。
     * @return 转推直播同步参数，参看 MixedStreamSyncControlConfig{@link #MixedStreamSyncControlConfig}。
     */
    
    virtual MixedStreamSyncControlConfig getSyncControlConfig() = 0;
    /**
     * 
     * @hidden for internal use only
     * @type api
     * @brief 设置转推直播同步参数。参看 MixedStreamSyncControlConfig{@link #MixedStreamSyncControlConfig}。
     */
    
    virtual void setSyncControlConfig(MixedStreamSyncControlConfig&) = 0;
    /**
     * 
     * @type api
     * @brief 获取服务端合流控制参数。参看 MixedStreamControlConfig{@link #MixedStreamControlConfig}。
     */
    
    virtual MixedStreamControlConfig getControlConfig() = 0;
    /**
     * 
     * @type api
     * @brief 设置服务端合流控制参数。参看 MixedStreamControlConfig{@link #MixedStreamControlConfig}。
     * @param config 参看 MixedStreamControlConfig{@link #MixedStreamControlConfig}.
     */
    
    virtual void setControlConfig(MixedStreamControlConfig& config) = 0;
    /**
     * 
     * @type api
     * @brief 获取用户配置的额外信息。
     * @return 用户配置的额外信息。
     */
    
    virtual const char* getUserConfigExtraInfo() = 0;
    /**
     * 
     * @type api
     * @brief 设置用户配置的额外信息。
     * @param user_ext_info 用户配置的额外信息。
     * @note
     *      WTN 流任务不支持设置本参数。
     */
    
    virtual void setUserConfigExtraInfo(const char* user_ext_info) = 0;
    /**
     * 
     * @type api
     * @brief 获取 WTN 流的布局模式。
     * @return StreamLayoutMode{@link #StreamLayoutMode} 信息。
     */
    
    virtual StreamLayoutMode getStreamLayoutMode() = 0;
    /**
     * 
     * @type api
     * @brief 设置 WTN 流的布局模式。
     * @param mode 布局模式。参看 StreamLayoutMode{@link #StreamLayoutMode}。
     */
    
    virtual void setStreamLayoutMode(StreamLayoutMode mode) = 0;
    /**
     * 
     * @type api
     * @brief 获取 WTN 流的补帧模式。
     * @return InterpolationMode{@link #InterpolationMode} 信息。
     */
    
    virtual InterpolationMode getInterpolationMode() = 0;
    /**
     * 
     * @type api
     * @brief 设置 WTN 流的补帧模式。
     * @param mode 补帧模式。参看 InterpolationMode{@link #InterpolationMode}。
     */
    
    virtual void setInterpolationMode(InterpolationMode mode) = 0;

    /**
     * 
     * @type api
     * @brief 获取 WTN 流 ID。
     * @return WTN 流 ID。
     */
    
    virtual const char* getPushWTNStreamID() = 0;
    /**
     * 
     * @type api
     * @brief 设置 WTN 流 ID。
     * @param stream_id WTN 流 ID。
     */
    
    virtual void setPushWTNStreamID(const char* stream_id) = 0;
    /**
     * 
     * @type api
     * @brief 获取任务类型。
     * @return MixedStreamPushTargetType{@link #MixedStreamPushTargetType}
     */
    
    virtual MixedStreamPushTargetType getPushTargetType() = 0;
    /**
     * 
     * @type api
     * @brief 设置推流目标。
     * @param type 参看 MixedStreamPushTargetType{@link #MixedStreamPushTargetType}。
     */
    
    virtual void setPushTargetType(MixedStreamPushTargetType type) = 0;

    /**
     * 
     * @type api
     * @brief 获取背景颜色。
     * @return 背景颜色，用十六进制颜色码（HEX）表示。例如，#FFFFFF 表示纯白，#000000 表示纯黑。默认值为 #000000。建议设置。
     */
    
    virtual const char* getBackgroundColor() = 0;
    /**
     * 
     * @type api
     * @brief 设置背景颜色。
     * @param background_color 背景颜色，用十六进制颜色码（HEX）表示。例如，#FFFFFF 表示纯白，#000000 表示纯黑。默认值为 #000000。建议设置。
     */
    
    virtual void setBackgroundColor(const char* background_color) = 0;
    /**
     * 
     * @type api
     * @brief 获取背景图片的地址。
     * @return 背景图片的地址。
     */
    
    virtual const char* getBackgroundImageURL() = 0;
    /**
     * 
     * @type api
     * @brief 设置合流后整体画布的背景图片。
     * @param background_image_url 图片的 URL，长度最大为 1023 bytes。 <br>
     *        必须是 http 开头，支持的图片格式包括：JPG, JPEG, PNG。如果背景图片的宽高和整体屏幕的宽高不一致，背景图片会缩放到铺满屏幕。
     */
    
    virtual void setBackgroundImageURL(const char* background_image_url) = 0;
    /**
     * 
     * @type api
     * @brief 获取合流视窗布局信息。
     * @param index 视窗对应下标。
     * @return 合流视窗布局信息，参看 MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig}。
     */
    
    virtual MixedStreamLayoutRegionConfig getLayoutByIndex(int32_t index) = 0;
    /**
     * 
     * @type api
     * @brief 获取合流参数中视窗的数量
     * @return 合流参数中视窗的数量
     */
    
    virtual int32_t getLayoutRegionsSize() = 0;
    /**
     * 
     * @type api
     * @brief 设置视频流合流整体布局信息。
     * @param regions 用户布局信息列表。为 MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig} 数据类型的数组。每一个该类型对象为一路单独的视频流的布局信息。 <br>
     *                       值不合法或未设置时，自动使用默认值。建议设置。
     * @param regions_size 合流视窗数量。建议设置。
     */
    
    virtual void setLayoutConfig(MixedStreamLayoutRegionConfig regions[], int32_t regions_size) = 0;
    virtual ~IMixedStreamConfig() = default;
};

/**
 * 
 * @type keytype
 * @brief 单流转推配置目标房间信息
 */

typedef struct DestInfo {
    /**
     * 
     * @brief 跨房间转发媒体流过程中目标房间 ID
     */
    
    const char* room_id;
    /**
     * 
     * @brief 转发目标房间中自定义用户ID 
     */
    
    const char* user_id;
} DestInfo;

/**
 * 
 * @type keytype
 * @brief 单流转推类型，指定将媒体流转推到 CDN 还是 RTC 房间。
 */

enum SingleStreamPushType {
    /**
     * 
     * @brief 转推到 CDN
     */
    
    kPushToCDN = 1,
    /**
     * 
     * @brief 转推到 RTC 房间
     */
    
    kPushToRTC = 2
};

/**
 * 
 * @type keytype
 * @brief 单流转推直播配置参数。
 */

typedef struct PushSingleStreamParam {
    /**
     * 
     * @brief 媒体流所在的房间 ID
     */
    
    const char* room_id;
    /**
     * 
     * @brief 媒体流所属的用户 ID
     */
    
    const char* user_id;
    /**
     * 
     * @brief 推流 CDN 地址。仅支持 RTMP 协议，Url 必须满足正则 `/^rtmps?:\/\
     *        本参数不支持过程中更新。
     */
    
    const char* uri;
    /**
     * 
     * @brief 媒体流是否为屏幕流。
     */
    
    bool is_screen_stream;
    /**
     * 
     * @brief 转推目标房间数组，默认值为nullptr
     */
    
    DestInfo* dest_info = nullptr;
    /**
     * 
     * @brief 转推目标房间数量,`dest_info` 的数组长度， <br>
     *        默认值为0,取值范围[0, 8]
     */
    
    int32_t dest_info_size = 0;
    /**
     * 
     * @brief 单流转推类型，指定将媒体流转推到 CDN 还是 RTC 房间，默认值为转推到 CDN。
     */
    
    SingleStreamPushType push_type = kPushToCDN;
}PushSingleStreamParam;

/**
* 
* @hidden internal use only
* @type keytype
* @brief 缓存同步模式。
*/

enum ChorusCacheSyncMode {
    /**
     * 
     * @brief 合唱场景下，主唱应采用此模式，以发送带时间戳信息的媒体数据。
     */
    
    kChorusCacheSyncModeProducer= 0,
    /**
     * 
     * @brief 合唱场景下，副唱应采用此模式。 <br>
     *        此模式下，副唱收到来自主唱的带时间戳的媒体数据。副唱发送的媒体数据中带有来自主唱的时间戳。
     */
    
    kChorusCacheSyncModeRetransmitter= 1,
    /**
     * 
     * @brief 合唱场景下，听众应采用此模式。 <br>
     *        此模式下，听众收到来自主唱的时间戳，并据此对齐来自主唱和副唱的媒体数据，以获得良好的合唱播放效果。
     */
    
    kChorusCacheSyncModeConsumer = 2
};
/**
* 
* @hidden internal use only
* @type keytype
* @brief 缓存同步事件。
*/

enum ChorusCacheSyncEvent {
     /**
     * 
     * @brief 成功
     */
    
    kChorusCacheSyncEventStartSuccess = 0,
     /**
     * 
     * @brief 失败。
     */
    
    kChorusCacheSyncEventStartFailed = 1,
};
/**
* 
* @hidden internal use only
* @type errorcode
* @brief 缓存同步错误码。
*/

enum ChorusCacheSyncError {
    /**
     * 
     * @brief 成功。
     */
    
    kChorusCacheSyncErrorOK = 0,
   /**
    * 
    * @brief 失败。推送至 CDN 时，应进行以下设置： <br>
    *        - `IMixedStreamConfig.MixedStreamSyncControlConfig.enable_sync = true`；
    *        - `IMixedStreamConfig.MixedStreamSyncControlConfig.base_user_id = {uid of producer}`。
    */
       
    kChorusCacheSyncErrorWrongState = 1,
    /**
    * 
    * @brief 缓存同步功能已启动，不需要重复开启。
    */
    
    kChorusCacheSyncErrorAlreadyRunning = 2
};
/**
* 
* @hidden internal use only
* @type keytype
* @brief 缓存同步配置。
*/

struct ChorusCacheSyncConfig {
     /**
      * 
      * @brief 最大媒体缓存时长（ms）。 <br>
      *        取值范围是 `[500, 2500]`，默认值是 `2000`。 <br>
      *        值越大，同步效果越好，但会造成占用内存较大。如果参与缓存同步的各路媒体流之间的时间差超过此值，会造成丢帧。
      */
     
    int32_t max_cache_time_ms = 2000;
    /**
      * 
      * @brief 收到 onSyncedVideoFrames{@link #IChorusCacheSyncObserver#onSyncedVideoFrames} 的频率。 <br>
      *        默认值是 `15`。此值通常应小于等于原始视频帧率；如果大于原始视频帧率，可能会收到重复帧。
      */
     
    int32_t video_fps = 15;
    /**
      * 
      * @brief 模式。参看 ChorusCacheSyncMode{@link #ChorusCacheSyncMode}. 默认值是 `retransmitter`。
      */
     
    ChorusCacheSyncMode mode = kChorusCacheSyncModeRetransmitter;
};

/**
 * 
 * @type callback
 * @hidden for internal use only on Windows
 * @brief 合流推流 Observer <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 */

class IClientMixedStreamObserver {
public:
    /**
     * 
     * @hidden for internal use only on Windows
     * @type callback
     * @brief 转推直播状态回调
     * @param info 参看 MixedStreamTaskInfo{@link #MixedStreamTaskInfo}
     * @param type 转推直播类型，参看 MixedStreamType{@link #MixedStreamType}
     * @param event 转推直播任务状态，参看 MixedStreamTaskEvent{@link #MixedStreamTaskEvent}
     * @param error_code 转推直播错误码，参看 MixedStreamTaskErrorCode{@link #MixedStreamTaskErrorCode}。
     */
    
    virtual void onClientMixedStreamEvent(MixedStreamTaskInfo info, MixedStreamType type, MixedStreamTaskEvent event, MixedStreamTaskErrorCode error_code) = 0;
    /**
     * 
     * @hidden for internal use only on Windows
     * @type callback
     * @region CDN 推流
     * @brief 客户端合流音频首帧回调
     * @param task_id 任务 ID
     */
    
    virtual void onMixedFirstAudioFrame(const char* task_id) = 0;
    /**
     * 
     * @hidden for internal use only on Windows
     * @type callback
     * @region CDN 推流
     * @brief 客户端合流视频首帧回调
     * @param task_id 任务 ID
     */
    
    virtual void onMixedFirstVideoFrame(const char* task_id) = 0;
    /**
     * 
     * @type callback
     * @hidden for internal use only on Windows
     * @region CDN 推流
     * @brief 合流视频回调，运行在视频回调线程
     * @param task_id 合流任务 ID
     * @param video_frame 视频帧，参看 IVideoFrame{@link #IVideoFrame}。
     * @note 收到该回调的周期与视频的帧间隔一致。
     */
    
    virtual void onMixedVideoFrame(const char* task_id, IVideoFrame* video_frame) = 0;

    /**
     * 
     * @type callback
     * @hidden for internal use only on Windows
     * @region CDN 推流
     * @brief 合流音频回调，运行在音频回调线程
     * @param task_id 转推直播任务 ID
     * @param audio_frame 音频帧，参看 IAudioFrame{@link #IAudioFrame}。
     * @note 收到该回调的周期为每 10 毫秒一次，并且每次的音频数据量为 10 毫秒数据量。
     */
    
    virtual void onMixedAudioFrame(const char* task_id, IAudioFrame* audio_frame) = 0;

    /**
     * 
     * @type callback
     * @hidden for internal use only on Windows
     * @region CDN 推流
     * @brief 视频 SEI 帧回调，运行在视频回调线程
     * @param task_id 转推直播任务 ID
     * @param data_frame SEI 数据
     */
    
    virtual void onMixedDataFrame(const char* task_id, IDataFrame* data_frame) = 0;

    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @region CDN 推流
     * @brief 同步视频帧回调。
     * @param task_id 转推直播任务 ID。
     * @param uids 同步视频帧对应的 uid 数组。
     * @param video_frames 同步视频帧数组，与 uids 对应。
     * @param data_frame SEI 数据。
     * @param count 数组的长度。
     */
    
    virtual void onCacheSyncVideoFrames(const char* task_id, const char* uids[], IVideoFrame* video_frames[],
            IDataFrame* data_frame[], int count) = 0;

    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IClientMixedStreamObserver() = default;
};

/**
 * 
 * @hidden internal use only
 * @type callback
 * @brief 缓存同步 Observer
 */

class IChorusCacheSyncObserver {
public:
    /**
     * @hidden constructor/destructor
    */
    virtual ~IChorusCacheSyncObserver() {
    }
    /**
     * 
     * @type callback
     * @brief 调用 startChorusCacheSync{@link #IRTCEngine#startChorusCacheSync}，并设置为 `consumer` 的用户会通过此回调获取经缓存同步后的视频帧。获取频率通过启动同步时的 `fps` 进行设置。
     * @param count `uids` 和 `videoFrames` 的数组长度
     * @param uids 参与合唱缓存同步的 `producer` 和 `retransmitter` 的列表，不包括参与但未发送媒体数据的用户。
     * @param video_frames 对应 `uids` 的视频帧。参看 IVideoFrame{@link #IVideoFrame}。
     */
        
    virtual void onSyncedVideoFrames(int count, const char* uids[], bytertc::IVideoFrame* video_frames[]) = 0;
    /**
     * 
     * @type callback
     * @brief 参与合唱缓存同步的 `producer` 和 `retransmitter` 发生变化时，收到此回调。
     * @param count 当前的 `uids` 的长度
     * @param uids 当前的参与者列表
     * @note 有以下情况可能造成参与者发生变化： <br>
     *        - 用户主动调用 startChorusCacheSync{@link #IRTCEngine#startChorusCacheSync} 或 stopChorusCacheSync{@link #IRTCEngine#stopChorusCacheSync};
     *        - 原本参与缓存同步的用户发生异常退出。
     */
    
    virtual void onSyncedUsersChanged(int count, const char* uids[]) = 0;
    /**
     * 
     * @type callback
     * @brief 缓存同步事件回调
     * @param event 事件，参看 ChorusCacheSyncEvent{@link #ChorusCacheSyncEvent}。
     * @param error 错误码，参看 ChorusCacheSyncError{@link #ChorusCacheSyncError}。
     */
    
    virtual void onSyncEvent(ChorusCacheSyncEvent event, ChorusCacheSyncError error) = 0;
};
}  
