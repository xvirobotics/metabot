/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Video Frame
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <memory>
#include <cstring>
#include "bytertc_media_defines.h"
#ifdef BYTERTC_ANDROID
#include <jni.h>
#endif

namespace bytertc {

/**
 * 
 * @type keytype
 * @brief 编码帧类型
 */

enum VideoPictureType {
    /**
     * 
     * @brief 未知类型
     */
    
    kVideoPictureTypeUnknown = 0,
    /**
     * 
     * @brief I 帧，关键帧，编解码不需要参考其他视频帧
     */
    
    kVideoPictureTypeI,
    /**
     * 
     * @brief P 帧，向前参考帧，编解码需要参考前一帧视频帧
     */
    
    kVideoPictureTypeP,
    /**
     * 
     * @brief B 帧，前后参考帧，编解码需要参考前后两帧视频帧
     */
    
    kVideoPictureTypeB
};

/**
 * 
 * @type keytype
 * @brief 视频帧旋转信息
 */

enum VideoRotation {
    /**
     * 
     * @brief 不旋转
    */
   
    kVideoRotation0 = 0,
    /**
     * 
     * @brief 顺时针旋转 90 度
    */
   
    kVideoRotation90 = 90,
    /**
     * 
     * @brief 顺时针旋转 180 度
    */
   
    kVideoRotation180 = 180,
    /**
     * 
     * @brief 顺时针旋转 270 度
    */
   
    kVideoRotation270 = 270
};

/**
 * 
 * @type keytype
 * @brief 视频帧缩放模式，默认值为 FitWithCropping。
 */

enum VideoStreamScaleMode {
    /**
     * 
     * @brief 自动模式
     */
    
    kVideoStreamScaleModeAuto = 0,
    /**
     * 
     * @brief 对视频帧进行缩放，直至充满和视窗分辨率一致为止。这一过程不保证等比缩放。
     */
    
    kVideoStreamScaleModeStretch = 1,
    /**
     * 
     * @brief 视窗填满优先。 <br>
     *        视频帧等比缩放，直至视窗被视频填满。如果视频帧长宽比例与视窗不同，视频帧的多出部分将无法显示。 <br>
     *        缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长大于等于视窗对应边长。
     */
    
    kVideoStreamScaleModeFitWithCropping = 2,
    /**
     * 
     * @brief 视频帧内容全部显示优先。 <br>
     *        视频帧等比缩放，直至视频帧能够在视窗上全部显示。如果视频帧长宽比例与视窗不同，视窗上未被视频帧填满区域将被涂黑。 <br>
     *        缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长小于等于视窗对应边长。
     */
    
    kVideoStreamScaleModeFitWithFilling = 3,
};


/**
 * 
 * @type keytype
 * @brief 视频编码模式
 */

enum VideoCodecMode {
    /**
     * 
     * @brief 自动选择
     */
    
    kVideoCodecModeAuto = 0,
    /**
     * 
     * @brief 硬编码
     */
    
    kVideoCodecModeHardware,
    /**
     * 
     * @brief 软编码
     */
    
    kVideoCodecModeSoftware,
};

/**
 * 
 * @type keytype
 * @brief 编码策略偏好。
 */

enum VideoEncodePreference {
    /**
     * 
     * @brief 无偏好。不降低帧率和分辨率。
     */
    
    kVideoEncodePreferenceDisabled = 0,
    /**
     * 
     * @brief 优先保障帧率。适用于动态画面。
     */
    
    kVideoEncodePreferenceFramerate,
    /**
     * 
     * @brief 清晰模式，优先保障分辨率。适用于静态画面。
     */
    
    kVideoEncodePreferenceQuality,
    /**
     * 
     * @brief 平衡帧率与分辨率。
     * 对于屏幕流来说是开启自动模式智能模式，将根据屏幕内容智能决策选择流畅模式或清晰模式。
     */
    
    kVideoEncodePreferenceAuto,
};


/**
 * 
 * @type keytype
 * @brief 摄像头。
 */

enum CameraID {
    /**
     * 
     * @brief 移动端前置摄像头，PC 端内置摄像头
     */
    
    kCameraIDFront = 0,
    /**
     * 
     * @brief 移动端后置摄像头，PC 端无定义
     */
    
