/*
 * Copyright (c) 2021 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCAudioMixingManager
*/

#import <Foundation/Foundation.h>
#import "ByteRTCAudioDefines.h"
/**
 * 
 * @type callback
 * @brief K 歌评分事件回调类。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

BYTERTC_APPLE_EXPORT @protocol ByteRTCSingScoringDelegate<NSObject>
@required
/**
 * 
 * @type callback
 * @brief 实时评分信息回调。调用 startSingScoring:scoringInfoInterval:{@link #ByteRTCSingScoringManager#startSingScoring:scoringInfoInterval:} 后，会收到该回调。
 * @param info 实时评分信息。详见 ByteRTCSingScoringRealtimeInfo{@link #ByteRTCSingScoringRealtimeInfo}。
 * @list 在线 KTV
 */

- (void)onCurrentScoringInfo:(ByteRTCSingScoringRealtimeInfo * _Nullable)info;
@end
/**
 * 
 * @type api
 * @brief K 歌评分管理接口。
 * @list Online Karaoke
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCSingScoringManager :NSObject
/**
 * 
 * @type api
 * @brief 初始化 K 歌评分。
 * @param singScoringAppkey K 歌评分密钥，用于鉴权验证 K 歌功能是否开通。
 * @param singScoringToken K 歌评分密钥，用于鉴权验证 K 歌功能是否开通。
 * @param delegate K 歌评分事件回调类，详见 ByteRTCSingScoringDelegate{@link #ByteRTCSingScoringDelegate}。
 * @return
 *        - 0：配置成功。
 *        - -1：接口调用失败。
 *        - -2：未集成 K 歌评分模块。
 *        - >0：其他错误，具体参看[错误码表](https:
 * @note 输入正确的鉴权信息才可以使用 K 歌评分相关的功能，鉴权方式为离线鉴权，根据包名（bundleID）绑定 Appkey 及 Token，K 歌评分密钥请联系技术支持人员申请。
 * @list 在线 KTV
 */

-(int)initSingScoring:(NSString * _Nullable)singScoringAppkey
     singScoringToken:(NSString * _Nullable)singScoringToken
             delegate:(id<ByteRTCSingScoringDelegate> _Nullable)delegate;
/**
 * 
 * @type api
 * @brief 设置 K 歌评分参数。
 * @param config K 歌评分的各项参数，详见 ByteRTCSingScoringConfig{@link #ByteRTCSingScoringConfig}。
 * @return
 *        - 0：配置成功。
 *        - -1：接口调用失败。
 *        - -2：未集成 K 歌评分模块。
 *        - >0：其他错误，具体参看[错误码表](https:
 * @list 在线 KTV
 */

-(int)setSingScoringConfig:(ByteRTCSingScoringConfig * _Nullable)config;
/**
 * 
 * @type api
 * @brief 获取标准音高数据。
 * @param midiFilepath 歌曲 midi 文件路径。
 * @return ByteRTCStandardPitchInfo{@link #ByteRTCStandardPitchInfo} 标准音高数据数组。
 * @note
 *        - 请保证此接口传入的 midi 文件路径与 setSingScoringConfig:{@link #ByteRTCSingScoringManager#setSingScoringConfig:} 接口中传入的路径一致。
 * @list 在线 KTV
 */

-(NSArray<ByteRTCStandardPitchInfo* > * _Nullable)getStandardPitchInfo:(NSString* _Nullable)midiFilepath;
/**
 * 
 * @type api
 * @brief 开始 K 歌评分。
 * @param position 开始评分时，音乐的播放进度，单位：ms。
 * @param scoringInfoInterval 实时回调的时间间隔，单位：ms；默认 50 ms。最低间隔为 20 ms。
 * @return
 *        - 0：配置成功。
 *        - -1：接口调用失败。
 *        - -2：未集成 K 歌评分模块。
 *        - >0：其他错误，具体参看[错误码表](https:
 * @note
 *        - 在调用 initSingScoring:singScoringToken:delegate:{@link #ByteRTCSingScoringManager#initSingScoring:singScoringToken:delegate:} 初始化 K 歌评分功能后调用该接口。
 *        - 调用该接口后，将会根据设置的回调时间间隔，收到评分结果 onCurrentScoringInfo:{@link #ByteRTCSingScoringDelegate#onCurrentScoringInfo:} 回调。
 *        - 如果使用 `ByteRTCMediaPlayer` 播放伴奏，请在收到 onMediaPlayerStateChanged:state:error:{@link #ByteRTCMediaPlayerEventHandler#onMediaPlayerStateChanged:state:error:} 回调且播放状态为 ByteRTCPlayerStatePlaying 后，调用此接口。
 * @list 在线 KTV
 */

-(int)startSingScoring:(int)position
   scoringInfoInterval:(int)scoringInfoInterval;
/**
 * 
 * @type api
 * @brief 停止 K 歌评分。
 * @return
 *        - 0：成功。
 *        - <0：失败。
 * @list 在线 KTV
 */

-(int)stopSingScoring;
/**
 * 
 * @type api
 * @brief 获取上一句的演唱评分。调用 startSingScoring:scoringInfoInterval:{@link #ByteRTCSingScoringManager#startSingScoring:scoringInfoInterval:} 开始评分后可以调用该接口。
 * @return
 *        - <0：获取评分失败。
 *        - >=0：上一句歌词的演唱评分。
 * @list 在线 KTV
 */

-(int)getLastSentenceScore;
/**
 * 
 * @type api
 * @brief 获取当前演唱总分。调用 startSingScoring:scoringInfoInterval:{@link #ByteRTCSingScoringManager#startSingScoring:scoringInfoInterval:} 开始评分后可以调用该接口。
 * @return
 *        - <0：获取总分失败。
 *        - >=0：当前演唱总分。
 * @list 在线 KTV
 */

 -(int)getTotalScore;
 /**
 * 
 * @type api
 * @brief 获取当前演唱歌曲的平均分。
 * @return
 *        - <0：获取平均分失败。
 *        - >=0：当前演唱平均分。
 * @list 在线 KTV
 */

-(int)getAverageScore;
@end