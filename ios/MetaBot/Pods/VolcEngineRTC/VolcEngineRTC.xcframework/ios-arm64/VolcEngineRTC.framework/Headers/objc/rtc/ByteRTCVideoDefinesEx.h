#import "ByteRTCDefines.h"
/**
 * 
 * @type keytype
 * @brief 视频内容类型
 */
typedef NS_ENUM(NSUInteger, ByteRTCVideoContentCategory) {
    /**
     * 
     * @brief 相机采集的内容
     */
    ByteRTCVideoContentCategoryCamera = 0,
    /**
     * 
     * @brief 屏幕共享的内容
     */
    ByteRTCVideoContentCategoryScreen = 1,
};

/**
 * 
 * @type keytype
 * @brief 视频源设置
 */
BYTERTC_APPLE_EXPORT @interface ByteRTCVideoSourceConfig : NSObject
/**
 * 
 * @brief 视频源类型。参看 ByteRTCVideoSourceType{@link #ByteRTCVideoSourceType}。默认是 `kVideoSourceTypeExternal`。
 */
@property (assign, nonatomic) ByteRTCVideoSourceType sourceType;
/**
 * 
 * @brief 参看 ByteRTCVideoContentCategory{@link #ByteRTCVideoContentCategory}。默认是 `kVideoContentCategoryCamera`。
 */
@property (assign, nonatomic) ByteRTCVideoContentCategory contentCategory;
@end


/**
 * 
 * @type keytype
 * @brief 媒体流的优先级
 */
typedef NS_ENUM(NSUInteger, ByteRTCStreamPriority) {
    /**
     * 
     * @brief 低优先级
     */
    ByteRTCStreamPriorityLow = 0,
    /**
     * 
     * @brief 中优先级
     */
    ByteRTCStreamPriorityNormal = 1,
    /**
     * 
     * @brief 高优先级
     */
    ByteRTCStreamPriorityHigh = 2,
};

/**
 * 
 * @type keytype
 * @brief 音频源包含的内容
 */
BYTERTC_APPLE_EXPORT @interface ByteRTCAudioContentTypeConfig : NSObject
/**
 * 
 * @brief 包含麦克风采集的音频
 */
@property(assign, nonatomic) BOOL hasMic;
/**
 * 
 * @brief 包含屏幕音频
 */
@property(assign, nonatomic) BOOL hasScreenAudio;
/**
 * 
 * @brief 包含媒体播放器的音频
 */
@property(assign, nonatomic) BOOL hasMediaPlayer;
@end

/**
 * 
 * @type keytype
 * @brief 音频编码设置。
 */
BYTERTC_APPLE_EXPORT @interface ByteRTCAudioEncodeConfig : NSObject
/**
 * 
 * @brief 编码器类型。<br>
 *        + 0：OPUS<br>
 *        + 1：NICO<br>
 *        + -1：配置下发的配置
 */
@property(nonatomic, assign) NSInteger codecType;
/**
 * 
 * @brief 编码模式<br>
 *        + 0：语音<br>
 *        + 1：音乐<br>
 *        + -1：根据 `channel_num` 不同，使用对应的编码模式。声道数为 `1` 时，编码模式是语音；声道数为 `2` 时，编码模式是音乐。
 */
@property(nonatomic, assign) NSInteger encMode;
/**
 * 
 * @brief 声道数。取值范围是：`{1, 2}`。默认值是 `-1`，配置下发的配置。
 */
@property(nonatomic, assign) NSInteger channelNum;
/**
 * 
 * @brief 码率。取值范围：`[6K, 510K]`。
 */
@property(nonatomic, assign) NSInteger encBitrate;
/**
 * 
 * @brief 是否启用非连续传输功能（DTX）。非连续传输功能能够在安静的场景下降低码率。<br>
 *        + 0：关闭<br>
 *        + 1：打开<br>
 *        + -1：配置下发的配置
 */
@property(nonatomic, assign) NSInteger useDtx;
/**
 * 
 * @brief 带内前向纠错功能。能够提供一定的抗丢包能力。<br>
 *        + 0：关闭<br>
 *        + 1：打开<br>
 *        + -1：配置下发的配置
 */
@property(nonatomic, assign) NSInteger useInbandfec;
/**
 * 
 * @brief 采样率。取值范围是 `[16K, 48K]`。<br>
 *        修改此参数会触发编码器重启。
 */
@property(nonatomic, assign) NSInteger sampleRate;
/**
 * 
 * @brief 编码帧长。取值范围：`{10, 20, 40, 60, 120}`。
 */
@property(nonatomic, assign) NSInteger packetSize;
@end

/**
 * 
 * @type keytype
 * @brief
 */
BYTERTC_APPLE_EXPORT @interface ByteRTCStreamKey : NSObject
/**
 * 
 * @brief 用户 ID
 */
@property(nonatomic, copy, nullable) NSString *userId;
/**
 * 
 * @brief 房间 ID
 */
@property(nonatomic, copy, nullable) NSString *roomId;
/**
 * 
 * @brief 流属性，包括主流、屏幕流。参看 [StreamIndex](70083#StreamIndex-2)
 */
@property(nonatomic, assign) ByteRTCStreamIndex streamIndex;
@end