    kCameraIDBack = 1,
    /**
     * 
     * @hidden currently not available
     * @brief 外接摄像头
     */
    
    kCameraIDExternal = 2,
    /**
     * 
     * @brief 无效值
     */
    
    kCameraIDInvalid = 3
};


#define SEND_KBPS_AUTO_CALCULATE -1
#define SEND_KBPS_DISABLE_VIDEO_SEND 0

/**
 * 
 * @deprecated since 3.36 along with setVideoEncoderConfig(StreamIndex index, const VideoSolution* solutions, int solution_num) = 0;
 * @type keytype
 * @brief 视频流参数
 */

struct VideoSolution {
    /**
     * 
     * @brief 视频宽度，单位：像素
     */
    
    int width;
    /**
     * 
     * @brief 视频高度，单位：像素
     */
    
    int height;
    /**
     * 
     * @brief 视频帧率
     */
    
    int fps;
    /**
     * 
     * @brief 最大发送编码码率（kbps），建议使用默认的自动码率。<li>-1: 自动码率</li><li>0: 不开启上限</li><li>>0: 填写预估码率<li>
     */
    
    int max_send_kbps = SEND_KBPS_AUTO_CALCULATE;
    int min_send_kbps = 0;
    /**
     * 
     * @brief 视频编码质量策略，参看 VideoEncodePreference{@link #VideoEncodePreference}
     */
    
    VideoEncodePreference encode_preference = VideoEncodePreference::kVideoEncodePreferenceFramerate;
};

/**
 * 
 * @hidden for internal use only on Windows and Android
 * @type keytype
 * @brief 视野范围（Fov）内的视频帧信息 <br>
 *        Tile 是 全景视频的基本单位。 <br>
 *        视野范围内的视频又分为高清视野和低清背景，均包含了多个 Tile。 <br>
 *        视频帧信息为发送端使用 `setVideoEncoderConfig(const VideoEncoderConfig& encoderConfig, const char* parameters)` 接口进行设置。
 */

struct FovVideoTileInfo {
    /**
     * 
     * @brief 高清视野宽度
     */
    
    uint32_t hd_width = 0;
    /**
     * 
     * @brief 高清视野高度
     */
    
    uint32_t hd_height = 0;
    /**
     * 
     * @brief 低清背景宽度
     */
    
    uint32_t ld_width = 0;
    /**
     * 
     * @brief 低清背景高度
     */
    
    uint32_t ld_height = 0;
    /**
     * 
     * @brief Tile 宽度
     */
    
    uint32_t tile_width = 0;
    /**
     * 
     * @brief Tile 高度
     */
    
    uint32_t tile_height = 0;
    /**
     * 
     * @brief 高清视野中的 Tile 行数
     */
    
    uint32_t hd_row = 0;
    /**
     * 
     * @brief 高清视野中的 Tile 列数
     */
    
    uint32_t hd_column = 0;
    /**
     * 
     * @brief 低清背景中的 Tile 行数
     */
    
    uint32_t ld_row = 0;
    /**
     * 
     * @brief 低清背景中的 Tile 列数
     */
    
    uint32_t ld_column = 0;
    /**
     * 
     * @brief 视野范围中的 Tile 行数
     */
    
    uint32_t dest_row = 0;
    /**
     * 
     * @brief 视野范围中的 Tile 列数
     */
    
    uint32_t dest_column = 0;
    /**
     * 
     * @brief Tile 位置映射表
     */
    
    uint8_t* tile_map = nullptr;
    /**
     * 
     * @brief Tile 数量
     */
    
