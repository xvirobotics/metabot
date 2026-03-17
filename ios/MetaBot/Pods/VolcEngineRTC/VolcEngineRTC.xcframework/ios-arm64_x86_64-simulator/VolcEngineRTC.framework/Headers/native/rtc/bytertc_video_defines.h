/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Video Defines
 */

#pragma once

#include "bytertc_rts_defines.h"
#include "bytertc_video_frame.h"

namespace bytertc {

/**
 * 
 * @type keytype
 * @brief 订阅大小流偏好
 */

enum SimulcastStreamType { 
    /**
     * 
     * @brief 弱流，最小分辨率的流。
     */
    
    kSimulcastStreamTypeWeak = 0, 
   /**
     * 
     * @brief 小流
     */
    
    kSimulcastStreamTypeLow, 
    /**
     * 
     * @brief 中流
     */
    
    kSimulcastStreamTypeMid, 
    /**
     * 
     * @brief 大流
     */
    
    kSimulcastStreamTypeHigh 
};

/**
 * 
 * @type keytype
 * @brief 发布大小流模式
 */

enum VideoSimulcastMode {
    /**
     * 
     * @brief 单流模式。始终只有 1 路分辨率的流。
     */
    
    kVideoSimulcastModeOnlyOne = 0,
    /**
     * 
     * @brief 按需订阅模式。发送端会根据订阅端的状态，按需发布。无订阅偏好设置默认发送 2 路。
     */
    
    kVideoSimulcastModeOnDemand,
    /**
     * 
     * @brief 订阅弱流。发送端始终按照设置的参数发布所有大小流。默认发送 2 路。
     */
    
    kVideoSimulcastModeAlwaysSimulcast,
};


/**
 * 
 * @type keytype
 * @brief 渲染模式
 */

enum RenderMode {
    /**
     * 
     * @brief 视窗填满优先，默认值。 <br>
     *        视频帧等比缩放，直至视窗被视频填满。如果视频帧长宽比例与视窗不同，视频帧的多出部分将无法显示。 <br>
     *        缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长大于等于视窗对应边长。
     */
    
    kRenderModeHidden = 1,
    /**
     * 
     * @brief 视频帧内容全部显示优先。 <br>
     *        视频帧等比缩放，直至视频帧能够在视窗上全部显示。如果视频帧长宽比例与视窗不同，视窗上未被视频帧填满区域将被涂黑。 <br>
     *        缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长小于等于视窗对应边长。
     */
    
    kRenderModeFit = 2,
    /**
     * 
     *  @brief 视频帧自适应画布。 <br>
     *         视频帧非等比缩放，直至画布被填满。在此过程中，视频帧的长宽比例可能会发生变化。
     */
   
    kRenderModeFill = 3,
};

/**
 * 
 * @type keytype
 * @brief 订阅媒体的类型
 */

enum SubscribeMediaType {
    /**
     * 
     * @brief 既不订阅音频，也不订阅视频
     */
    
    kRTCSubscribeMediaTypeNone = 0,
    /**
     * 
     * @brief 只订阅音频，不订阅视频
     */
    
    kRTCSubscribeMediaTypeAudioOnly,
    /**
     * 
     * @brief 只订阅视频，不订阅音频
     */
    
    kRTCSubscribeMediaTypeVideoOnly,
    /**
     * 
     * @brief 同时订阅音频和视频
     */
    
    kRTCSubscribeMediaTypeVideoAndAudio
};

/**
 * 
 * @type keytype
 * @brief 暂停/恢复接收远端的媒体流类型。
 */

enum PauseResumeControlMediaType {
    /**
     * 
     * @brief 只控制音频，不影响视频
     */
    
    kRTCPauseResumeControlMediaTypeAudio = 0,
    /**
     * 
     * @brief 只控制视频，不影响音频
     */
    
    kRTCPauseResumeControlMediaTypeVideo = 1,
    /**
     * 
     * @brief 同时控制音频和视频
     */
    
    kRTCPauseResumeControlMediaTypeVideoAndAudio = 2
};

/**
 * 
 * @type keytype
 * @brief 时域分层定义
 */

enum SVCLayer {
    /**
    * 
    * @brief 不指定分层（默认值）
    */
   
    kSVCLayerDefault = 0,
    /**
     * 
     * @brief T0 层
     */
    
    kSVCLayerBase = 1,
    /**
     * 
     * @brief T0+T1 层
     */
    
    kSVCLayerMain = 2,
    /**
     * 
     * @brief T0+T1+T2 层
     */
    
    kSVCLayerHigh = 3,
};

/**
 * 
 * @type keytype
 * @brief 流切换信息。本地用户订阅的远端流触发回退策略时的流切换信息。
 */

struct RemoteStreamSwitch {
    /**
     * 
     * @brief 订阅的音视频流的发布者的用户 ID。
     */
    
    const char* uid;
    /**
     * 
     * @brief 是否是屏幕流。
     */
    
    bool is_screen;
    /**
     * 
     * @brief 流切换前本地用户订阅的视频流的分辨率对应的索引。
     */
    
    int before_video_index;
    /**
     * 
     * @brief 流切换后本地用户订阅的视频流的分辨率对应的索引。
     */
    
    int after_video_index;
    /**
     * 
     * @brief 流切换前是否有视频流。
     */
    
    bool before_enable;
    /**
     * 
     * @brief 流切换后是否有视频流。
     */
    
    bool after_enable;
    /**
     * 
     * @brief 触发流回退的原因，详见类型 FallbackOrRecoverReason{@link #FallbackOrRecoverReason} 。
     */
    
    FallbackOrRecoverReason reason;
};

/**
 * 
 * @type keytype
 * @brief 背景模式
 */

enum BackgroundMode {
    /**
     * 
     * @brief 无
     */
    
    kBackgroundModeNone = 0,
    /**
     * 
     * @brief 虚化
     */
    
    kBackgroundModeBlur = 1,
    /**
     * 
     * @brief 背景 1
     */
    
    kBackgroundModeA = 2,
    /**
     * 
     * @brief 背景 2
     */
    
    kBackgroundModeB = 3,
};

/**
 * 
 * @type keytype
 * @brief 分割模型
 */

enum DivideMode {
    /**
     * 
     * @brief 自研
     */
    
    kDivideModeNone = 0,
    /**
     * 
     * @brief effect
     */
    
    kDivideModeEffect = 1,
};

/**
 * 
 * @type keytype
 * @brief 流属性。
 */

struct MediaStreamInfo {
    /**
     * 
     * @brief 发布此流的用户 ID 。
     */
    
    const char* user_id;
    /**
     * 
     * @hidden for internal use only
     * @brief 该流的标识 ID，用户内唯一。
     */
    
    StreamIndex stream_index;
    /**
     * 
     * @brief 此流是否为共享屏幕流。
     */
    
    bool is_screen;
    /**
     * 
     * @brief 此流是否包括视频流。
     */
    
    bool has_video;
    /**
     * 
     * @brief 流是否包括音频流。
     */
    
    bool has_audio;
    /**
     * 
     * @brief 视频流的属性。 <br>
     *        当远端用户调用 setVideoEncoderConfig{@link #IRTCEngine#setVideoEncoderConfig} 方法发布多个配置的视频流时，此处会包含该用户发布的所有视频流的属性信息。     
     */
    
    VideoSolutionDescription* profiles;
    /**
     * 
     * @brief 不同配置流的个数。 <br>
     *        当远端用户调用 setVideoEncoderConfig{@link #IRTCEngine#setVideoEncoderConfig} 方法发布多个配置的视频流时，此处会包含该用户发布的视频流的数目。
     */
    
    int profile_count;

    /**
     * 
     * @brief 最大视频流的属性。 <br>
     *        当远端用户开启带通道划分的大小流时，回调最大视频流属性信息。用户可以使用新的订阅方式，订阅任意不超过该分辨率的流。 <br>
     *        最终接收到的流，会受房间内其他用户的订阅行为的影响，综合确定一个最接近用户请求的分辨率的流。
     */
    
