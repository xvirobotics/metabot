/*
 * Copyright (c) 2023 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCMediaPlayer
*/

#import <Foundation/Foundation.h>
#import "ByteRTCAudioDefines.h"
/**
 * 
 * @type callback
 * @brief 本地音频文件混音的音频帧观察者。
 * @list 
 */

@protocol ByteRTCMediaPlayerAudioFrameObserver <NSObject>
/**
 * 
 * @type callback
 * @brief 当本地音频文件混音时，回调播放的音频帧。
 * @param playerId 播放器 ID
 * @param audioFrame 参看 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}。
 * @list 混音
 */

- (void)onFrame:(int)playerId audioFrame:(ByteRTCAudioFrame * _Nonnull)audioFrame;

@end
/**
 * 
 * @type callback
 * @brief ByteRTCMediaPlayer{@link #ByteRTCMediaPlayer} 对应的回调句柄。你必须调用 setEventHandler:{@link #ByteRTCMediaPlayer#setEventHandler:} 完成设置后，才能收到对应回调。
 * @list Audio Mixing
 */

@protocol ByteRTCMediaPlayerEventHandler <NSObject>
/**
 * 
 * @type callback
 * @brief 播放状态改变时回调。
 * @param playerId ByteRTCMediaPlayer{@link #ByteRTCMediaPlayer} 的 ID。通过 getMediaPlayer:{@link #ByteRTCEngine#getMediaPlayer:} 设置。
 * @param state 混音状态。参考 ByteRTCPlayerState{@link #ByteRTCPlayerState}。
 * @param error 错误码。参考 ByteRTCPlayerError{@link #ByteRTCPlayerError}。
 * @list 混音
 */

-(void)onMediaPlayerStateChanged:(int)playerId state:(ByteRTCPlayerState)state error:(ByteRTCPlayerError)error;
/**
 * 
 * @type callback
 * @brief 播放进度周期性回调。回调周期通过 setProgressInterval:{@link #ByteRTCMediaPlayer#setProgressInterval:} 设置。
 * @param playerId ByteRTCMediaPlayer{@link #ByteRTCMediaPlayer} 的 ID。通过 getMediaPlayer:{@link #ByteRTCEngine#getMediaPlayer:} 设置。
 * @param progress 进度。单位 ms。
 * @list 混音
 */

-(void)onMediaPlayerPlayingProgress:(int)playerId progress:(int64_t)progress;
/**
 * 
 * @valid since 3.59
 * @type callback
 * @brief 播放事件回调。调用 selectAudioTrack:{@link #ByteRTCMediaPlayer#selectAudioTrack:} 和 setPosition:{@link #ByteRTCMediaPlayer#setPosition:} 后，会触发此回调。
 * @param playerId ByteRTCMediaPlayer{@link #ByteRTCMediaPlayer} 的 ID。通过 getMediaPlayer:{@link #ByteRTCEngine#getMediaPlayer:} 设置。
 * @param event 播放器事件。参看 ByteRTCPlayerEvent{@link #ByteRTCPlayerEvent}。
 * @param message 事件描述信息，可能为空。
 * @list 混音
 */

-(void)onMediaPlayerEvent:(int)playerId event:(ByteRTCPlayerEvent)event message:(NSString*_Nullable)message;

@end
/**
 * 
 * @valid since 3.53
 * @type api
 * @brief 音乐播放器 <br>
 *        调用 setEventHandler:{@link #ByteRTCMediaPlayer#setEventHandler:} 设置回调句柄以获取相关回调。
 * @list Audio Mixing
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCMediaPlayer :NSObject
/**
 * 
 * @type api
 * @brief 打开音乐文件。 <br>
 *        一个播放器实例仅能够同时打开一个音乐文件。如果需要同时打开多个音乐文件，请创建多个音乐播放器实例。 <br>
 *        要播放 PCM 格式的音频数据，参看 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:}。`openWithCustomSource` 和此 API 互斥。
 * @param filePath 音乐文件路径。 <br>
 *        支持在线文件的 URL、本地文件的 URI、或本地文件的绝对路径。对于在线文件的 URL，仅支持 https 协议。 <br>
 *        推荐的采样率：8KHz、16KHz、22.05KHz、44.1KHz、48KHz。 <br>
 *        不同平台支持的本地文件格式: <br>
 *        <table>
 *           <tr><th></th><th>mp3</th><th>mp4</th><th>aac</th><th>m4a</th><th>3gp</th><th>wav</th><th>ogg</th><th>ts</th><th>wma</th></tr>
 *           <tr><td>Android</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td></tr>
 *           <tr><td>iOS/macOS</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td><td></td></tr>
 *           <tr><td>Windows</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td>Y</td><td>Y</td></tr>
 *        </table>
 *        不同平台支持的在线文件格式: <br>
 *        <table>
 *           <tr><th></th><th>mp3</th><th>mp4</th><th>aac</th><th>m4a</th><th>3gp</th><th>wav</th><th>ogg</th><th>ts</th><th>wma</th></tr>
 *           <tr><td>Android</td><td>Y</td><td></td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td><td></td></tr>
 *           <tr><td>iOS/macOS</td><td>Y</td><td></td><td>Y</td><td>Y</td><td></td><td>Y</td><td></td><td></td><td></td></tr>
 *           <tr><td>Windows</td><td>Y</td><td></td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td>Y</td><td>Y</td></tr>
 *        </table>
 * @param config 详见 ByteRTCMediaPlayerConfig{@link #ByteRTCMediaPlayerConfig}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @list 混音
 */