    uint32_t tile_size = 0;
};
/**
 * 
 * @hidden constructor/destructor
 * @valid since 3.60.
 * @param width 视频宽度
 * @param height 视频高度
 * @type keytype
 * @brief 视频像素
 */

struct VideoDimensions {
    int width;
    int height;
};

/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_120x120 = {120, 120};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_160x120 = {160, 120};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_180x180 = {180, 180};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_240x180 = {240, 180};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_320x180 = {320, 180};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_240x240 = {240, 240};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_320x240 = {320, 240};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_424x240 = {424, 240};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_360x360 = {360, 360};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_480x360 = {480, 360};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_640x360 = {640, 360};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_480x480 = {480, 480};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_640x480 = {640, 480};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_840x480 = {840, 480};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_960x540 = {960, 540};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_960x720 = {960, 720};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_1280x720 = {1280, 720};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_1920x1080 = {1920, 1080};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_2540x1440 = {2540, 1440};
/**
 * 
 * @hidden constructor/destructor
 */

const static VideoDimensions VD_3840x2160 = {3840, 2160};


/**
 * 
 * @type keytype
 * @brief 推荐的视频帧率
 * @hidden constructor/destructor
 */

enum FrameRateFps {
    kFrameRateFps1 = 1,
    kFrameRateFps7 = 7,
    kFrameRateFps10 = 10,
    kFrameRateFps15 = 15,
    kFrameRateFps24 = 24,
    kFrameRateFps30 = 30,
    kFrameRateFps60 = 60,
};
/**
 * 
 * @type keytype
 * @brief 视频编码配置。参考 [设置视频发布参数](https:
 */

struct VideoEncoderConfig {
    /**
     * 
     * @brief 视频宽度，单位：像素
     */
    
    int width;
    /**
     * 
     * @brief 视频高度，单位：像素
     */
    
    int height;
    /**
     * 
     * @brief 视频帧率，单位：fps
     */
    
    int frame_rate;
    /**
     * 
     * @brief 最大编码码率，使用 SDK 内部采集时可选设置，自定义采集时必须设置，单位：kbps。 <br>
     *        内部采集模式下默认值为 -1，即适配码率模式，系统将根据输入的分辨率和帧率自动计算适用的码率。 <br>
     *        设为 0 则不对视频流进行编码发送。
     */
    
    int max_bitrate = SEND_KBPS_AUTO_CALCULATE;
    /**
     * 
     * @brief 视频最小编码码率, 单位 kbps。编码码率不会低于 `minBitrate`。 <br>
     *        默认值为 `0`。 <br>
     *        范围：[0, maxBitrate)，当 `maxBitrate` < `minBitrate` 时，为适配码率模式。 <br>
     *        以下情况，设置本参数无效： <br>
     *        - 当 `maxBitrate` 为 `0` 时，不对视频流进行编码发送。
     *        - 当 `maxBitrate` < `0` 时，适配码率模式。
     */
     
    int min_bitrate = 0;
    /**
     * 
     * @brief 编码策略偏好，默认为帧率优先。参看 VideoEncodePreference{@link #VideoEncodePreference}。
     */
    
    VideoEncodePreference encoder_preference = VideoEncodePreference::kVideoEncodePreferenceFramerate;
};

/**
 * 
 * @hidden
 * @deprecated since 3.36 along with onUserUnPublishStream and onUserUnPublishScreen, and will be deleted in 3.51.
 * @type keytype
 * @brief 视频属性
 */

struct VideoSolutionDescription {
    /**
     * 
     * @brief 宽（像素） <br>
     *        默认值为 `1920` <br>
     *        为 `0` 时，保持源的宽。
     */
    
    int width;
    /**
     * 
     * @brief 高（像素） <br>
     *        默认值为 `1080` <br>
     *        为 `0` 时，保持源的高。
     */
    
    int height;
    /**
     * 
     * @brief 视频帧率(fps)，默认为 15 fps
     */
    
    int fps;
    /**
     * 
     * @brief 最大发送速率（千比特每秒）。 <br>
     *        默认为 `-1`，适配码率模式，系统将根据输入的分辨率和帧率自动计算适用的码率
     */
    
    int max_send_kbps;
    int min_send_kbps;
    /**
     * 
     * @brief 缩放模式。参看 VideoStreamScaleMode{@link #VideoStreamScaleMode}
     */
    
    VideoStreamScaleMode scale_mode = VideoStreamScaleMode::kVideoStreamScaleModeAuto;
    /**
     * 
     * @brief 视频的编码类型。参看 VideoCodecType{@link #VideoCodecType}
     */
    
    VideoCodecType codec_name = VideoCodecType::kVideoCodecTypeUnknown;
    /**
     * 
     * @brief 视频的编码模式。参看 VideoCodecMode{@link #VideoCodecMode}
     */
    
