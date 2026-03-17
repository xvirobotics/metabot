/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Internal API
*/

#import <Foundation/Foundation.h>
#import "ByteRTCDefines.h"
@class ByteRTCRoom;
@class ByteRTCEngine;
@class ByteRTCVideoEncoderConfig;
@class ByteRTCAmazingEffect;

#define BYTERTC_APPLE_EXPORT __attribute__((visibility("default")))

/** 
 * @type keytype
 * @brief 5.18版本后，飞书会议弱网提示专用，设置订阅视频基线数据用于计算网络等级
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCSubscribeVideoBaseline: NSObject
/** 
 * @brief 网络等级判定为优秀的视频分辨率基线，-1表示业务不设置计算时忽略此参数
 */
@property (nonatomic, assign) NSInteger acceptableGoodVideoPixelBaseline;
/** 
 * @brief 网络等级判定为优秀的视频帧率基线，-1表示业务不设置计算时忽略此参数
 */
@property (nonatomic, assign) NSInteger acceptableGoodVideoFpsBaseline;
/** 
 * @brief 网络等级判定为差的视频分辨率基线，-1表示业务不设置计算时忽略此参数
 */
@property (nonatomic, assign) NSInteger acceptableBadVideoPixelBaseline;
/** 
 * @brief 网络等级判定为差的视频帧率基线，-1表示业务不设置计算时忽略此参数
 */
@property (nonatomic, assign) NSInteger acceptableBadVideoFpsBaseline;
/** 
 * @brief 业务可以接受的最小分辨率宽度，-1表示业务忽略此参数
 */
@property (nonatomic, assign) NSInteger acceptableMinVideoResolutionWidth;
/** 
 * @brief 业务可以接受的最小分辨率宽度，-1表示业务忽略此参数
 */
@property (nonatomic, assign) NSInteger acceptableMinVideoResolutionHeight;
/** 
 * @brief 流的优先级，-1表示业务忽略此参数
 */
@property (nonatomic, assign) NSInteger streamPriority;

@end

BYTERTC_APPLE_EXPORT @interface MeetingAdvance : NSObject

/**
 * 
 * @hidden
 * @brief 推流到子频道channelName
 * > 支持会前、会中设置
 * @param [ByteRTCRoom *_Nonnull] room 目标room
 * @param [NSString *_Nonnull] channelName 子频道
 */
+ (int) setPublishChannel:(ByteRTCRoom *_Nonnull)room
              channelName:(NSString *_Nonnull)channelName;
/**
 * 
 * @hidden
 * @brief 订阅子频道数组里的所有流
 * > 支持会前、会中设置
 * @param [ByteRTCRoom *_Nonnull] room 目标room
 * @param [NSArray <NSString *> * _Nonnull] channelsInfo 子频道数组
 * @param [bool] enableVolumeRescale 是否开启音频抑制功能
 */
+ (int) setSubscribeChannels:(ByteRTCRoom *_Nonnull)room
                channelsInfo:(NSArray <NSString *> * _Nonnull)channelsInfo
         enableVolumeRescale:(bool)enableVolumeRescale;
/**
 * 
 * @hidden
 * @brief 设置是否开启音频抑制
 * > 支持会前、会中设置
 * @param [ByteRTCRoom *_Nonnull] room 目标room
 * @param [bool] enableVolumeRescale 是否抑制
 */
+ (int) enableRescaleAudioVolume:(ByteRTCRoom *_Nonnull)room
             enableVolumeRescale:(bool)enableVolumeRescale;
/**
 * 
 * @hidden
 * @brief 设置发布的特殊流类型
 * > 支持会前设置
 * @param [ByteRTCRoom *_Nonnull] room 目标room
 * @param [int] type 流类型
 */
+ (int) setPublishSpecialStream:(ByteRTCRoom *_Nonnull)room
                           type:(int)type;
/**
 * 
 * @hidden
 * @brief 设置订阅的特殊流类型
 * > 支持会前、会中设置
 * @param [ByteRTCRoom *_Nonnull] room 目标room
 * @param [NSArray<NSNumber*> * _Nonnull] types 流类型集合
 */