- (int)open:(NSString *_Nullable)filePath config:(ByteRTCMediaPlayerConfig *_Nullable)config;
/**
 * 
 * @type api
 * @brief 播放音乐。你仅需要在调用 open:config:{@link #ByteRTCMediaPlayer#open:config:}，且未开启自动播放时，调用此方法。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 * - 要播放 PCM 格式的音频数据，参看 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:}。`openWithCustomSource` 和此 API 互斥。
 * - 调用本方法播放音频文件后，可调用 stop{@link #ByteRTCMediaPlayer#stop} 方法暂停播放。
 * @list 混音
 */

- (int)start;
/**
 * 
 * @type api
 * @brief 启动音频裸数据混音。 <br>
 *        要播放音乐文件，参看 open:config:{@link #ByteRTCMediaPlayer#open:config:}。`open` 与此 API 互斥。
 * @param source 数据源，详见 ByteRTCMediaPlayerCustomSource{@link #ByteRTCMediaPlayerCustomSource}
 * @param config 详见 ByteRTCMediaPlayerConfig{@link #ByteRTCMediaPlayerConfig}
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 调用本方法启动后，再调用 pushExternalAudioFrame:{@link #ByteRTCEngine#pushExternalAudioFrame:} 推送音频数据，才会开始混音。
 *       - 如要结束 PCM 音频数据混音，调用 stop{@link #ByteRTCMediaPlayer#stop}。
 * @list 混音
 */

- (int)openWithCustomSource:(ByteRTCMediaPlayerCustomSource *_Nullable)source config:(ByteRTCMediaPlayerConfig *_Nullable)config;
/**
 * 
 * @type api
 * @brief 调用 open:config:{@link #ByteRTCMediaPlayer#open:config:}, start{@link #ByteRTCMediaPlayer#start}, 或 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:} 开始播放后，可以调用本方法停止。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @list 混音
 */

- (int)stop;
/**
 * 
 * @type api
 * @brief 调用 open:config:{@link #ByteRTCMediaPlayer#open:config:}，或 start{@link #ByteRTCMediaPlayer#start} 开始播放音频文件后，调用本方法暂停播放。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 * - 调用本方法暂停播放后，可调用 resume{@link #ByteRTCMediaPlayer#resume} 恢复播放。
 * - 此接口仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)pause;
/**
 * 
 * @type api
 * @brief 调用 pause{@link #ByteRTCMediaPlayer#pause} 暂停音频播放后，调用本方法恢复播放。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        此接口仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)resume;
/**
 * 
 * @type api
 * @brief 调节指定混音的音量大小，包括音乐文件混音和 PCM 混音。
 * @param volume 播放音量相对原音量的比值。单位为 %。范围为 `[0, 400]`，建议范围是 `[0, 100]`。带溢出保护。
 * @param type 详见 ByteRTCAudioMixingType{@link #ByteRTCAudioMixingType}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 如果在起播前调用此接口，设置的参数值将被缓存，在起播后生效。
 * @list 混音
 */
 
- (int)setVolume:(int)volume type:(ByteRTCAudioMixingType)type;
/**
 * 
 * @type api
 * @brief 获取当前音量
 * @param type 详见 ByteRTCAudioMixingType{@link #ByteRTCAudioMixingType}。
 * @return
 *        - >0: 成功, 当前音量值。
 *        - < 0: 失败
 * @note 仅在音频播放进行状态时，调用此方法。包括音乐文件混音和 PCM 混音。
 * @list 混音
 */