    VideoCodecMode codec_mode = VideoCodecMode::kVideoCodecModeAuto;
    /**
     * 
     * @brief 视频编码质量偏好策略。参看 VideoEncodePreference{@link #VideoEncodePreference}
     */
    
    VideoEncodePreference encode_preference = VideoEncodePreference::kVideoEncodePreferenceFramerate;
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool operator==(const VideoSolutionDescription& config) const {
        bool result = width == config.width && height == config.height && fps == config.fps
                             && max_send_kbps == config.max_send_kbps && min_send_kbps == config.min_send_kbps
                             && scale_mode == config.scale_mode && codec_name == config.codec_name
                             && codec_mode == config.codec_mode && encode_preference == config.encode_preference;
        return result;
    }
    /**
     * 
     * @hidden constructor/destructor
     */
    
    bool operator!=(const VideoSolutionDescription& config) const {
        bool result = (*this == config);
        return !result;
    }
};


/**
 * 
 * @deprecated since 3.45 and will be deleted in 3.51.
 * @type keytype
 * @region 视频管理
 */

typedef VideoSolutionDescription VideoProfile;

/**
 * 
 * @type keytype
 * @brief 视频帧像素格式
 */

enum VideoPixelFormat {
    /**
     * 
     * @brief 未知格式
     */
    
    kVideoPixelFormatUnknown = 0,
    /**
     * 
     * @brief YUV I420 格式
     */
    
    kVideoPixelFormatI420 = 1,
    /**
     * 
     * @brief YUV NV12 格式
     */
    
    kVideoPixelFormatNV12 = 2,
    /**
     * 
     * @brief YUV NV21 格式
     */
    
    kVideoPixelFormatNV21 = 3,
    /**
     * 
     * @brief RGB 24bit 格式，
     */
    
    kVideoPixelFormatRGB24 = 4,
    /**
     * 
     * @brief RGBA 编码格式
     */
    
    kVideoPixelFormatRGBA = 5,
    /**
     * 
     * @brief ARGB 编码格式
     */
    
    kVideoPixelFormatARGB = 6,
    /**
     * 
     * @brief BGRA 编码格式
     */
    
    kVideoPixelFormatBGRA = 7,
    
    
    /**
     * 
     * @hidden for internal use only
     * @brief 像素格式结束标志。新加的格式数值应该小于kVideoPixelFormatEndMark。
     */
    
    kVideoPixelFormatEndMark = 0xFF,
    
    /**
     * 
     * @brief Texture2D 格式
     */
    
    kVideoPixelFormatTexture2D = 0x0DE1,
    /**
     * 
     * @brief TextureOES 格式
     */
    
    kVideoPixelFormatTextureOES = 0x8D65,
};

/**
 * 
 * @type keytype
 * @brief 视频帧内容类型
 */

enum VideoContentType {
    /**
     * 
     * @brief 普通视频
     */
    
    kVideoContentTypeNormalFrame = 0,
    /**
     * 
     * @brief 黑帧
     */
    
    kVideoContentTypeBlackFrame = 1,
};


/**
 * 
 * @type keytype
 * @brief 视频缓冲区类型
 */

enum VideoBufferType {
    /**
     * 
     * @brief 原始内存数据
     */
    
    kVideoBufferTypeRawMemory = 0,
    /**
     * 
     * @hidden(Windows, Android, Linux)
     * @brief CVPixelBufferRef 类型
     */
    
    kVideoBufferTypeCVPixelBuffer = 1,
    /**
     * 
     * @brief OpenGL 纹理数据类型
     */
    
    kVideoBufferTypeGLTexture = 2,
    /**
     * 
     * @brief cuda 数据类型
     */
    
    kVideoBufferTypeCuda = 3,
    /**
     * 
     * @brief direct3d11 纹理
     */
    
    kVideoBufferTypeD3D11 = 4,
    /**
     * 
     * @brief vaapi 数据格式
     */
    
    kVideoBufferTypeVAAPI = 5,
	/**
     * 
     * @hidden(Windows)
     * @brief nvidia jetson dma 数据格式
     */
    