    VideoSolutionDescription max_profile;
};

/**
 * 
 * @type keytype
 * @brief 视频订阅配置信息
 */

struct SubscribeVideoConfig {
    /**
     * 
     * @brief 订阅的视频流分辨率下标。 <br>
     *        当远端用户通过调用 setVideoEncoderConfig{@link #IRTCEngine#setVideoEncoderConfig} 方法启动发布多路不同分辨率的视频流时，本地用户需通过此参数指定希望订阅的流。 <br>
     *        默认值为 0，即订阅第一路流。 <br>
     *        如果不想更改之前的设置，可以输入 -1。
     */
    
    int video_index = 0;
    /**
     * 
     * @brief 远端用户优先级，参看 RemoteUserPriority{@link #RemoteUserPriority}，默认值为 0。
     */
    
    int priority = 0;
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    SubscribeVideoConfig() : video_index(0), priority(0) {
    }
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool operator==(const SubscribeVideoConfig& config) const {
        bool result = video_index == config.video_index && priority == config.priority;
        return result;
    }
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool operator!=(const SubscribeVideoConfig& config) const {
        bool result = (*this == config);
        return !result;
    }
};

/**
 * 
 * @type keytype
 * @brief 手动订阅流的配置信息。
 */

struct SubscribeConfig {
    /**
     * 
     * @brief 是否是屏幕流（默认为否）。
     */
    
    bool is_screen = false;
    /**
     * 
     * @brief 是否订阅视频。
     */
    
    bool sub_video;
    /**
     * 
     * @brief 是否订阅音频。
     */
    
    bool sub_audio;
    /**
     * 
     * @brief 订阅的视频流分辨率下标。 <br>
     *        用户可以通过调用 setVideoEncoderConfig{@link #IRTCEngine#setVideoEncoderConfig} 方法在一路流中发布多个不同分辨率的视频。因此订阅流时，需要指定订阅的具体分辨率。此参数即用于指定需订阅的分辨率的下标，默认值为 0 。
     */
    
    int video_index;
    /**
     * 
     * @brief 远端用户的需求优先级，参看 RemoteUserPriority{@link #RemoteUserPriority}，默认值为 0 。 <br>
     *        当开启了订阅流回退选项功能（详见 setSubscribeFallbackOption{@link #IRTCEngine#setSubscribeFallbackOption} 方法），弱网或性能不足时会优先保证收到的高优先级用户的流的质量。
     */
    
    int priority = 0;
    /**
     * 
     * @brief 远端用户的时域分层，参看 SVCLayer{@link #SVCLayer}，默认值为 0 。 <br>
     *        仅码流支持 SVC 特性时可以生效。
     */
    
    int svc_layer = 0;
    /**
     * 
     * @brief 期望订阅的最高帧率，单位：fps，默认值为 0，设为大于 0 的值时开始生效。 <br>
     *        如果发布端发布帧率 > 订阅端订阅的帧率，下行媒体服务器 SVC 丢帧，订阅端收到通过此接口设置的帧率；如果发布端发布帧率 < 订阅端订阅的帧率，则订阅端只能收到发布的帧率。<br>
     *        仅码流支持 SVC 分级编码特性时方可生效。
     */
    
    int framerate = 0;
    /**
     * 
     * @brief 用户通过指定 UI 对应的最合适的流的宽度，单位：px
     */
    
    int sub_width = 0;
    /**
     * 
     * @brief 用户通过指定 UI 对应的最合适的流的高度，单位：px
     */
    
    int sub_height = 0;
    /**
     * 
     * @hidden for internal use only
     */
    
    int sub_video_index = -1;

public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    SubscribeConfig() : is_screen(false), sub_video(true), sub_audio(true), video_index(0), priority(0) {
    }

    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    SubscribeConfig(bool is_screen, bool subvideo, bool subaudio, int videoindex)
            : is_screen(is_screen), sub_video(subvideo), sub_audio(subaudio), video_index(videoindex) {
    }

    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    SubscribeConfig(bool is_screen, bool subvideo, bool subaudio, int videoindex, int priority)
            : is_screen(is_screen),
              sub_video(subvideo),
              sub_audio(subaudio),
              video_index(videoindex),
              priority(priority) {
    }

    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    SubscribeConfig(bool is_screen, bool subvideo, bool subaudio, int videoindex,
     int priority,int svc_layer,int width,int height)
            : is_screen(is_screen),
              sub_video(subvideo),
              sub_audio(subaudio),
              video_index(videoindex),
              priority(priority),
              svc_layer(svc_layer),
              sub_width(width),
              sub_height(height) {
    }
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool operator==(const SubscribeConfig& config) const {
        
        bool common_result = is_screen == config.is_screen && sub_video == config.sub_video
                             && sub_audio == config.sub_audio && priority == config.priority
                             && svc_layer == config.svc_layer && framerate == config.framerate;
        bool result;
        if(sub_width * sub_height > 0 && config.sub_width * config.sub_height > 0) {
          result = common_result && sub_width == config.sub_width && sub_height == config.sub_height ;
        }  else if((sub_width * sub_height == 0) && (config.sub_width * config.sub_height == 0) ) {
          result = common_result && video_index == config.video_index;
        } else {
          result = false;
        }
        return result;
    }
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool operator!=(const SubscribeConfig& config) const {
        bool result = (*this == config);
        return !result;
    }
};

/**
 * 
 * @type keytype
 * @brief 远端视频类型 <br>
 *      目前 C++ sdk 只支持 kVideoStreamTypeHigh 类型
 */

enum VideoStreamType {
    /**
     * 
     * @brief 高分辨率视频流
     */
    
    kVideoStreamTypeHigh = 0,
    /**
     * 
     * @brief 低分辨率视频
     */
    
    kVideoStreamTypeLow = 1,
};

/**
 * 
 * @type keytype
 * @brief 渲染目标类型
 */

enum RenderTargetType {
    /**
     * 
     * @brief 指定渲染目标类型为 SurfaceView/TextureView (Android), UIView(iOS), NSView(macOS), HWND (Windows)
     */
    
    kRenderTargetTypeView = 0,
    /**
     * 
     * @brief 仅在 Android 平台生效，指定渲染目标类型为 Surface
     */
    
    kRenderTargetTypeSurface = 1,
};

/**
 * 
 * @type keytype
 * @brief 视频帧渲染设置。
 */

struct VideoCanvas {
    /**
     * 
     * @brief 本地视图句柄
     */
    
    void* view = NULL;
    /**
     * 
     * @brief 视频渲染模式，参看 RenderMode{@link #RenderMode}
     */
    
    int render_mode = kRenderModeHidden;
    /**
     * 
     * @brief 用于填充画布空白部分的背景颜色。取值范围是 `[0x00000000, 0xFFFFFFFF]`。默认值是 `0x00000000`。其中，透明度设置无效。
     */
    
    uint32_t background_color = 0;
    /**
     * 
     * @brief 渲染目标的类型。参看 RenderTargetType{@link #RenderTargetType}。
     */
    
    RenderTargetType render_target_type = kRenderTargetTypeView;
    /**
     * 
     * @brief 视频帧旋转角度。参看 VideoRotation{@link #VideoRotation}。默认为 0 度，即不做旋转处理。 <br>
     *        该设置仅对远端视频有效，对本地视频设置不生效。
     */
    
     VideoRotation render_rotation = kVideoRotation0;
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    VideoCanvas() : view(NULL), render_mode(kRenderModeHidden), background_color(0), render_target_type(kRenderTargetTypeView) {
    }
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    VideoCanvas(void* v, int m, uint32_t c) : view(v), render_mode(m), background_color(c) {
    }
};

/**
 * 
 * @type keytype
 * @brief 远端视频帧渲染设置
 */

struct RemoteVideoRenderConfig {
    /**
     * 
     * @brief 渲染模式： <br>
     *        - 1（`RENDER_MODE_HIDDEN`）视窗填满优先，默认值。视频帧等比缩放，直至视窗被视频填满。如果视频帧长宽比例与视窗不同，视频帧的多出部分将无法显示。缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长大于等于视窗对应边长。
     *        - 2（`RENDER_MODE_FIT`）视频帧内容全部显示优先。视频帧等比缩放，直至视频帧能够在视窗上全部显示。如果视频帧长宽比例与视窗不同，视窗上未被视频帧填满区域将填充 `backgroundColor`。缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长小于等于视窗对应边长。
     *        - 3（`RENDER_MODE_FILL`）视频帧自适应画布。视频帧非等比缩放，直至画布被填满。在此过程中，视频帧的长宽比例可能会发生变化。
     */
    
