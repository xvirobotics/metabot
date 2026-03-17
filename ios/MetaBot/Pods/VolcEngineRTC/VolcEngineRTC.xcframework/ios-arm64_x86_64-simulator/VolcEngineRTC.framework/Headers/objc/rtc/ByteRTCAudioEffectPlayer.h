/*
 * Copyright (c) 2023 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCAudioEffectPlayer
*/

#import <Foundation/Foundation.h>
#import "ByteRTCAudioDefines.h"
/**
 * 
 * @type callback
 * @brief ByteRTCAudioEffectPlayer{@link #ByteRTCAudioEffectPlayer} 对应的回调句柄。你必须调用 setEventHandler:{@link #ByteRTCAudioEffectPlayer#setEventHandler:} 完成设置后，才能收到对应回调。
 * @list 
 */
  
@protocol ByteRTCAudioEffectPlayerEventHandler <NSObject>
/**
 * 
 * @type callback
 * @brief 播放状态改变时回调。
 * @param effectId ByteRTCAudioEffectPlayer{@link #ByteRTCAudioEffectPlayer} 的 ID。通过 getAudioEffectPlayer{@link #ByteRTCEngine#getAudioEffectPlayer} 设置。
 * @param state 混音状态。参考 ByteRTCPlayerState{@link #ByteRTCPlayerState}。
 * @param error 错误码。参考 ByteRTCPlayerError{@link #ByteRTCPlayerError}。
 * @list 混音
 */

-(void)onAudioEffectPlayerStateChanged:(int)effectId state:(ByteRTCPlayerState)state error:(ByteRTCPlayerError)error;

@end
/**
 * 
 * @valid since 3.53
 * @type api
 * @brief 音效播放器。 <br>
 * 注意： <br>
 *      - 调用 setEventHandler:{@link #ByteRTCAudioEffectPlayer#setEventHandler:} 设置回调句柄以获取相关回调。
 *      - 使用混音功能时，你必须通过 [setActive:withOptions:error:](https:
 * @list 混音
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCAudioEffectPlayer :NSObject
/**
 * 
 * @type api
 * @brief 开始播放音效文件。 <br>
 *        可以通过传入不同的 ID 和 filepath 多次调用本方法，以实现同时播放多个音效文件，实现音效叠加。
 * @param effectId 音效 ID。用于标识音效，请保证音效 ID 唯一性。 <br>
 *        如果使用相同的 ID 重复调用本方法后，上一个音效会停止，下一个音效开始，并收到 onAudioEffectPlayerStateChanged:state:error:{@link #ByteRTCAudioEffectPlayerEventHandler#onAudioEffectPlayerStateChanged:state:error:}。
 * @param filePath 音效文件路径。 <br>
 *        支持在线文件的 URL、本地文件的 URI、或本地文件的绝对路径。对于在线文件的 URL，仅支持 https 协议。 <br>
 *        推荐的音效文件采样率：8KHz、16KHz、22.05KHz、44.1KHz、48KHz。 <br>
 *        不同平台支持的本地音效文件格式: <br>
 *        <table>
 *           <tr><th></th><th>mp3</th><th>mp4</th><th>aac</th><th>m4a</th><th>3gp</th><th>wav</th><th>ogg</th><th>ts</th><th>wma</th></tr>
 *           <tr><td>Android</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td></tr>
 *           <tr><td>iOS/macOS</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td><td></td></tr>
 *           <tr><td>Windows</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td>Y</td><td>Y</td></tr>
 *        </table>
 *        不同平台支持的在线音效文件格式: <br>
 *        <table>
 *           <tr><th></th><th>mp3</th><th>mp4</th><th>aac</th><th>m4a</th><th>3gp</th><th>wav</th><th>ogg</th><th>ts</th><th>wma</th></tr>
 *           <tr><td>Android</td><td>Y</td><td></td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td><td></td></tr>
 *           <tr><td>iOS/macOS</td><td>Y</td><td></td><td>Y</td><td>Y</td><td></td><td>Y</td><td></td><td></td><td></td></tr>
 *           <tr><td>Windows</td><td>Y</td><td></td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td>Y</td><td>Y</td></tr>
 *        </table>
 * @param config 音效配置，详见 ByteRTCAudioEffectPlayerConfig{@link #ByteRTCAudioEffectPlayerConfig}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 如果已经通过 preload:filePath:{@link #ByteRTCAudioEffectPlayer#preload:filePath:} 将文件加载至内存，确保此处的 ID 与 `preload` 设置的 ID 相同。
 *       - 开始播放音效文件后，可以调用 stop:{@link #ByteRTCAudioEffectPlayer#stop:} 方法停止播放音效文件。
 * @list 混音
 */