    kVideoBufferTypeNvidiaJetsonDma =6,
};

/**
 * 
 * @type keytype
 * @brief 自定义内存释放器
 */

struct ManagedMemory {
    /**
     * 
     * @brief 内存数据地址
     */
    
    uint8_t* data = nullptr;
    /**
     * 
     * @brief 内存空间的大小
     */
    
    int size = 0;
    /**
     * 
     * @brief 用户自定义数据
     */
    
    void* user_opaque = nullptr;
    /**
     * 
     * @brief 自定义内存释放方法指针，如果指针不为空，方法会被调用，用来释放内存空间。 <br>
     *        函数传入的参数分别是本结构体内 data、size、user_opaque 3 个字段。
     */
    
    int (*memory_deleter)(uint8_t* data, int size, void* user_opaque) = nullptr;
};

/**
 * 
 * @type keytype
 * @region 视频管理
 * @brief 视频帧数据
 */

typedef struct VideoFrameData {
    /**
     * 
     * @brief 视频帧缓冲区类型，默认为原始内存，详见 VideoBufferType{@link #VideoBufferType}。必填。
     */
    
    VideoBufferType buffer_type = kVideoBufferTypeRawMemory;
    /**
     * 
     * @brief 视频帧像素格式，详见 VideoPixelFormat{@link #VideoPixelFormat}。当 `buffer_type` 为 `kVideoFrameTypeGLTexture` 必填。
     */
    
    VideoPixelFormat pixel_format = kVideoPixelFormatUnknown;

    /**
     * 
     * @brief 视频帧内容类型，参看 VideoContentType{@link #VideoContentType}
     */
    
    VideoContentType content_type = kVideoContentTypeNormalFrame;
    /**
     * 
     * @brief 视频帧平面个数。当 `buffer_type` 为 `kVideoBufferTypeRawMemory` 时，必填。
     */
    
    int number_of_planes = 0;
    /**
     * 
     * @brief 视频帧平面数组。当 `buffer_type` 为 `kVideoBufferTypeRawMemory` 时，必填。
     */
    
    uint8_t* plane_data[4] = { nullptr };
    /**
     * 
     * @brief stride 数组。stride 指视频帧平面相邻两行图像数据之间的内存长度（单位字节）。当 `buffer_type` 为 `kVideoBufferTypeRawMemory` 时，必填。
     */
    
    int plane_stride[4] = { 0 };
    /**
     * 
     * @brief SEI 数据
     */
    
    uint8_t* sei_data = nullptr;
    /**
     * 
     * @brief SEI 数据大小
     */
    
    int sei_data_size = 0;
    /**
     * 
     * @brief 视频帧感兴趣区域数据
     */
    
    uint8_t* roi_data = nullptr;
    /**
     * 
     * @brief 视频帧感兴趣区域数据数据长度
     */
    
    int roi_data_size = 0;
    /**
     * 
     * @brief 视频帧宽度。必填。
     */
    
    int width = 0;
    /**
     * 
     * @brief 视频帧高度。必填。
     */
    
    int height = 0;
    /**
     * 
     * @brief 视频旋转角度，参看 VideoRotation{@link #VideoRotation}。 <br>
     */
    
    VideoRotation rotation = kVideoRotation0;
    /**
     * 
     * @brief 视频帧时间戳，单位：微秒。必填。
     */
    
    int64_t timestamp_us = 0;
    /**
     * 
     * @brief 硬件加速缓冲区指针
     */
    
    void* hw_buffer = nullptr;
    /**
    * 
    * @brief 硬件加速上下文对象句柄
    */
    
    void* hw_context = nullptr;
    /**
     * 
     * @brief 纹理矩阵(仅针对纹理类型的 frame 生效)
     */
    
    float texture_matrix[16] = { 0 };
    /**
     * 
     * @brief 纹理 ID(仅针对纹理类型的 frame 生效)。当 `buffer_type` 为 `kVideoBufferTypeRawMemory` 或 `kVideoFrameTypeGLTexture` 必填。
     */
    
    uint32_t texture_id = 0;
    
} VideoFrameData;

/**
 * 
 * @type keytype
 * @brief 视频帧接口
 */

class IVideoFrame {
public:
    /**
     * 
     * @brief 获取视频帧缓冲区类型，参看 VideoBufferType{@link #VideoBufferType}。
     */
    