    RenderMode render_mode = kRenderModeHidden;
    /**
     * 
     * @brief 用于填充画布空白部分的背景颜色。取值范围是 `[0x00000000, 0xFFFFFFFF]`。默认值是 `0x00000000`。其中，透明度设置无效。
     */
    
    uint32_t background_color = 0;

    /**
     * 
     * @brief 用户自定义画面旋转。可选值 0, 90, 180, 270， 默认值是 0。其中 0 代表保持原始画面，90, 180, 270 代表顺时针旋转原始 canvas 相应角度。
     */
    
    VideoRotation render_rotation = kVideoRotation0;
};

/**
 * 
 * @type api
 * @brief Windows 窗口 id, Windows 平台下传 Windows 窗口句柄
 */

typedef void* view_t;

/**
 * 
 * @type keytype
 * @brief 矩形区域，用于指定屏幕区域。
 */

struct Rectangle {
    /**
     * 
     * @brief 矩形区域左上角的 x 坐标。
     */
    
    int x = 0;
    /**
     * 
     * @brief 矩形区域左上角的 y 坐标。
     */
    
    int y = 0;
    /**
     * 
     * @brief 矩形宽度。
     */
    
    int width = 0;
    /**
     * 
     * @brief 矩形高度。
     */
    
    int height = 0;
};

/**
 * 
 * @type keytype
 * @brief 屏幕共享时的边框高亮设置。
 */

struct HighlightConfig {
    /**
     * 
     * @brief 是否显示高亮边框，默认显示。
     */
    
    bool enable_highlight = true;
    /**
     * 
     * @brief 边框的颜色, 颜色格式为十六进制 ARGB:  0xAARRGGB。
     */
    
    uint32_t border_color = 0xFF29CCA3;
    /**
     * 
     * @brief 边框的宽度，单位：像素。
     */
    
    int border_width = 4;
};

/**
 * 
 * @type keytype
 * @brief 抓取屏幕时排除指定窗口，默认不排除任何窗体。
 */

struct ScreenFilterConfig {
    /**
     * 
     * @brief 抓取屏幕时排除窗口列表。这个参数仅在抓取屏幕时生效。
     */
    
    view_t* excluded_window_list = nullptr;
   /**
     * 
     * @brief 排除窗口的数量。
     */
    
    int excluded_window_num = 0;
};

/**
 * 
 * @type keytype
 * @region 屏幕共享
 * @brief 内部采集屏幕视频流时，是否采集鼠标信息。
 */

enum MouseCursorCaptureState {
    /**
     * 
     * @brief 采集鼠标信息。
     */
    
    kMouseCursorCaptureStateOn,
    /**
     * 
     * @brief 不采集鼠标信息。
     */
    
    kMouseCursorCaptureStateOff,
};

/**
 * 
 * @type keytype
 * @brief 屏幕共享参数
 */

struct ScreenParameters {
    /**
     * 
     * @brief 编码帧率,单位为 fps
     */
    
    int frame_rate = 15;
    /**
     * 
     * @brief 屏幕采集编码码率, `-1` 为自动码率, SDK 会根据宽高信息选择合适的码率，单位 kbps
     */
    
    int kbitrate = -1;
    /**
     * 
     * @brief 视频最小编码码率, 单位 kbps。编码码率不会低于 `min_kbitrate`。 <br>
     *        默认值为 `0`。 <br>
     *        范围：[0, kbitrate)，当 `kbitrate` < `min_kbitrate` 时，为适配码率模式。 <br>
     *        以下情况，设置本参数无效： <br>
     *        - 当 `kbitrate` 为 `0` 时，不对视频流进行编码发送。
     *        - 当 `kbitrate` < `0` 时，适配码率模式。
     */
    
    int min_kbitrate = 0;
};

/**
 * 
 * @type keytype
 * @region 屏幕共享
 * @brief 内部采集屏幕视频流的内容类型。
 */

enum ContentHint {
    /**
     * 
     * @brief 细节内容。当共享文档、图片时，建议使用该内容类型。默认值。
     */
    
    kContentHintDetails = 0,
    /**
     * 
     * @brief 动画内容。当共享视频、游戏时，建议使用该内容类型。
     */
    
    kContentHintMotion,
};

/**
 * 
 * @type keytype
 * @brief 屏幕共享内部采集参数
 */

struct ScreenCaptureParameters {
    /**
     * 
     * @brief 采集区域，参看 Rectangle{@link #Rectangle}。
     */
    
    Rectangle region_rect;
    /**
     * 
     * @brief 是否采集鼠标状态，参看 MouseCursorCaptureState{@link #MouseCursorCaptureState}。
     */
    
    MouseCursorCaptureState capture_mouse_cursor = MouseCursorCaptureState::kMouseCursorCaptureStateOn;
    /**
     * 
     * @brief 屏幕过滤设置，填写不需要采集的窗口 ID，参看 ScreenFilterConfig{@link #ScreenFilterConfig}。
     */
    
    ScreenFilterConfig filter_config;
    /**
     * 
     * @brief 采集区域的边框高亮设置，参看 HighlightConfig{@link #HighlightConfig}。
     */
    
    HighlightConfig highlight_config;
};

/**
 * 
 * @type keytype
 * @brief 屏幕共享的编码参数配置。
 */

struct DesktopCaptureParameters {
    /**
     * 
     * @brief 屏幕采集编码最大宽度，单位 px。
     */
    
    int max_width = 1920;
    /**
     * 
     * @brief 屏幕采集编码最大高度，单位 px。
     */
    
    int max_height = 1080;
    /**
     * 
     * @brief 屏幕采集编码帧率, 单位 fps。
     */
    
    int frame_rate = 15;
    /**
     * 
     * @brief 屏幕采集编码码率, -1 为自动码率, SDK 会根据宽高信息选择合适的码率，单位 kbps
     */
    
    int bitrate = -1;
    /**
     * 
     * @brief 最小编码码率，使用 SDK 内部采集时可选设置，自定义采集时必须设置，单位：kbps。 <br>
     *        最小编码码率必须小于或等于最大编码，否则不对视频流进行编码发送。
     */
    
    int min_bitrate = 0;
    /**
     * 
     * @brief 是否捕获鼠标光标。
     */
    
    bool capture_mouse_cursor = true;
    /**
     * 
     * @brief 抓取屏幕时排除窗口列表。这个参数仅在抓取屏幕时生效。
     */
    
    view_t* excluded_window_list = nullptr;
    /**
     * 
     * @brief 抓取屏幕时排除的窗口数量。
     */
    
    int excluded_window_num = 0;
    /**
     * 
     * @brief 高亮边框的配置。
     */
    
    HighlightConfig highlight_config;
};

/**
 * 
 * @type keytype
 * @brief 自定义视频渲染器
 */

class IVideoSink {
public:
    /**
     * 
     * @type keytype
     * @brief 视频帧编码格式
     */
    
    enum PixelFormat {
        /**
         * 
         * @brief YUV I420 格式
         */
        
        kI420 = VideoPixelFormat::kVideoPixelFormatI420,
        /**
         * 
         * @brief RGBA 格式, 字节序为 R8 G8 B8 A8
         */
        
        kRGBA = VideoPixelFormat::kVideoPixelFormatRGBA,
        /**
         * 
         * @brief 原始视频帧格式
         */
        
		kOriginal = VideoPixelFormat::kVideoPixelFormatUnknown,
    };
    /**
     * 
     * @type callback
     * @brief 视频帧回调
     * @param video_frame 视频帧结构类，参看 IVideoFrame{@link #IVideoFrame}
     * @return 返回值暂未使用
     * @list 
     */
    