- (int)getVolume:(ByteRTCAudioMixingType)type;
/**
 * 
 * @type api
 * @brief 获取音乐文件时长。
 * @return
 *        - >0: 成功, 音乐文件时长，单位为毫秒。
 *        - < 0: 失败
 * @note
 *        - 仅在音频播放进行状态时，调用此方法。
 *        - 此接口仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)getTotalDuration;
/**
 * 
 * @type api
 * @brief 获取混音音乐文件的实际播放时长，单位为毫秒。
 * @return
 *        - >0: 实际播放时长。
 *        - < 0: 失败。
 * @note
 *        - 实际播放时长指的是歌曲不受停止、跳转、倍速、卡顿影响的播放时长。例如，若歌曲正常播放到 1:30 时停止播放 30s 或跳转进度到 2:00, 随后继续正常播放 2 分钟，则实际播放时长为 3 分 30 秒。
 *        - 仅在音频播放进行状态，且 setProgressInterval:{@link #ByteRTCMediaPlayer#setProgressInterval:} 设置间隔大于 `0` 时，调用此方法。
 *        - 此接口仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)getPlaybackDuration;
/**
 * 
 * @type api
 * @brief 获取音乐文件播放进度。
 * @return
 *        - >0: 成功, 音乐文件播放进度，单位为毫秒。
 *        - < 0: 失败
 * @note
 *        - 仅在音频播放进行状态时，调用此方法。
 *        - 此接口仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)getPosition;
/**
 * 
 * @valid since 3.59
 * @type api
 * @brief 获取播放器状态
 * @return 播放器当前状态，参看 ByteRTCPlayerState{@link #ByteRTCPlayerState}。
 * @note 仅在音频实例创建后，调用此方法。
 * @list 混音
 */

- (ByteRTCPlayerState)getState;
/**
 * 
 * @type api
 * @brief 开启变调功能，多用于 K 歌场景。
 * @param pitch 与音乐文件原始音调相比的升高/降低值，取值范围为 `[-12，12]`，默认值为 0。每相邻两个值的音高距离相差半音，正值表示升调，负值表示降调。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 如果在起播前调用此接口，设置的参数值将被缓存，在起播后生效。
 *        - 仅支持音乐文件混音，不支持 PCM 数据。
 * @list 混音
 */

- (int)setAudioPitch:(int)pitch;
/**
 * 
 * @type api
 * @brief 设置音乐文件的起始播放位置。
 * @param position 音乐文件起始播放位置，单位为毫秒。 <br>
 *        你可以通过 getTotalDuration{@link #ByteRTCMediaPlayer#getTotalDuration} 获取音乐文件总时长，position 的值应小于音乐文件总时长。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 此接口仅支持音频文件，不支持 PCM 数据。
 *        - 在播放在线文件时，调用此接口可能造成播放延迟的现象。
 *        - 调用本接口后，会收到 onMediaPlayerEvent:event:message:{@link #ByteRTCMediaPlayerEventHandler#onMediaPlayerEvent:event:message:} 回调。
 * @list 混音
 */

- (int)setPosition:(int)position;
/**
 * 
 * @type api
 * @brief 设置当前音乐文件的声道模式
 * @param mode 声道模式。默认的声道模式和源文件一致，详见 ByteRTCAudioMixingDualMonoMode{@link #ByteRTCAudioMixingDualMonoMode}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 如果在起播前调用此接口，设置的参数值将被缓存，在起播后生效。
 *        - 仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)setAudioDualMonoMode:(ByteRTCAudioMixingDualMonoMode)mode;
/**
 * 
 * @type api
 * @brief 获取当前音乐文件的音轨数
 * @return + >= 0：成功，返回当前音乐文件的音轨数
 *         - < 0：方法调用失败
 * @note
 *        - 仅在音频播放进行状态时，调用此方法。
 *        - 此方法仅支持音乐文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)getAudioTrackCount;
/**
 * 
 * @type api
 * @brief 指定当前音乐文件的播放音轨
 * @param index 指定的播放音轨，从 0 开始，取值范围为 `[0, getAudioTrackCount()-1]`。 <br>
 *        设置的参数值需要小于 getAudioTrackCount{@link #ByteRTCMediaPlayer#getAudioTrackCount} 的返回值
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 仅在音频播放进行状态时，调用此方法。
 *        - 此方法仅支持音乐文件，不支持 PCM 数据。
 *        - 调用本接口后，会收到 onMediaPlayerEvent:event:message:{@link #ByteRTCMediaPlayerEventHandler#onMediaPlayerEvent:event:message:} 回调。
 * @list 混音
 */

