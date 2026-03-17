


#pragma once
namespace bytertc {
/**
 * 
 * @type keytype
 * @brief 视频内容类型
 */

enum VideoContentCategory {
    /**
     * 
     * @brief 相机采集的内容
     */
    
    kVideoContentCategoryCamera = 0, 
    /**
     * 
     * @brief 屏幕共享的内容
     */
    
    kVideoContentCategoryScreen = 1, 
};
/**
 * 
 * @type keytype
 * @brief 视频源设置
 */

struct VideoSourceConfig {
    /**
     * 
     * @brief 视频源类型。参看 VideoSourceType{@link #VideoSourceType}。默认是 `kVideoSourceTypeExternal`。
     */
    
    VideoSourceType source_type = kVideoSourceTypeExternal;
    /**
     * 
     * @brief 参看 VideoContentCategory{@link #VideoContentCategory}。默认是 `kVideoContentCategoryCamera`。
     */
    /**
     * 
     * @brief Refer to VideoContentCategory{@link #VideoContentCategory}. It defaults to `kVideoContentCategoryCamera`.
     */
    VideoContentCategory content_category = kVideoContentCategoryCamera;
};
/**
 * 
 * @type keytype
 * @brief 音频源包含的内容
 */

struct AudioContentTypeConfig {
    /**
     * 
     * @brief 包含麦克风采集的音频
     */
    
    bool has_mic = true;
    /**
     * 
     * @brief 包含屏幕音频
     */
    
    bool has_screen_audio = false;
    /**
     * 
     * @brief 包含媒体播放器的音频
     */
    
    bool has_media_player = false;
};
/**
 * 
 * @type keytype
 * @brief 音频编码设置。
 */

struct AudioEncodeConfig {
    /**
     * 
     * @brief 编码器类型。<br>
     *        + 0：OPUS<br>
     *        + 1：NICO<br>
     *        + -1：配置下发的配置
     */
    
    int codec_type = -1;
    /**
     * 
     * @brief 编码模式<br>
     *        + 0：语音<br>
     *        + 1：音乐<br>
     *        + -1：根据 `channel_num` 不同，使用对应的编码模式。声道数为 `1` 时，编码模式是语音；声道数为 `2` 时，编码模式是音乐。
     */
    int enc_mode = -1;
    /**
     * 
     * @brief 声道数。取值范围是：`{1, 2}`。默认值是 `-1`，配置下发的配置。
     */
    int channel_num = -1;
    /**
     * 
     * @brief 码率。取值范围：`[6K, 510K]`。
     */
    int enc_bitrate = -1;
    /**
     * 
     * @brief 是否启用非连续传输功能（DTX）。非连续传输功能能够在安静的场景下降低码率。<br>
     *        + 0：关闭<br>
     *        + 1：打开<br>
     *        + -1：配置下发的配置
     */
    int use_dtx = -1;
    /**
     * 
     * @brief 带内前向纠错功能。能够提供一定的抗丢包能力。<br>
     *        + 0：关闭<br>
     *        + 1：打开<br>
     *        + -1：配置下发的配置
     */
    int use_inbandfec = -1;
    /**
     * 
     * @brief 采样率。取值范围是 `[16K, 48K]`。<br>
     *        修改此参数会触发编码器重启。
     */
    int sample_rate = -1;
    /**
     * 
     * @brief 编码帧长。取值范围：`{10, 20, 40, 60, 120}`。
     */
    int packet_size = -1;
};
/**
 * 
 * @type keytype
 * @brief 媒体流的优先级
 */
enum StreamPriority {
    /**
     * 
     * @brief 低优先级
     */
    kStreamPriorityLow = 0,
    /**
     * 
     * @brief 中优先级
     */
    kStreamPriorityNormal = 1,
    /**
     * 
     * @brief 高优先级
     */
    kStreamPriorityHigh = 2,
};

/**
 * 
 * @type keytype
 * @brief 
 */
struct StreamKey {
    /**
     * 
     * @brief 房间 ID
     */
    const char* room_id;
    /**
     * 
     * @brief 用户 ID
     */
    const char* user_id;
    /**
     * 
     * @brief 流索引。参看 StreamIndex{@link #StreamIndex}。
     */
    StreamIndex stream_index;
};

}