    virtual bool onFrame(IVideoFrame* video_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 获取外部渲染耗时。
     * @note 获取外部渲染耗时进行上报。开发者需要自己计算平均渲染耗时。
     * @list 房间管理
     */
    
    virtual int getRenderElapse() = 0;
    /**
     * 
     * @type callback
     * @brief 释放渲染器。
     * @note 通知开发者渲染器即将被废弃。收到该返回通知后即可释放资源。
     * @list Room Management
     */
    
    virtual void release() {
    }
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IVideoSink() = default;

    /**
     * 
     * @hidden sink id
     * @brief sink id
     */
    
    virtual void* uniqueId() const { return (void *)this; }
};

/**
 * 
 * @type keytype
 * @brief 本地视频帧回调位置。
 */

enum LocalVideoSinkPosition {
    /**
     * 
     * @brief 采集后。
     */
    
    kLocalVideoSinkPositionAfterCapture,
    /**
     * 
     * @brief （默认值）前处理后。
     */
    
    kLocalVideoSinkPositionAfterPreProcess
};

/**
 * 
 * @type keytype
 * @brief 是否将视频帧自动转正。
 */

enum VideoApplyRotation {
    /**
     * 
     * @brief （默认值）不旋转。
     */
    
    kVideoApplyRotationDefault = -1,
    /**
     * 
     * @brief 自动转正视频，即根据视频帧的旋转角信息将视频帧旋转到 0 度。
     */
    
    kVideoApplyRotation0 = 0,
};

/**
 * 
 * @type keytype
 * @brief 是否将视频帧镜像。
 */

enum VideoSinkMirrorType {
    /**
     * 
     * @brief 开启镜像。
     */
    
    kVideoSinkMirrorTypeON = 1,
    /**
     * 
     * @brief （默认值）不开启镜像。
     */
    
    kVideoSinkMirrorTypeOFF = 2,
};

/**
 * 
 * @type keytype
 * @brief 本地视频帧回调配置。
 */

struct LocalVideoSinkConfig {
    /**
     * 
     * @brief 本地视频帧回调位置，参看 LocalVideoSinkPosition{@link #LocalVideoSinkPosition}，默认回调前处理后的视频帧。
     */
    
    LocalVideoSinkPosition position = kLocalVideoSinkPositionAfterPreProcess;
    /**
     * 
     * @brief 本地视频帧回调格式，参看 VideoPixelFormat{@link #VideoPixelFormat}，默认值为 0。
     */
    
    VideoPixelFormat pixel_format = kVideoPixelFormatUnknown;
};

/**
 * 
 * @type keytype
 * @brief 远端视频帧回调位置。
 */

enum RemoteVideoSinkPosition {
    /**
     * 
     * @hidden not available
     * @brief 解码后。
     */
    
    kRemoteVideoSinkPositionAfterDecoder,
    /**
     * 
     * @brief （默认值）后处理后。
     */
    
    kRemoteVideoSinkPositionAfterPostProcess
};

/**
 * 
 * @type keytype
 * @brief 远端视频帧回调配置。
 */

struct RemoteVideoSinkConfig {
    /**
     * 
     * @brief 远端视频帧回调位置，参看 RemoteVideoSinkPosition{@link #RemoteVideoSinkPosition}，默认回调后处理后的视频帧。
     */
    
    RemoteVideoSinkPosition position = kRemoteVideoSinkPositionAfterPostProcess;
    /**
     * 
     * @brief 远端视频帧回调格式，参看 VideoPixelFormat{@link #VideoPixelFormat}，默认值为 0。
     */
    
    VideoPixelFormat pixel_format = kVideoPixelFormatUnknown;
    /**
     * 
     * @brief 是否将视频帧自动转正，参看 VideoApplyRotation{@link #VideoApplyRotation}，默认为不旋转。
     */
    
    VideoApplyRotation apply_rotation = kVideoApplyRotationDefault;
    /**
     * 
     * @brief 是否将视频帧镜像。参看 VideoSinkMirrorType{@link #VideoSinkMirrorType}，默认为不镜像。 <br>
     *        本设置与 setRemoteVideoMirrorType{@link #IRTCEngine#setRemoteVideoMirrorType} （适用于内部渲染）相互独立。
     */
    
    VideoSinkMirrorType mirror_type = kVideoSinkMirrorTypeOFF;
};

/**
 * 
 * @hidden
 * @type keytype
 * @deprecated since 3.29 and will be deleted in 3.51, use MirrorType{@link #MirrorType} instead.
 * @brief 是否开启镜像模式
 */

enum MirrorMode {
    /**
     * 
     * @brief 不开启
     */
    
    kMirrorModeOff = 0,
    /**
     * 
     * @brief 开启
     */
    
    kMirrorModeOn = 1,
};

/**
 * 
 * @type keytype
 * @brief 镜像类型
 */

enum MirrorType : int {
    /**
     * 
     * @brief 本地预览和编码传输时均无镜像效果
     */
    
    kMirrorTypeNone = 0,
    /**
     * 
     * @brief 本地预览时有镜像效果，编码传输时无镜像效果
     */
    
    kMirrorTypeRender = 1,
    /**
     * 
     * @brief 本地预览时无镜像效果，仅编码传输时有镜像效果
     */
    
    kMirrorTypeEncoder = 2,
    /**
     * 
     * @brief 本地预览和编码传输时均有镜像效果
     */
    
    kMirrorTypeRenderAndEncoder = 3,
};


/**
 * 
 * @type keytype
 * @brief 远端流的镜像类型。
 */

enum RemoteMirrorType {
    /**
     * 
     * @brief （默认值）远端视频渲染无镜像效果。
     */
    
    kRemoteMirrorTypeNone = 0,
    /**
     * 
     * @brief 远端视频渲染有镜像效果。
     */
    
    kRemoteMirrorTypeRender = 1,
};

/**
 * 
 * @type keytype
 * @brief 弱光适应类型
 */

enum VideoEnhancementMode {
    /**
     * 
     * @brief 关闭弱光适应
     */
    
    kVideoEnhancementModeDisabled = 0,
    /**
     * 
     * @brief 开启弱光适应
     */
    
    kVideoEnhancementModeAuto = 1,
};

/**
 * 
 * @type keytype
 * @brief 视频输入源类型
 */

enum VideoSourceType {
    /**
     * 
     * @brief 自定义采集视频源
     */
    
    kVideoSourceTypeExternal = 0,
    /**
     * 
     * @brief 内部采集视频源
     */
    
    kVideoSourceTypeInternal = 1,
    /**
     * 
     * @brief 自定义编码视频源。 <br>
     *        你仅需推送分辨率最大的一路编码后视频流，SDK 将自动转码生成多路小流
     */
    
    kVideoSourceTypeEncodedWithAutoSimulcast = 2,
    /**
     * 
     * @brief 自定义编码视频源。 <br>
     *        SDK 不会自动生成多路流，你需要自行生成并推送多路流
     */
    
    kVideoSourceTypeEncodedWithoutAutoSimulcast = 3,
};

/**
 * 
 * @type keytype
 * @brief Alpha 通道相对于 RGB 通道数据的排列位置。
 */
 
enum AlphaLayout {
    /**
     * 
     * @brief Alpha 数据置于 RGB 数据上方。
     */
    
    kAlphaLayoutTop = 0,
    /**
     * 
     * @hidden currently not available
     * @brief Alpha 数据置于 RGB 数据下方。
     */
    
    kAlphaLayoutBottom = 1,
    /**
     * 
     * @hidden currently not available
     * @brief Alpha 数据置于 RGB 数据左方。
     */
    
    kAlphaLayoutLeft = 2,
    /**
     * 
     * @hidden currently not available
     * @brief Alpha 数据置于 RGB 数据右方。
     */
     
    kAlphaLayoutRight = 3,
};

/**
 * 
 * @type keytype
 * @brief 数码变焦参数类型
 */

enum ZoomConfigType {
    /**
     * 
     * @brief 设置缩放系数
     */
    
    kZoomConfigTypeFocusOffset = 0, 
    /**
     * 
     * @brief 设置移动步长
     */
    
    kZoomConfigTypeMoveOffset,
};

/**
 * 
 * @type keytype
 * @brief 数码变焦操作类型
 */

enum ZoomDirectionType {
    /**
     * 
     * @brief 相机向左移动
     */
    
    kZoomDirectionTypeMoveLeft = 0,
    /**
     * 
     * @brief 相机向右移动
     */
    