    virtual VideoBufferType bufferType() = 0;
    /**
     * 
     * @brief 获取视频帧像素格式，参看 VideoPixelFormat{@link #VideoPixelFormat}。
     */
    
    virtual VideoPixelFormat pixelFormat() = 0;
    /**
     * 
     * @brief 获取视频帧内容类型，参看 VideoContentType{@link #VideoContentType}。
     */
    
    virtual VideoContentType contentType() = 0;

    /**
     * 
     * @brief 获取视频帧时间戳，单位：微秒
     */
    
    virtual int64_t timestampUs() = 0;
    /**
     * 
     * @brief 获取视频帧宽度
     */
    
    virtual int width() = 0;
    /**
     * 
     * @brief 获取视频帧高度
     */
    
    virtual int height() = 0;
    /**
     * 
     * @brief 获取视频帧旋转角度，参看 VideoRotation{@link #VideoRotation}
     */
    
    virtual VideoRotation rotation() = 0;
    /**
     * 
     * @brief 获取视频帧平面数量
     */
    
    virtual int numberOfPlanes() = 0;
    /**
     * 
     * @brief 获取视频帧平面
     * @param plane_index plane 索引
     */
    
    virtual uint8_t* planeData(int plane_index) = 0;
    /**
     * 
     * @brief 获取视频帧平面相邻两行图像数据之间的内存长度（单位字节）
     * @param plane_index plane 索引
     */
    
    virtual int planeStride(int plane_index) = 0;
    /**
     * 
     * @brief 获取 SEI 数据指针
     * @param size SEI 数据字节数
     */
    
    virtual uint8_t* seiData(int& size)  = 0;  
    /**
     * 
     * @brief 获取硬件缓冲区指针
     */
    
    virtual void* hwBuffer() = 0;
    /**
     * 
     * @brief 获取硬件加速上下文对象句柄
     */
    
    virtual void* hwContext() = 0;
    /**
     * 
     * @brief 获取纹理矩阵(仅针对纹理类型的 frame 生效)
     * @param matrix 纹理矩阵
     */
    
    virtual void textureMatrix(float matrix[16]) = 0;
    /**
     * 
     * @brief 获取纹理 ID(仅针对纹理类型的 frame 生效)
     */
    
    virtual uint32_t textureId() = 0;
    /**
     * 
     * @brief 视频帧引用计数加一
     * @note 视频帧消费者希望对视频帧进行异步处理时（例如切换线程进行渲染），需要调用此接口增加引用计数。异步处理结束则需要调用 `releaseRef` 使引用计数减1
     */
    
    virtual void addRef() = 0;
    /**
     * 
     * @brief  视频帧引用计数减一
     * @note 视频帧引用计数减为 0 时，视频帧对象会被释放。视频帧对象释放后，不应该继续使用视频帧。
     */
    
    virtual long releaseRef() = 0;
    /**
     * 
     * @brief 获取视频帧的摄像头位置信息，参看 CameraID{@link #CameraID}
     */
    
    virtual CameraID cameraId()  = 0;
    /**
     * 
     * @hidden for internal use only on Windows and Android
     * @type api
     * @brief 获取全景视频的 Tile 信息
     * @return FoV（可视范围）随本端的头位姿实时更新获取到的视频帧，包括高清视野和低清背景。参见 FovVideoTileInfo{@link #FovVideoTileInfo}。
     * @list 
     */
    
