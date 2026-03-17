/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCKTVPlayer
*/

#import <Foundation/Foundation.h>
#import "ByteRTCKTVDefines.h"
@class ByteRTCKTVPlayer;

NS_ASSUME_NONNULL_BEGIN
/**
 * 
 * @type callback
 * @brief KTV 播放器回调接口。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

@protocol ByteRTCKTVPlayerDelegate <NSObject>
/**
 * 
 * @type callback
 * @brief 音乐播放进度回调。
 * @param ktvPlayer 当前 ktvPlayer 对象，参看 ByteRTCKTVPlayer{@link #ByteRTCKTVPlayer}。
 * @param musicId 音乐 ID。
 * @param progress 音乐播放进度，单位为毫秒。
 * @list 在线 KTV
 */

- (void)ktvPlayer:(ByteRTCKTVPlayer *)ktvPlayer onPlayProgress:(NSString *)musicId progress:(int64_t)progress;
/**
 * 
 * @type callback
 * @brief 音乐播放状态改变回调。
 * @param ktvPlayer 当前 ktvPlayer 对象，参看 ByteRTCKTVPlayer{@link #ByteRTCKTVPlayer}。
 * @param musicId 音乐 ID。
 * @param state 音乐播放状态，参看 ByteRTCPlayState{@link #ByteRTCPlayState}。
 * @param error 错误码，参看 ByteRTCKTVPlayerErrorCode{@link #ByteRTCKTVPlayerErrorCode}。
 * @note
 *       此回调被触发的时机汇总如下： <br>
 *       - 调用 playMusic:audioTrackType:audioPlayType:{@link #ByteRTCKTVPlayer#playMusic:audioTrackType:audioPlayType:} 成功后，会触发 playState 值为 ByteRTCPlayStatePlaying 的回调；否则会触发 playState 值为 ByteRTCPlayStateFailed 的回调。
 *       - 使用相同的音乐 ID 重复调用 playMusic:audioTrackType:audioPlayType:{@link #ByteRTCKTVPlayer#playMusic:audioTrackType:audioPlayType:} 后，后一次播放会覆盖前一次，且会触发 playState 值为 ByteRTCPlayStatePlaying 的回调，表示后一次音乐播放已开始。
 *       - 调用 pauseMusic:{@link #ByteRTCKTVPlayer#pauseMusic:} 方法暂停播放成功后，会触发 playState 值为 ByteRTCPlayStatePaused 的回调；否则触发 playState 值为 ByteRTCPlayStateFailed 的回调。
 *       - 调用 resumeMusic:{@link #ByteRTCKTVPlayer#resumeMusic:} 方法恢复播放成功后，会触发 playState 值为 ByteRTCPlayStatePlaying 的回调；否则触发 playState 值为 ByteRTCPlayStateFailed 的回调。
 *       - 调用 stopMusic:{@link #ByteRTCKTVPlayer#stopMusic:} 方法停止播放成功后，会触发 playState 值为 ByteRTCPlayStateStoped 的回调；否则触发 playState 值为 ByteRTCPlayStateFailed 的回调。
 *       - 音乐播放结束会触发 playState 值为 ByteRTCPlayStateFinished 的回调。
 * @list Online Karaoke
 */

- (void)ktvPlayer:(ByteRTCKTVPlayer *)ktvPlayer onPlayStateChanged:(NSString *)musicId state:(ByteRTCPlayState)state error:(ByteRTCKTVPlayerErrorCode)error;
@end
/**
 * 
 * @type api
 * @brief KTV 播放器接口。
 * @list Online Karaoke
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCKTVPlayer : NSObject

@property(nonatomic, weak) id<ByteRTCKTVPlayerDelegate> delegate;

#pragma mark - Core Methods
/**
 * 
 * @type api
 * @brief 播放歌曲。
 * @param musicId 音乐 ID。 <br>
 *        若同一 musicId 的歌曲正在播放，再次调用接口会从开始位置重新播放。若 musicId 对应的音频文件不存在会触发报错。
 * @param trackType 原唱伴唱类型，参看 ByteRTCAudioTrackType{@link #ByteRTCAudioTrackType}。
 * @param playType 音乐播放类型。参看 ByteRTCAudioPlayType{@link #ByteRTCAudioPlayType}。
 * @note
 *        - 调用接口后，你会收到 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调歌曲播放状态。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 *        - 若音乐文件不存在，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3020，playState 为 4。
 * @list 在线 KTV
 */