    kZoomDirectionTypeMoveRight,
    /**
     * 
     * @brief 相机向上移动
     */
    
    kZoomDirectionTypeMoveUp,
    /**
     * 
     * @brief 相机向下移动
     */
    
    kZoomDirectionTypeMoveDown,
    /**
     * 
     * @brief 相机缩小焦距
     */
    
    kZoomDirectionTypeZoomOut,
    /**
     * 
     * @brief 相机放大焦距
     */
    
    kZoomDirectionTypeZoomIn,
    /**
     * 
     * @brief 恢复到原始画面
     */
    
    kZoomDirectionTypeReset,
};

/**
 * 
 * @type keytype
 * @brief 视频帧信息
 */

struct VideoFrameInfo {
    /**
     * 
     * @brief 宽（像素）
     */
    
    int width = 0;
    /**
     * 
     * @brief 高（像素）
     */
    
    int height = 0;
    /**
     * 
     * @brief 视频帧顺时针旋转角度。参看 VideoRotation{@link #VideoRotation}。
     */
    
    VideoRotation rotation = VideoRotation::kVideoRotation0;
};

/**
 * 
 * @type keytype
 * @brief 视频前处理配置参数。
 */

struct VideoPreprocessorConfig {
    /**
     * 
     * @brief 视频帧像素格式，参看 VideoPixelFormat{@link #VideoPixelFormat} <br>
     *        当前仅支持 `kVideoPixelFormatI420` 和 `kVideoPixelFormatUnknown` 格式。
     */
    
    VideoPixelFormat required_pixel_format = kVideoPixelFormatUnknown;
};

/**
 * 
 * @type keytype
 * @brief 自定义编码流信息
 */

struct VideoRateInfo {
    /**
     * 
     * @brief 帧率，单位 fps
     */
    
    int fps = 0;
    /**
     * 
     * @brief 码率，单位 kbps
     */
    
    int bitrate_kbps = 0;
    /**
     * 
     * @brief 视频最小编码码率, 单位 kbps。编码码率不会低于 `min_bitrate_kbps`。 <br>
     *        默认值为 `0`。 <br>
     *        范围：[0, bitrate_kbps)，当 `bitrate_kbps` < `min_bitrate_kbps` 时，为适配码率模式。 <br>
     *        以下情况，设置本参数无效： <br>
     *        - 当 `bitrate_kbps` 为 `0` 时，不对视频流进行编码发送。
     *        - 当 `bitrate_kbps` < `0` 时，适配码率模式。
     */
    
    int min_bitrate_kbps = 0;
};

/**
 * 
 * @type keytype
 * @brief 视频解码方式
 */

enum VideoDecoderConfig {
    /**
     * 
     * @brief 开启 SDK 内部解码，只回调解码后的数据。回调为 onFrame{@link #IVideoSink#onFrame}。
     */
    
    kVideoDecoderConfigRaw,
    /**
     * 
     * @brief 开启自定义解码，只回调解码前数据。回调为 onRemoteEncodedVideoFrame{@link #IRemoteEncodedVideoFrameObserver#onRemoteEncodedVideoFrame}。
     */
    
    kVideoDecoderConfigEncode,
    /**
     * 
     * @brief 开启 SDK 内部解码，同时回调解码前和解码后的数据。回调为 onFrame{@link #IVideoSink#onFrame} 和 onRemoteEncodedVideoFrame{@link #IRemoteEncodedVideoFrameObserver#onRemoteEncodedVideoFrame}。
     */
    
    kVideoDecoderConfigBoth,
};

/**
 * 
 * @type keytype
 * @brief 视频采集配置参数。
 */

struct VideoCaptureConfig {
   /**
    * 
    * @type keytype
    * @brief 视频采集模式
    */
   
    enum CapturePreference {
       /**
        * 
        * @brief （默认）自动设置采集参数。 <br>
        *        SDK 在开启采集时根据服务端下发的采集配置结合编码参数设置最佳采集参数。
        */
       
        kAuto = 0,
       /**
        * 
        * @brief 手动设置采集参数，包括采集分辨率、帧率。
        */
       
        kManual = 1,
       /**
        * 
        * @brief 采集参数与编码参数一致，即在 setVideoEncoderConfig{@link #IRTCEngine#setVideoEncoderConfig} 中设置的参数。
        */
       
        kAutoPerformance = 2,
    };
    /**
     * 
     * @brief 视频采集模式，参看 CapturePreference{@link #CapturePreference}.
     */
    
    CapturePreference capture_preference = CapturePreference::kAuto;

    /**
     * 
     * @brief 视频采集分辨率的宽度，单位：px。
     */
    
    int width = 0;
    /**
     * 
     * @brief 视频采集分辨率的高度，单位：px。
     */
    
    int height = 0;
    /**
     * 
     * @brief 视频采集帧率，单位：fps。
    */
   
    int frame_rate = 0;
};

/**
 * 
 * @type keytype
 * @brief 本地录制的媒体类型
 */

enum RecordingType {
    /**
     * 
     * @brief 只录制音频
     */
    
    kRecordingTypeAudioOnly = 0,
    /**
     * 
     * @brief 只录制视频
     */
    
    kRecordingTypeVideoOnly = 1,
    /**
     * 
     * @brief 同时录制音频和视频
     */
    
    kRecordingTypeVideoAndAudio = 2,
};

/**
 * 
 * @type keytype
 * @brief 当前视频设备类型
 */

enum RTCVideoDeviceType {
    /**
     * 
     * @brief 未知设备类型
     */
    
    kRTCVideoDeviceTypeUnknown = -1,
    /**
     * 
     * @brief 视频渲染设备类型
     */
    
    kRTCVideoDeviceTypeRenderDevice = 0,
    /**
     * 
     * @brief 视频采集设备类型
     */
    
    kRTCVideoDeviceTypeCaptureDevice = 1,
    /**
     * 
     * @brief 屏幕流视频设备
     */
    
    kRTCVideoDeviceTypeScreenCaptureDevice = 2,
};

/**
 * 
 * @deprecated since 3.60, see onMixedStreamEvent{@link #IRTCEngineEventHandler#onMixedStreamEvent}.
 * @type errorcode
 * @brief WTN 流状态码
 */

enum PublicStreamErrorCode {
    /**
     * 
     * @brief 发布或订阅成功。
     */
    
    kPublicStreamErrorCodeOK = 0,
    /**
     * 
     * @brief 订阅失败，发布端未开始发布流。
     */
    
    kPublicStreamErrorCodePullNoPushStream = 1300,
};

/**
 * 
 * @type keytype
 * @brief 视频旋转模式
 */

enum VideoRotationMode {
    /**
     * 
     * @brief 跟随 App 界面方向
     */
    
    kVideoRotationModeFollowApp = 0,
    /**
     * 
     * @brief 跟随设备重力方向
     */
    
    kVideoRotationModeFollowGSensor = 1,
};

/**
 * 
 * @type keytype
 * @brief 媒体流类型
 */

enum MediaStreamType {
    /**
     * 
     * @brief 只控制音频
     */
    
    kMediaStreamTypeAudio = 1 << 0,
    /**
     * 
     * @brief 只控制视频
     */
    
    kMediaStreamTypeVideo = 1 << 1,
    /**
     * 
     * @brief 同时控制音频和视频
     */
    
    kMediaStreamTypeBoth = kMediaStreamTypeAudio | kMediaStreamTypeVideo
};
/**
 * 
 * @type keytype
 * @brief 占发布端原始帧率的比例
 */

enum FrameRateRatio {
  /**
   * 
   * @brief 100%
   */
  
  kFrameRateRatioOriginal = 0,
  /**
   * 
   * @brief 50%
   */
  
  kFrameRateRatioHalf = 1,
  /**
   * 
   * @brief 25%
   */
  
  kFrameRateRatioQuarter = 2,
};
/**
 * 
 * @type keytype
 * @brief 远端视频帧信息
 */

struct RemoteVideoConfig {
    /**
     * 
     * @brief 期望订阅的最高帧率，单位：fps，默认值为 0 即满帧订阅，设为大于 0 的值时开始生效。 <br>
     *        如果发布端发布帧率 > 订阅端订阅的帧率，下行媒体服务器 SVC 丢帧，订阅端收到通过此接口设置的帧率；如果发布端发布帧率 < 订阅端订阅的帧率，则订阅端只能收到发布的帧率。<br>
     *        仅码流支持 SVC 分级编码特性时方可生效。
     */
    