-(int)start:(int)effectId filePath:(NSString * _Nullable)filePath config:(ByteRTCAudioEffectPlayerConfig * _Nullable)config;
/**
 * 
 * @type api
 * @brief 停止播放音效文件。
 * @param effectId 音效 ID
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 方法开始播放音效文件后，可以调用本方法停止播放音效文件。
 *       - 调用本方法停止播放音效文件后，该音效文件会被自动卸载。
 * @list 混音
 */

-(int)stop:(int)effectId;
/**
 * 
 * @type api
 * @brief 停止播放所有音效文件。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 方法开始播放音效文件后，可以调用本方法停止播放所有音效文件。
 *       - 调用本方法停止播放所有音效文件后，该音效文件会被自动卸载。
 * @list 混音
 */

-(int)stopAll;
/**
 * 
 * @type api
 * @brief 预加载指定音乐文件到内存中，以避免频繁播放同一文件时的重复加载，减少 CPU 占用。
 * @param effectId 音效 ID。用于标识音效，请保证音效 ID 唯一性。 <br>
 *        如果使用相同的 ID 重复调用本方法，后一次会覆盖前一次。 <br>
 *        如果先调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:}，再使用相同的 ID 调用本方法 ，会收到回调 onAudioEffectPlayerStateChanged:state:error:{@link #ByteRTCAudioEffectPlayerEventHandler#onAudioEffectPlayerStateChanged:state:error:}，通知前一个音效停止，然后加载下一个音效。 <br>
 *        调用本方法预加载 A.mp3 后，如果需要使用相同的 ID 调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 播放 B.mp3，请先调用 unload:{@link #ByteRTCAudioEffectPlayer#unload:} 卸载 A.mp3 ，否则会报错 AUDIO_MIXING_ERROR_LOAD_CONFLICT。
 * @param filePath 音效文件路径。支持本地文件的 URI、或本地文件的绝对路径。 <br>
 *                 预加载的文件长度不得超过 20s。 <br>
 *                 不同平台支持的音效文件格式和 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 一致。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 本方法只是预加载指定音效文件，只有调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 方法才开始播放指定音效文件。
 *       - 调用本方法预加载的指定音效文件可以通过 unload:{@link #ByteRTCAudioEffectPlayer#unload:} 卸载。
 * @list 混音
 */

-(int)preload:(int)effectId filePath:(NSString * _Nullable)filePath;
/**
 * 
 * @type api
 * @brief 卸载指定音效文件。
 * @param effectId 音效 ID
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 调用本方法卸载该文件后，关于当前的混音状态，如果设置了 setEventHandler:{@link #ByteRTCAudioEffectPlayer#setEventHandler:}，会收到回调 `onAudioEffectPlayerStateChanged`。
 * @list 混音
 */

-(int)unload:(int)effectId;
/**
 * 
 * @type api
 * @brief 卸载所有音效文件。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 调用本方法卸载该文件后，关于当前的混音状态，如果设置了 setEventHandler:{@link #ByteRTCAudioEffectPlayer#setEventHandler:}，会收到回调 `onAudioEffectPlayerStateChanged`。
 * @list 混音
 */

-(int)unloadAll;
/**
 * 
 * @type api
 * @brief 暂停播放音效文件。
 * @param effectId 音效 ID
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 方法开始播放音效文件后，可以通过调用本方法暂停播放音效文件。
 *       - 调用本方法暂停播放音效文件后，可调用 resume:{@link #ByteRTCAudioEffectPlayer#resume:} 方法恢复播放。
 * @list 混音
 */