- (int)selectAudioTrack:(int)index;
/**
 * 
 * @type api
 * @brief 设置播放速度
 * @param speed 播放速度与原始文件速度的比例，单位：%，取值范围为 `[50,200]`，默认值为 100。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 如果在起播前调用此接口，设置的参数值将被缓存，在起播后生效。
 *        - 此方法对音频文件可用，不支持 PCM 数据。
 * @list 混音
 */

- (int)setPlaybackSpeed:(int)speed;
/**
 * 
 * @type api
 * @brief 设置音频文件混音时，收到 onMediaPlayerPlayingProgress:progress:{@link #ByteRTCMediaPlayerEventHandler#onMediaPlayerPlayingProgress:progress:} 的间隔。
 * @param interval 时间间隔，单位毫秒。 <br>
 *       - interval > 0 时，触发回调。实际间隔为 10 的倍数。如果输入数值不能被 10 整除，将自动向上取整。例如传入 `52`，实际间隔为 60 ms。
 *       - interval <= 0 时，不会触发回调。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 如果在起播前调用此接口，设置的参数值将被缓存，在起播后生效。
 *        - 此方法仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */
 
- (int)setProgressInterval:(int64_t)interval;
/**
 * 
 * @type api
 * @brief 如果你需要使用 enableVocalInstrumentBalance:{@link #ByteRTCEngine#enableVocalInstrumentBalance:} 对音频文件/PCM 音频数据设置音量均衡，你必须通过此接口传入其原始响度。
 * @param loudness 原始响度，单位：lufs，取值范围为 `[-70.0, 0.0]`。 <br>
 *        当设置的值小于 -70.0lufs 时，则默认调整为 -70.0lufs，大于 0.0lufs 时，则不对该响度做音量均衡处理。默认值为 1.0lufs，即不做处理。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 如果在起播前调用此接口，设置的参数值将被缓存，在起播后生效。
 *        - 此方法对音频文件和音频裸数据播放都可用。
 * @list 混音
 */

- (int)setLoudness:(float)loudness;
/**
 * 
 * @type api
 * @brief 注册回调句柄以在本地音乐文件混音时，收到相关回调。
 * @param observer 参看 ByteRTCMediaPlayerAudioFrameObserver{@link #ByteRTCMediaPlayerAudioFrameObserver}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        此接口仅支持音频文件，不支持 PCM 数据。
 * @list 混音
 */

- (int)registerAudioFrameObserver:(_Nullable id<ByteRTCMediaPlayerAudioFrameObserver>)observer;
/**
 * 
 * @type api
 * @brief 推送用于混音的 PCM 音频帧数据
 * @param audioFrame 音频帧，详见 ByteRTCAudioFrame{@link #ByteRTCAudioFrame}。 <br>
 *        - 音频采样格式必须为 S16。音频缓冲区内的数据格式必须为 PCM，其容量大小应该为 audioFrame.samples × audioFrame.channel × 2。
 *        - 必须指定具体的采样率和声道数，不支持设置为自动。
 * @return
 *       - 0: 成功
 *       - < 0: 失败
 * @note
 *      - 调用该方法前，须通过 openWithCustomSource:config:{@link #ByteRTCMediaPlayer#openWithCustomSource:config:} 启动外部音频流混音。
 *      - 使用参考建议：首次推送数据，请在应用侧先缓存一定数据（如 200 毫秒），然后一次性推送过去；此后的推送操作定时 10 毫秒一次，并且每次的音频数据量为 10 毫秒数据量。
 *      - 如果要暂停播放，暂停推送即可。
 * @list 混音
 */

- (int)pushExternalAudioFrame:(ByteRTCAudioFrame *_Nullable)audioFrame;
/**
 * 
 * @type api
 * @brief 设置回调句柄。
 * @param handler 参看 ByteRTCMediaPlayerEventHandler{@link #ByteRTCMediaPlayerEventHandler}。
 * @return
 *        - 0: 成功。
 *        - < 0: 失败。
 * @list 混音
 */

- (int)setEventHandler:(_Nullable id<ByteRTCMediaPlayerEventHandler>)handler;
@end