    int framerate = 0;
    /**
     * 
     * @brief 视频宽度，单位：px
     */
    
    int resolution_width = 0;
    /**
     * 
     * @brief 视频高度，单位：px
     */
    
    int resolution_height = 0;
};

/**
 * 
 * @type keytype
 * @brief 房间类型
 */

enum GameRoomType {
    /**
     * 
     * @brief 小队房间。
     */
    
    kGameRoomTypeTeam = 0,
     /**
     * 
     * @brief 世界房间。
     */
    
    kGameRoomTypeWorld = 1
};

/**
 * 
 * @type keytype
 * @brief 游戏场景类型 
 */

enum GameSceneType {
    /**
     * 
     * @brief 普通场景。<br>
     *        同一个小队房间的队友，仅支持在同一个世界房间内通话。
     */
    
    kGameSceneTypeNormal = 0,
    /**
     * 
     * @brief 主题公园场景。<br>
     *        同一个小队房间的队友，支持跨世界房间通话。
     */
    
    kGameSceneTypeThemePark = 1
};

/**
 * 
 * @type keytype
 * @brief 房间参数配置。房间是 RTC 通话的抽象概念。在同一个房间中的用户可以进行音视频通话。
 */

struct GameRoomConfig {
    /**
     * 
     * @type keytype
     * @brief 游戏房间类型。参看 GameRoomType{@link #GameRoomType}，默认为 `kGameRoomTypeTeam`（小队房间），进房后不可更改。
     */
    
    GameRoomType game_room_type = GameRoomType::kGameRoomTypeTeam;
    /**
     * 
     * @type keytype
     * @brief 游戏场景类型。参看 GameSceneType{@link #GameSceneType}，默认为 `kGameSceneTypeNormal`（普通场景），进房后不可更改。 
     */
    
    GameSceneType game_scene_type = GameSceneType::kGameSceneTypeNormal;
};

/**
 * 
 * @type keytype
 * @brief 房间参数配置。房间是 RTC 通话的抽象概念。在同一个房间中的用户可以进行音视频通话。
 */

struct RTCRoomConfig {
    /**
     * 
     * @brief 房间模式，参看 RoomProfileType{@link #RoomProfileType}，默认为 `kRoomProfileTypeCommunication`，进房后不可更改。
     */
    
    RoomProfileType room_profile_type = kRoomProfileTypeCommunication;
    /**
     * 
     * @brief 流 ID。为进房发布的流设置流 ID，可设置为空字符串。若为空，SDK 自动生成 stream_id。
     */
    
    const char* stream_id = nullptr;
    /**
     * 
     * @brief 是否自动发布音频流，默认为自动发布。
     *        + 若调用 `setUserVisibility` 将自身可见性设为 false，无论是默认的自动发布流还是手动设置的自动发布流都不会进行发布，你需要将自身可见性设为 true 后方可发布。 
     *        + 多房间场景下，若已在其中一个房间成功设置了自动发布，其他房间的自动发布设置均不会生效。若每个房间均不做设置，则默认在第一个加入的房间内自动发布流。
     */
    
    bool is_auto_publish_audio = true;
    /**
     * 
     * @brief 是否自动发布视频流，默认为自动发布。 <br>
     *        + 若调用 `setUserVisibility` 将自身可见性设为 false，无论是默认的自动发布流还是手动设置的自动发布流都不会进行发布，你需要将自身可见性设为 true 后方可发布。 
     *        + 多房间场景下，若已在其中一个房间成功设置了自动发布，其他房间的自动发布设置均不会生效。若每个房间均不做设置，则默认在第一个加入的房间内自动发布流。
     */
    
    bool is_auto_publish_video = true;

    /**
     * 
     * @brief 是否自动订阅音频流，默认为自动订阅。 <br>
     *        包含主流和屏幕流。
     */
    
    bool is_auto_subscribe_audio = true;
    /**
     * 
     * @brief 是否自动订阅视频流，默认为自动订阅。 <br>
     *        包含主流和屏幕流。
     */
    
    bool is_auto_subscribe_video = true;
};

/**
 * 
 * @type keytype
 * @brief 屏幕采集的媒体类型
 */

enum ScreenMediaType {
    /**
     * 
     * @brief 仅采集视频
     */
    
    kScreenMediaTypeVideoOnly = 0,
    /**
     * 
     * @brief 仅采集音频
     */
    
    kScreenMediaTypeAudioOnly = 1,
    /**
     * 
     * @brief 同时采集音频和视频
     */
    
    kScreenMediaTypeVideoAndAudio = 2,
};

/**
 * 
 * @type keytype
 * @brief 基础美颜模式。
 */

enum EffectBeautyMode {
    /**
     * 
     * @brief 美白。
     */
     
    kEffectBeautyModeWhite = 0,
    /**
     * 
     * @brief 磨皮。
     */
    
    kEffectBeautyModeSmooth = 1,
    /**
     * 
     * @brief 锐化。
     */
     
    kEffectBeautyModeSharpen = 2,
    /**
     * 
     * @valid since 3.55
     * @brief 清晰，需集成 v4.4.2+ 版本的特效 SDK。
     */
     
    kEffectBeautyModeClear = 3,
};

/**
 * 
 * @type keytype
 * @brief 视频设备朝向类型
 */

enum VideoDeviceFacing {
    /**
     * 
     * @brief 前置摄像头
     */
    
    kVideoDeviceFacingFront = 0,
    /**
     * 
     * @brief 后置摄像头
     */
    
    kVideoDeviceFacingBack = 1,
    /**
     * 
     * @brief 未知类型
     */
    
    kVideoDeviceFacingUnknown = 2,
};

/**
 * 
 * @type keytype
 * @brief 视频设备信息
 */

struct VideoDeviceInfo {
    /**
     * 
     * @brief 设备 ID
     */
    
    char device_id[MAX_DEVICE_ID_LENGTH];
    /**
     * 
     * @brief 设备名称
     */
    
    char device_name[MAX_DEVICE_ID_LENGTH];
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
     * @brief 视频设备朝向类型
     */
    
    VideoDeviceFacing device_facing;
    /**
     * 
     * @hidden constructor/destructor
     */
    
    VideoDeviceInfo() {
        memset(device_id, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_name, 0, MAX_DEVICE_ID_LENGTH);
        this->device_vid = 0;
        this->device_pid = 0;
        this->transport_type = DeviceTransportType::kDeviceTransportTypeUnknown;
        this->device_facing = VideoDeviceFacing::kVideoDeviceFacingFront;
    };
    /**
     * 
     * @hidden constructor/destructor
     */
    