-(int)pause:(int)effectId;
/**
 * 
 * @type api
 * @brief 暂停播放所有音效文件。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *       - 调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 方法开始播放音效文件后，可以通过调用本方法暂停播放所有音效文件。
 *       - 调用本方法暂停播放所有音效文件后，可调用 resumeAll{@link #ByteRTCAudioEffectPlayer#resumeAll} 方法恢复所有播放。
 * @list 混音
 */

-(int)pauseAll;
/**
 * 
 * @type api
 * @brief 恢复播放音效文件。
 * @param effectId 音效 ID
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 调用 pause:{@link #ByteRTCAudioEffectPlayer#pause:} 方法暂停播放音效文件后，可以通过调用本方法恢复播放。
 * @list 混音
 */

-(int)resume:(int)effectId;
/**
 * 
 * @type api
 * @brief 恢复播放所有音效文件。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 调用 pauseAll{@link #ByteRTCAudioEffectPlayer#pauseAll} 方法暂停所有正在播放音效文件后，可以通过调用本方法恢复播放。
 * @list 混音
 */

-(int)resumeAll;
/**
 * 
 * @type api
 * @brief 设置音效文件的起始播放位置。
 * @param effectId 音效 ID
 * @param position 音效文件起始播放位置，单位为毫秒。 <br>
 *        你可以通过 getDuration:{@link #ByteRTCAudioEffectPlayer#getDuration:} 获取音效文件总时长，position 的值应小于音效文件总时长。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 在播放在线文件时，调用此接口可能造成播放延迟的现象。
 *        - 仅在调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 后调用此接口。
 * @list 混音
 */

-(int)setPosition:(int)effectId position:(int)position;
/**
 * 
 * @type api
 * @brief 获取音效文件播放进度。
 * @param effectId 音效 ID
 * @return
 *        - >0: 成功, 音效文件播放进度，单位为毫秒。
 *        - < 0: 失败
 * @note
 *        - 在播放在线文件时，调用此接口可能造成播放延迟的现象。
 *        - 仅在调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 后调用此接口。
 * @list 混音
 */

-(int)getPosition:(int)effectId;
/**
 * 
 * @type api
 * @brief 调节指定音效的音量大小，包括音效文件和 PCM 音频。
 * @param effectId 音效 ID
 * @param volume 播放音量相对原音量的比值。单位为 %。范围为 `[0, 400]`，建议范围是 `[0, 100]`。带溢出保护。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 仅在调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 后调用此接口。
 * @list 混音
 */

-(int)setVolume:(int)effectId volume:(int)volume;
/**
 * 
 * @type api
 * @brief 设置所有音效的音量大小，包括音效文件和 PCM 音效。
 * @param volume 播放音量相对原音量的比值。单位为 %。范围为 `[0, 400]`，建议范围是 `[0, 100]`。带溢出保护。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 该接口的优先级低于 setVolume:volume:{@link #ByteRTCAudioEffectPlayer#setVolume:volume:}，即通过 `setVolume` 单独设置了音量的音效 ID，不受该接口设置的影响。
 * @list 混音
 */

-(int)setVolumeAll:(int)volume;
/**
 * 
 * @type api
 * @brief 获取当前音量。
 * @param effectId 音效 ID
 * @return
 *        - >0: 成功, 当前音量值。
 *        - < 0: 失败
 * @note 仅在调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 后调用此接口。
 * @list 混音
 */

-(int)getVolume:(int)effectId;
/**
 * 
 * @type api
 * @brief 获取音效文件时长。
 * @param effectId 音效 ID
 * @return
 *        - >0: 成功, 音效文件时长，单位为毫秒。
 *        - < 0: 失败
 * @note 仅在调用 start:filePath:config:{@link #ByteRTCAudioEffectPlayer#start:filePath:config:} 后调用此接口。
 * @list 混音
 */

-(int)getDuration:(int)effectId;
/**
 * 
 * @type api
 * @brief 设置回调句柄。
 * @param handler 参看 ByteRTCAudioEffectPlayerEventHandler{@link #ByteRTCAudioEffectPlayerEventHandler}。
 * @return
 *        - 0: 成功。
 *        - < 0: 失败。
 * @list 混音
 */

-(int)setEventHandler:(_Nullable id<ByteRTCAudioEffectPlayerEventHandler>)handler;
@end