+ (int) setSubscribeSpecialStream:(ByteRTCRoom *_Nonnull)room
                            types:(NSArray<NSNumber*> * _Nonnull)types;

/**
 * 
 * @hidden
* @brief 判断本地音视频流是否取消发布成功状态
* > 支持会中设置
* @param [ByteRTCRoom *_Nonnull] room 目标room
* @param [ByteRTCMediaStreamType] type 需要检查的流类型
* @return
*        true: 关闭状态；
*        false: 开启状态；
*/
+ (bool)isStreamUnpublished:(ByteRTCRoom *_Nonnull)room type:(ByteRTCMediaStreamType)type;

/**
 * 
 * @hidden
* @brief 设置视频编码参数
* > 该方法用于编码参数设置
* @return
*        0: 成功；
*        != 0: 失败；
*/
+ (int) setVideoEncoderConfig:(ByteRTCEngine*_Nonnull)video channelSolutions:(NSArray <ByteRTCVideoEncoderConfig *> * _Nonnull) channelSolutions
    mainSolutions:(NSArray <ByteRTCVideoEncoderConfig *> * _Nonnull) channelMainSolutions;


+ (int) enableSimulcastMode:(ByteRTCEngine*_Nonnull)video enable:(BOOL) enabled;
/**
 * 
 * @hidden
* @brief 更新自动订阅选项
* > 支持会中设置
*@param [ByteRTCRoom *_Nonnull] room 目标room
*@param [ByteRTCSubscribeMode] audioMode 音频自动订阅模式
*@param [ByteRTCSubscribeMode] videoMode 视频自动订阅模式
*/
+ (int) enableAutoSubscribe:(ByteRTCRoom *_Nonnull)room audioSubMode:(ByteRTCSubscribeMode)audioMode videoMode:(ByteRTCSubscribeMode)videoMode;

/**
 * 
 * @hidden
* @brief 订阅基线设置
* > 支持订阅流前设置
* @param [ByteRTCRoom *_Nonnull] room 目标room
* @param [NSString *_Nonnull] userId 订阅用户名
* @param [BOOL] isScreen 是否屏幕流
* @param [ByteRTCSubscribeVideoBaseline* _Nullable] videoBaseline 订阅基线设置
*/
+ (void) setSubscribeBaselineData:(ByteRTCRoom *_Nonnull)room userId:(NSString *_Nonnull)userId isScreen:(BOOL)isScreen videoBaseline:(ByteRTCSubscribeVideoBaseline* _Nullable)videoBaseline;

/**
 * 
 * @hidden
* @brief 获取用于实时crash分子数据信息
* @return
*        [NSString * _Nonnull]: 返回json格式的信息
*/
+ (NSString * _Nonnull) getSessionLaunchParams;
/**
 * 
 * @brief 获取视频特效接口ByteRTCAmazingEffect
* @param [ByteRTCEngine *_Nonnull] ivideo 目标ByteRTCVideo
*/
+ (ByteRTCAmazingEffect* _Null_unspecified) getAmazingEffectInterface:(ByteRTCEngine *_Nonnull)video;

/**
 * 
 * @hidden
 * @brief 设置屏幕流多分辨率参数
 * @return 0 成功 !0失败
 */
+ (int)setScreenVideoConfigs:(ByteRTCEngine * _Nonnull)video;

/**
 * 
 * @hidden
 * @brief 清空屏幕流多分辨率参数
 * @return 0 成功 !0失败
 */
+ (int)resetScreenVideoConfigs:(ByteRTCEngine * _Nonnull)video;

/**
 * 
 * @brief 获取上行带宽状态
 * @param [ByteRTCRoom*] room 目标room
 */
+ (int)getUplinkNetworkBandwidthEstimationStatus:(ByteRTCRoom *_Nonnull)room;

/**
 * 
 * @brief 获取下行带宽状态
 * @param [ByteRTCRoom*] room 目标room
 */
+ (int)getDownlinkNetworkBandwidthEstimationStatus:(ByteRTCRoom *_Nonnull)room;
@end