    VideoDeviceInfo& operator=(const VideoDeviceInfo& src) {
        if (this != &src) {
            strncpy(device_id, src.device_id, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_name, src.device_name, MAX_DEVICE_ID_LENGTH - 1);
            device_id[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_name[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_vid = src.device_vid;
            device_pid = src.device_pid;
            transport_type = src.transport_type;
            device_facing = src.device_facing;
        }

        return *this;
    }
};

/**
 * 
 * @type keytype
 * @brief 视频帧朝向
 */

enum VideoOrientation {
    /**
     * 
     * @brief （默认）使用相机输出的原始视频帧的角度，不对视频帧进行额外旋转。
     */
    
    kVideoOrientationAdaptive = 0,
    /**
     * 
     * @brief 固定为竖屏，将相机采集到的视频帧转换为竖屏，在整个 RTC 链路中传递竖屏帧。
     */
    
    kVideoOrientationPortrait = 1,
    /**
     * 
     * @brief 固定为横屏，将相机采集到的视频帧转换为横屏，在整个 RTC 链路中传递横屏帧。
     */
    
    kVideoOrientationLandscape = 2
};

/**
 * 
 * @hidden for internal use only
 * @type keytype
 * @brief 超分状态改变原因。
 */

enum VideoSuperResolutionModeChangedReason {
    /**
     * 
     * @brief 调用 setRemoteVideoSuperResolution{@link #IRTCEngine#setRemoteVideoSuperResolution} 成功关闭超分。
     */
    
    kVideoSuperResolutionModeChangedReasonAPIOff = 0,
    /**
     * 
     * @brief 调用 setRemoteVideoSuperResolution{@link #IRTCEngine#setRemoteVideoSuperResolution} 成功开启超分。
     */
    
    kVideoSuperResolutionModeChangedReasonAPIOn = 1,
    /**
     * 
     * @brief 开启超分失败，远端视频流的原始视频分辨率超过 640 × 360 px。
     */
    
    kVideoSuperResolutionModeChangedReasonResolutionExceed = 2,
    /**
     * 
     * @brief 开启超分失败，已对一路远端流开启超分。
     */
    
    kVideoSuperResolutionModeChangedReasonOverUse = 3,
    /**
     * 
     * @brief 设备不支持使用超分辨率。
     */
    
    kVideoSuperResolutionModeChangedReasonDeviceNotSupport = 4,
    /**
     * 
     * @brief 当前设备性能存在风险，已动态关闭超分。
     */
    
    kVideoSuperResolutionModeChangedReasonDynamicClose = 5,
    /**
     * 
     * @brief 超分因其他原因关闭。
     */
    
    kVideoSuperResolutionModeChangedReasonOtherSettingDisabled = 6,
    /**
     * 
     * @brief 超分因其他原因开启。
     */
    
    kVideoSuperResolutionModeChangedReasonOtherSettingEnabled = 7,
    /**
     * 
     * @brief SDK 没有编译超分组件。
     */
    
    kVideoSuperResolutionModeChangedReasonNoComponent = 8,
    /**
     * 
     * @brief 远端流不存在。房间 ID 或用户 ID 无效，或对方没有发布流。
     */
    
    kVideoSuperResolutionModeChangedReasonStreamNotExist = 9,
};

/**
 * 
 * @hidden for internal use only
 * @type keytype
 * @brief 视频降噪模式状态改变原因。
 */

enum VideoDenoiseModeChangedReason {
    /**
     * 
     * @brief 未知原因导致视频降噪状态改变。
     */
    
    kVideoDenoiseModeChangedReasonNull = -1,
    /**
     * 
     * @brief 通过调用 setVideoDenoiser{@link #IRTCEngine#setVideoDenoiser} 成功关闭视频降噪。
     */
    
    kVideoDenoiseModeChangedReasonApiOff = 0,
    /**
     * 
     * @brief 通过调用 setVideoDenoiser{@link #IRTCEngine#setVideoDenoiser} 成功开启视频降噪。
     */
    
    kVideoDenoiseModeChangedReasonApiOn = 1,
    /**
     * 
     * @brief 后台未配置视频降噪，视频降噪开启失败，请联系技术人员解决。
     */
    
    kVideoDenoiseModeChangedReasonConfigDisabled = 2,
    /**
     * 
     * @brief 后台配置开启了视频降噪。
     */
    
    kVideoDenoiseModeChangedReasonConfigEnabled = 3,
    /**
     * 
     * @brief 由于内部发生了异常，视频降噪关闭。
     */
    
    kVideoDenoiseModeChangedReasonInternalException = 4,
    /**
     * 
     * @brief 当前设备性能过载，已动态关闭视频降噪。
     */
    
    kVideoDenoiseModeChangedReasonDynamicClose = 5,
    /**
     * 
     * @brief 当前设备性能裕量充足，已动态开启视频降噪。
     */
    
    kVideoDenoiseModeChangedReasonDynamicOpen = 6,
    /**
     * 
     * @brief
     * 分辨率导致视频降噪状态发生改变。分辨率过高会导致性能消耗过大，从而导致视频降噪关闭。若希望继续使用视频降噪，可选择降低分辨率。
     */
    
    kVideoDenoiseModeChangedReasonResolution = 7,
};

/**
 * 
 * @type keytype
 * @brief 屏幕采集对象的类型
 */

enum ScreenCaptureSourceType {
    /**
     * 
     * @brief 类型未知
     */
    
    kScreenCaptureSourceTypeUnknown,

    /**
     * 
     * @brief 应用程序的窗口
     */
    
    kScreenCaptureSourceTypeWindow,

    /**
     * 
     * @brief 桌面
     */
    
    kScreenCaptureSourceTypeScreen
};

/**
 * 
 * @type keytype
 * @brief 屏幕共享对象的详细信息
 */

struct ScreenCaptureSourceInfo {
    /**
     * 
     * @brief 屏幕共享对象的类型，参看 ScreenCaptureSourceType{@link #ScreenCaptureSourceType}
     */
    
    ScreenCaptureSourceType type = kScreenCaptureSourceTypeUnknown;

    /**
     * 
     * @brief 屏幕共享对象的 ID
     */
    
    view_t source_id = nullptr;
    /**
     * 
     * @brief 屏幕共享对象的名称 <br>
     *        调用 release{@link #IScreenCaptureSourceList#release} 时将被释放，请及时转为 string 对象保存
     */
    
    const char* source_name = nullptr;
    /**
     * 
     * @brief 共享的应用窗体所属应用的名称 <br>
     *        当共享对象为应用窗体时有效 <br>
     *        调用 release{@link #IScreenCaptureSourceList#release} 时将被释放，请及时转为 string 对象保存
     */
    
    const char* application = nullptr;
    /**
     * 
     * @brief 共享的应用窗体所属应用进程的 pid <br>
     *        当共享对象为应用窗体时有效
     */
     
    int pid = 0;
    /**
     * 
     * @brief 共享的屏幕是否为主屏。 <br>
     *        当共享对象为屏幕时有效
     */
     
    bool primary_monitor = false;
    /**
     * 
     * @brief 屏幕共享对象的坐标。详见 Rectangle{@link #Rectangle}。 <br>
     *        仅在采集源为显示器屏幕时有效。 <br>
     *        - 对于多屏幕的场景，不同平台的坐标系原点不同：
     *          - 对于 Windows 平台，屏幕坐标以主屏左上角为原点 (0, 0)，向右向下扩展。
     *          — 对于 Linux 平台，屏幕坐标以 **恰好包住所有显示器的矩形区域的左上角** 为原点 (0, 0)，向右向下扩展。 <br>
     *        - 对于不同平台，窗口区域不同：
     *          - 对于 Windows 和 macOS 平台，窗口区域包含系统标题栏。
     *          - 对于 Linux 平台，窗口区域不包含系统标题栏。
     */
    
    Rectangle region_rect;
};
/**
 * 
 * @type api
 * @brief 屏幕共享对象信息列表 <br>
 * 共享对象相关数据将在调用 release{@link #IScreenCaptureSourceList#release} 后被释放。
 * @list 屏幕共享
 */

class IScreenCaptureSourceList {
public:
   /**
    * 
    * @hidden constructor/destructor
    */
   
    virtual ~IScreenCaptureSourceList() {
    }
    /**
     * 
     * @type api
     * @brief 获取列表长度
     * @return 可共享对象数量
     * @list 屏幕共享
     */
    
    virtual int32_t getCount() = 0;
    /**
     * 
     * @type api
     * @brief 根据索引号，获取屏幕共享列表中的元素
     * @param index 列表索引号
     * @return 屏幕源类型信息，详见 ScreenCaptureSourceInfo{@link #ScreenCaptureSourceInfo}。
     * @list 屏幕共享
     */
    
    virtual ScreenCaptureSourceInfo getSourceInfo(int32_t index) = 0;
    /**
     * 
     * @type api
     * @brief IScreenCaptureSourceList{@link #IScreenCaptureSourceList} 使用结束后，调用本接口删除对象，清空和释放内存。
     * @list 屏幕共享
     */
    
    virtual void release() = 0;
};
/**
 * 
 * @type callback
 * @brief 自定义编码帧回调类 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list Screen Sharing
 */

class IExternalVideoEncoderEventHandler {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IExternalVideoEncoderEventHandler(){}
    /**
     * 
     * @type callback
     * @brief 提示自定义编码帧可以开始推送的回调。 <br>
     *        收到该回调后，你即可调用 pushExternalEncodedVideoFrame{@link #IRTCEngine#pushExternalEncodedVideoFrame} 向 SDK 推送自定义编码视频帧
     * @param stream_id 编码流 ID
     * @param stream_info 编码流信息，参看 StreamInfo{@link #StreamInfo}
     * @list 自定义流处理
     */
    
    virtual void onStart(const char* stream_id, const StreamInfo& stream_info) = 0;
    /**
     * 
     * @type callback
     * @brief 当收到该回调时，你需停止向 SDK 推送自定义编码视频帧
     * @param stream_id 编码流 ID
     * @param stream_info 编码流信息，参看 StreamInfo{@link #StreamInfo}
     * @list 自定义流处理
     */
    
    virtual void onStop(const char* stream_id, const StreamInfo& stream_info) = 0;
    /**
     * 
     * @type callback
     * @brief 当自定义编码流的帧率或码率发生变化时，触发该回调
     * @param stream_id 远端编码流 ID
     * @param stream_info 远端编码流信息，参看 StreamInfo{@link #StreamInfo}
     * @param video_index 对应编码流的下标
     * @param info 变化后的编码流信息，参看 VideoRateInfo{@link #VideoRateInfo}
     * @list 自定义流处理
     */
    
    virtual void onRateUpdate(const char* stream_id, const StreamInfo& stream_info, int32_t video_index, VideoRateInfo info) = 0;
    /**
     * 
     * @type callback
     * @brief 提示流发布端需重新生成关键帧的回调
     * @param stream_id 远端编码流 ID
     * @param stream_info 远端编码流信息，参看 StreamInfo{@link #StreamInfo}
     * @param video_index 对应编码流的下标
     * @list 自定义流处理
     */
    
    virtual void onRequestKeyFrame(const char* stream_id, const StreamInfo& stream_info, int32_t video_index) = 0;
    /**
     * 
     * @valid since 3.56
     * @type callback
     * @brief 作为自定义编码视频流的发送端，你会在视频流可发送状态发生变化时，收到此回调。 <br>
     *        你可以根据此回调的提示，仅对可发送的视频流进行编码，以降低本端视频编码性能消耗。此回调会根据多个因素综合判断触发，包括：本端设备性能和本端网络性能，以及按需订阅场景下，远端用户是否订阅。
     * @param stream_id 远端编码流 ID
     * @param stream_info 远端编码流信息，参看 StreamInfo{@link #StreamInfo}
     * @param video_index 对应编码流的下标
     * @param active 该路流可发送状态
     * @note 要收到此回调，必须调用 setVideoSourceType{@link #IRTCEngine#setVideoSourceType} 设置视频源是自定义编码，且通过 setExternalVideoEncoderEventHandler{@link #IRTCEngine#setExternalVideoEncoderEventHandler} 设置了回调句柄。
     * @list 自定义流处理
     */
    
    virtual void onActiveVideoLayer(const char* stream_id, const StreamInfo& stream_info, int32_t video_index, bool active) = 0;
};
/**
 * 
 * @type callback
 * @brief 本地视频帧监测器 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 自定义流处理
 */

class ILocalEncodedVideoFrameObserver {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~ILocalEncodedVideoFrameObserver() {
    }
    /**
     * 
     * @type callback
     * @brief 调用 registerLocalEncodedVideoFrameObserver{@link #IRTCEngine#registerLocalEncodedVideoFrameObserver} 后，SDK 每次使用内部采集，采集到一帧视频帧，或收到一帧外部视频帧时，都会回调该事件。
     * @param video_source 预留参数。
     * @param video_stream 本地视频帧信息，参看 IEncodedVideoFrame{@link #IEncodedVideoFrame}
     * @list 自定义流处理
     */
    
    virtual void onLocalEncodedVideoFrame(IVideoSource *video_source, const IEncodedVideoFrame& video_stream) = 0;
};
/**
 * 
 * @type callback
 * @brief 远端编码后视频数据监测器 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 视频管理
 */

class IRemoteEncodedVideoFrameObserver {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IRemoteEncodedVideoFrameObserver() {
    }
    /**
     * 
     * @type callback
     * @brief 调用 registerRemoteEncodedVideoFrameObserver{@link #IRTCEngine#registerRemoteEncodedVideoFrameObserver} 后，SDK 监测到远端编码后视频数据时，触发该回调
     * @param stream_id 收到的远端流 ID
     * @param stream_info 收到的远端流信息，参看 StreamInfo{@link #StreamInfo}
     * @param video_stream 收到的远端视频帧信息，参看 IEncodedVideoFrame{@link #IEncodedVideoFrame}
     * @list 自定义流处理
     */
    
    virtual void onRemoteEncodedVideoFrame(const char* stream_id, const StreamInfo& stream_info, const IEncodedVideoFrame& video_stream) = 0;
};

/**
 * 
 * @type keytype
 * @brief 视频帧中包含的 metadata 信息
 */

struct VideoMetadataBuffer {
    /**
     * 
     * @brief 接收或者发送的 metadata
     */
    
    char* data;
    /**
     * 
     * @brief 接收或者发送的 metadata 数据大小，不能超过 1024
     */
    
    int size;
    /**
     * 
     * @brief 包含 metadata 视频帧的时间戳，单位微秒
     */
    
    int64_t timestamp_us;
};

/**
 * 
 * @hidden(Linux)
 * @type callback
 * @brief 截图的回调。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list Video Management
 */

class ISnapshotResultCallback {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~ISnapshotResultCallback() = default;
    /**
     * 
     * @type callback
     * @brief 调用 takeLocalSnapshot{@link #IRTCEngine#takeLocalSnapshot} 截取视频画面时，收到此回调。
     * @param task_id 本地截图任务的编号。和 takeLocalSnapshot{@link #IRTCEngine#takeLocalSnapshot} 的返回值一致。
     * @param video_source 截图的视频源，参看IVideoSource{@link #IVideoSource}。
     * @param image 截图。你可以保存为文件，或对其进行二次处理。截图失败时，为空。
     * @param error_code 截图错误码： <br>
     *        - 0: 成功
     *        - -1: 截图错误。生成图片数据失败或 RGBA 编码失败
     *        - -2: 截图错误。流无效。
     *        - -3: 截图错误。截图超时,超时时间 1 秒。
     * @list 高级功能
     */
    
    virtual void onTakeLocalSnapshotResult(long task_id, IVideoSource* video_source, IVideoFrame* image, int error_code) = 0;
    /**
     * 
     * @type callback
     * @brief 调用 takeRemoteSnapshot{@link #IRTCEngine#takeRemoteSnapshot} 截取视频画面时，收到此回调。
     * @param task_id 远端截图任务的编号。和 takeRemoteSnapshot{@link #IRTCEngine#takeRemoteSnapshot} 的返回值一致。
     * @param stream_id 远端流 ID
     * @param stream_info 远端流信息，参看 StreamInfo{@link #StreamInfo}。
     * @param image 截图。你可以保存为文件，或对其进行二次处理。截图失败时，为空。
     * @param error_code 截图错误码： <br>
     *        - 0: 成功
     *        - -1: 截图错误。生成图片数据失败或 RGBA 编码失败
     *        - -2: 截图错误。流无效。
     *        - -3: 截图错误。截图超时,超时时间 1 秒。
     * @list 高级功能
     */
    
    virtual void onTakeRemoteSnapshotResult(long task_id, const char* stream_id, const StreamInfo& stream_info, IVideoFrame* image, int error_code) = 0;
};
/**
 * 
 * @hidden(macOS, Windows, Linux)
 * @type keytype
 * @brief 蜂窝网络辅助增强应用的媒体模式
 */

struct MediaTypeEnhancementConfig {
    /**
     * 
     * @brief 对信令消息，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_signaling = false;
    /**
     * 
     * @brief 对屏幕共享以外的其他音频，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_audio = false;
    /**
     * 
     * @brief 对屏幕共享音频，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_screen_audio = false;
    /**
     * 
     * @brief 对屏幕共享视频以外的其他视频，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_video = false;
    /**
     * 
     * @brief 对屏幕共享视频，是否启用蜂窝网络辅助增强。默认不启用。
     */
    
    bool enhance_screen_video = false;
};

}  