- (void)playMusic:(NSString * _Nonnull)musicId audioTrackType:(ByteRTCAudioTrackType)trackType audioPlayType:(ByteRTCAudioPlayType)playType;
/**
 * 
 * @type api
 * @brief 暂停播放歌曲。
 * @param musicId 音乐 ID。
 * @note
 *        - 调用接口后，你会收到 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调歌曲播放状态。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 * @list 在线 KTV
 */

- (void)pauseMusic:(NSString * _Nonnull)musicId;
/**
 * 
 * @type api
 * @brief 继续播放歌曲。
 * @param musicId 音乐 ID。
 * @note
 *        - 调用接口后，你会收到 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调歌曲播放状态。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 * @list 在线 KTV
 */

- (void)resumeMusic:(NSString * _Nonnull)musicId;
/**
 * 
 * @type api
 * @brief 停止播放歌曲。
 * @param musicId 音乐 ID。
 * @note
 *        - 调用接口后，你会收到 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调歌曲播放状态。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 * @list 在线 KTV
 */

- (void)stopMusic:(NSString * _Nonnull)musicId;
/**
 * 
 * @type api
 * @brief 设置音乐文件的起始播放位置。
 * @param musicId 音乐 ID。
 * @param position 音乐起始位置，单位为毫秒，取值小于音乐文件总时长。
 * @note
 *        - 调用本接口时音乐必须处于播放中状态。
 *        - 调用接口后，你会收到 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调歌曲播放状态。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 * @list 在线 KTV
 */

- (void)seekMusic:(NSString * _Nonnull)musicId position:(int)position;
/**
 * 
 * @type api
 * @brief 设置歌曲播放音量，只能在开始播放后进行设置。
 * @param musicId 音乐 ID。
 * @param volume 歌曲播放音量，调节范围：[0,400]。 <br>
 *        - 0：静音。
 *        - 100：原始音量。
 *        - 400: 原始音量的 4 倍(自带溢出保护)。
 * @note
 *        - 调用本接口时音乐必须处于播放中状态。
 *        - 若设置的音量大于 400，则按最大值 400 进行调整；若设置的音量小于 0，则按最小值 0 进行调整。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 * @list 在线 KTV
 */

- (void)setMusicVolume:(NSString * _Nonnull)musicId volume:(int)volume;
/**
 * 
 * @type api
 * @brief 切换歌曲原唱伴唱。
 * @param musicId 音乐 ID。
 * @note 调用本接口时音乐必须处于播放中状态。
 * @list 在线 KTV
 */

- (void)switchAudioTrackType:(NSString * _Nonnull)musicId;
/**
 * 
 * @type api
 * @brief 对播放中的音乐设置升降调信息。
 * @param musicId 音乐 ID。
 * @param pitch 相对于音乐文件原始音调的升高/降低值，取值范围 [-12，12]，默认值为 0，即不做调整。 <br>
 *              取值范围内每相邻两个值的音高距离相差半音，正值表示升调，负值表示降调，设置的绝对值越大表示音调升高或降低越多。
 * @note
 *        - 调用本接口时音乐必须处于播放中状态。
 *        - 若设置的 pitch 大于 12，则按最大值 12 进行调整；若设置的 pitch 小于 –12，则按最小值 –12 进行调整。
 *        - 若音乐 ID 错误，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3023，playState 为 4。
 *        - 若未进房，会触发 ktvPlayer:onPlayStateChanged:state:error:{@link #ByteRTCKTVPlayerDelegate#ktvPlayer:onPlayStateChanged:state:error:} 回调，errorCode 为 –3022，playState 为 4。
 * @list 在线 KTV
 */

- (void)setMusicPitch:(NSString * _Nonnull)musicId pitch:(int)pitch;

@end

NS_ASSUME_NONNULL_END