    virtual FovVideoTileInfo fovTileInfo() = 0;
/**
 * 
 * @hidden constructor/destructor
 */

protected:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IVideoFrame() = default;
};





















/**
 * 
 * @type keytype
 * @region 视频管理
 * @brief 视频帧参数
 */

typedef struct EncodedVideoFrameBuilder {
    /**
     * 
     * @brief 视频帧编码格式，参看 VideoCodecType{@link #VideoCodecType}
     */
    
    VideoCodecType codec_type = kVideoCodecTypeUnknown;
    /**
     * 
     * @brief 视频帧编码类型，参看 VideoPictureType{@link #VideoPictureType}
     */
    
    VideoPictureType picture_type = kVideoPictureTypeUnknown;
    /**
     * 
     * @brief 视频帧旋转角度，参看 VideoRotation{@link #VideoRotation}
     */
    
    VideoRotation rotation = kVideoRotation0;
    /**
     * 
     * @brief 视频帧数据指针
     * @note IEncodedVideoFrame 会获取数据的所有权
     */
    
    uint8_t* data = nullptr;
    /**
     * 
     * @brief 视频帧数据大小
     */
    
    int size = 0;
    /**
     * 
     * @brief 视频帧宽度，单位：px
     */
    
    int width = 0;
    /**
     * 
     * @brief 视频帧高度，单位：px
     */
    
    int height = 0;
    /**
     * 
     * @brief 视频采集时间戳，单位：微秒
     */
    
    int64_t timestamp_us = 0;
    /**
     * 
     * @brief 视频编码时间戳，单位：微秒
     */
    
    int64_t timestamp_dts_us = 0;
    /**
     * 
     * @brief 用户定义的视频帧释放回调函数指针，如果指针不为空，方法会被调用用来释放内存空间
     */
    
    int (*memory_deleter)(uint8_t* data, int size, void* user_opaque) = nullptr;
} EncodedVideoFrameBuilder;
/**
 * 
 * @type keytype
 * @brief 视频帧信息
 */ 

class IEncodedVideoFrame {
public:
    /**
     * 
     * @type api
     * @brief 获取视频编码类型
     * @return 视频编码类型，参看 VideoCodecType{@link #VideoCodecType}
     * @list Video Management
     */
    
    virtual VideoCodecType codecType() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频采集时间戳
     * @return 视频采集时间戳，单位：微秒
     * @list Video Management
     */
    
    virtual int64_t timestampUs() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频编码时间戳
     * @return 视频编码时间戳，单位：微秒
     * @list Video Management
     */
    
    virtual int64_t timestampDtsUs() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频帧宽度
     * @return 视频帧宽度，单位：px
     * @list Video Management
     */
    
    virtual int width() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频帧高度
     * @return 视频帧高度，单位：px
     * @list Video Management
     */
    
    virtual int height() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频编码帧类型
     * @return 视频编码帧类型，参看 VideoPictureType{@link #VideoPictureType}
     * @list Video Management
     */
    
    virtual VideoPictureType pictureType() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频帧旋转角度
     * @return 视频帧旋转角度，参看 VideoRotation{@link #VideoRotation}
     * @list Video Management
     */
    
    virtual VideoRotation rotation() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频帧数据指针地址
     * @return 视频帧数据指针地址
     * @list Video Management
     */
    
    virtual uint8_t* data() const = 0;
    /**
     * 
     * @type api
     * @brief 获取视频帧数据大小
     * @return 视频帧数据大小
     * @list Video Management
     */
    
    virtual int dataSize() const = 0;
    /**
     * 
     * @type api
     * @brief 浅拷贝视频帧并返回指针
     * @list Video Management
     */
    
    virtual IEncodedVideoFrame* shallowCopy() const = 0;
    /**
     * 
     * @type api
     * @brief 释放视频帧
     * @list Video Management
     */
    
    virtual void release() = 0;
    /**
     * 
     * @hidden constructor/destructor
     */
    
protected:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IEncodedVideoFrame() = default;
};
/**
 * 
 * @type api
 * @brief 创建 IEncodedVideoFrame
 * @param builder 编码后的视频帧构建实例，参看 EncodedVideoFrameBuilder{@link #EncodedVideoFrameBuilder}
 * @return IEncodedVideoFrame{@link #IEncodedVideoFrame} 实例
 * @list 视频管理
 */

BYTERTC_API IEncodedVideoFrame* buildEncodedVideoFrame(const EncodedVideoFrameBuilder& builder);


/**
 * 
 * @hidden(Linux,Android,iOS)
 * @type keytype
 * @brief RTC 智能决策后得到的帧率和分辨率积（宽*高）。
 */

struct FrameUpdateInfo {
    /**
     * 
     * @brief 分辨率积（宽*高）。
     */
    
    int pixel;
    /**
     * 
     * @brief 帧率。
     */
    
    int framerate;
};

}  
