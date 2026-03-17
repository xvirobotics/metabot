/*
 *  Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief range audio interface
 */

#pragma once

#import <Foundation/Foundation.h>
#import "ByteRTCAudioDefines.h"

/**
 * 
 * @type keytype
 * @brief 空间音频音量随距离衰减模式
 */
 
typedef NS_ENUM(NSInteger, ByteRTCAttenuationType) {
    /**
     * 
     * @brief 不随距离衰减
     */
    
    ByteRTCAttenuationTypeNone = 0,
    /**
     * 
     * @brief 线性衰减，音量随距离增大而线性减小
     */
    
    ByteRTCAttenuationTypeLinear = 1,
    /**
     * 
     * @brief 指数型衰减，音量随距离增大进行指数衰减
     */
    
    ByteRTCAttenuationTypeExponential = 2
};

/**
 * 
 * @type keytype
 * @brief 本地用户能收听到、且具有衰减效果的音频接收范围
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCReceiveRange : NSObject
/**
 * 
 * @brief 能够接收语音、并且具有衰减效果的最小距离值，该值须 ≥ 0，但 ≤ max。 <br>
 *        小于该值的范围内没有范围语音效果，即收听到的音频音量相同。
 */

@property(nonatomic, assign) int min;
/**
 * 
 * @brief 能够收听语音的最大距离值，该值须 > 0 且 ≥ min。 <br>
 *        当收听者和声源距离处于 [min, max) 之间时，收听到的音量根据距离呈衰减效果。 <br>
 *        超出该值范围的音频将无法收听到。
 */

@property(nonatomic, assign) int max;
@end
/**
 * 
 * @type api
 * @brief 范围语音接口实例
 * @list 
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCRangeAudio :NSObject
/**
 * 
 * @type api
 * @brief 开启/关闭范围语音功能。 <br>
 *        范围语音是指，在同一 RTC 房间中设定的音频接收距离范围内，本地用户收听到的远端用户音频音量会随着远端用户的靠近/远离而放大/衰减；若远端用户在房间内的位置超出设定范围，则本地用户无法接收其音频。音频接收范围设置参看 updateReceiveRange:{@link #ByteRTCRangeAudio#updateReceiveRange:}。
 * @param enable 是否开启范围语音功能： <br>
 *        - YES: 开启
 *        - NO: 关闭（默认）
 * @note 该方法进房前后都可调用，为保证进房后范围语音效果的平滑切换，你需在该方法前先调用 updatePosition:{@link #ByteRTCRangeAudio#updatePosition:} 设置自身位置坐标，然后开启该方法收听范围语音效果。
 * @list 高级功能
 */

- (void)enableRangeAudio:(BOOL)enable;
/**
 * 
 * @type api
 * @brief 更新本地用户的音频收听范围。
 * @param range 音频收听范围，参看 ByteRTCReceiveRange{@link #ByteRTCReceiveRange}。
 * @return 方法调用结果： <br>
 *        - 0：成功；
 *        - !0: 失败。
 * @list 高级功能
 */

- (int)updateReceiveRange:(ByteRTCReceiveRange* _Nonnull) range;
/**
 * 
 * @type api
 * @brief 更新本地用户在房间内空间直角坐标系中的位置坐标。
 * @param pos 三维坐标的值，默认为 [0, 0, 0]，参看 ByteRTCPosition{@link #ByteRTCPosition}.
 * @return 方法调用结果： <br>
 *        - 0：成功；
 *        - !0：失败。
 * @note 调用该接口更新坐标后，你需调用 enableRangeAudio:{@link #ByteRTCRangeAudio#enableRangeAudio:} 开启范围语音功能以收听范围语音效果。
 * @list 高级功能
 */

- (int)updatePosition:(ByteRTCPosition* _Nonnull) pos;
/**
 * 
 * @type api
 * @brief 设置范围语音的音量衰减模式。
 * @param type 音量衰减模式。默认为线性衰减。详见 ByteRTCAttenuationType{@link #ByteRTCAttenuationType}。
 * @param coefficient 指数衰减模式下的音量衰减系数，默认值为 1。范围 [0.1,100]，推荐设置为 `50`。数值越大，音量的衰减速度越快。
 * @return 调用是否成功 <br>
 *         - `0`:调用成功
 *         - `-1`:调用失败。原因为在调用 enableRangeAudio:{@link #ByteRTCRangeAudio#enableRangeAudio:} 开启范围语音前或进房前调用本接口
 * @note 音量衰减范围通过 updateReceiveRange:{@link #ByteRTCRangeAudio#updateReceiveRange:} 进行设置。
 * @list 高级功能
 */

- (int)setAttenuationModel:(ByteRTCAttenuationType) type coefficient:(float)coefficient;
/**
 * 
 * @type api
 * @brief 添加标签组，用于标记相互之间通话不衰减的用户组。 <br>
 *        在同一个 RTC 房间中，如果多个用户的标签组之间有交集，那么，他们之间互相通话时，通话不衰减。 <br>
 *        比如，用户身处多个队伍，队伍成员间通话不衰减。那么，可以为每个队伍绑定专属标签，每个用户的标签组包含用户所属各个队伍的标签。
 * @param flags 标签组。
 * @list 高级功能
 */

- (void) setNoAttenuationFlags:(NSArray <NSString *> *_Nonnull)flags;

@end