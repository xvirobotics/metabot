/*
 * Copyright (c) 2023 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Audio Effect Player
*/
#pragma once

#include "bytertc_audio_frame.h"

namespace bytertc {
/**
 * 
 * @type callback
 * @brief IAudioEffectPlayer{@link #IAudioEffectPlayer} 对应的回调句柄。你必须调用 setEventHandler{@link #IAudioEffectPlayer#setEventHandler} 完成设置后，才能收到对应回调。
 * @list 混音
 */

class IAudioEffectPlayerEventHandler {
public:
    virtual ~IAudioEffectPlayerEventHandler() {}
    /**
     * 
     * @type callback
     * @brief 播放状态改变时回调。
     * @param effect_id IAudioEffectPlayer{@link #IAudioEffectPlayer} 的 ID。通过 getAudioEffectPlayer{@link #IRTCEngine#getAudioEffectPlayer} 设置。
     * @param state 混音状态。参考 PlayerState{@link #PlayerState}。
     * @param error 错误码。参考 PlayerError{@link #PlayerError}。
     * @list 混音
     */
    
    virtual void onAudioEffectPlayerStateChanged(int effect_id, PlayerState state, PlayerError error) = 0;
};
/**
 * 
 * @valid since 3.53
 * @type api
 * @brief 音效播放器 <br>
 *        调用 setEventHandler{@link #IAudioEffectPlayer#setEventHandler} 设置回调句柄以获取相关回调。
 * @list 混音
 */

class IAudioEffectPlayer {
public:
    IAudioEffectPlayer(){}
    virtual ~IAudioEffectPlayer() {}
    /**
     * 
     * @type api
     * @brief 开始播放音效文件。 <br>
     *        可以通过传入不同的 ID 和 filepath 多次调用本方法，以实现同时播放多个音效文件，实现音效叠加。
     * @param effect_id 音效 ID。用于标识音效，请保证音效 ID 唯一性。 <br>
     *        如果使用相同的 ID 重复调用本方法后，上一个音效会停止，下一个音效开始，并收到 onAudioEffectPlayerStateChanged{@link #IAudioEffectPlayerEventHandler#onAudioEffectPlayerStateChanged}。
     * @param file_path 音效文件路径。 <br>
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
     * @param config 音效配置，详见 AudioEffectPlayerConfig{@link #AudioEffectPlayerConfig}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 如果已经通过 preload{@link #IAudioEffectPlayer#preload} 将文件加载至内存，确保此处的 ID 与 preload{@link #IAudioEffectPlayer#preload} 设置的 ID 相同。
     *       - 开始播放音效文件后，可以调用 stop{@link #IAudioEffectPlayer#stop} 方法停止播放音效文件。
     * @list 混音
     */
    
    virtual int start(int effect_id, const char* file_path, const AudioEffectPlayerConfig& config) = 0;
    /**
     * 
     * @type api
     * @brief 停止播放音效文件。
     * @param effect_id 音效 ID
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用 start{@link #IAudioEffectPlayer#start} 方法开始播放音效文件后，可以调用本方法停止播放音效文件。
     *       - 调用本方法停止播放音效文件后，该音效文件会被自动卸载。
     * @list 混音
     */
    
    virtual int stop(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 停止播放所有音效文件。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用 start{@link #IAudioEffectPlayer#start} 方法开始播放音效文件后，可以调用本方法停止播放所有音效文件。
     *       - 调用本方法停止播放所有音效文件后，该音效文件会被自动卸载。
     * @list 混音
     */
    
    virtual int stopAll() = 0;
    /**
     * 
     * @type api
     * @brief 预加载指定音乐文件到内存中，以避免频繁播放同一文件时的重复加载，减少 CPU 占用。
     * @param effect_id 音效 ID。用于标识音效，请保证音效 ID 唯一性。 <br>
     *        如果使用相同的 ID 重复调用本方法，后一次会覆盖前一次。 <br>
     *        如果先调用 start{@link #IAudioEffectPlayer#start}，再使用相同的 ID 调用本方法 ，会收到回调 onAudioEffectPlayerStateChanged{@link #IAudioEffectPlayerEventHandler#onAudioEffectPlayerStateChanged} ，通知前一个音效停止，然后加载下一个音效。 <br>
     *        调用本方法预加载 A.mp3 后，如果需要使用相同的 ID 调用 start{@link #IAudioEffectPlayer#start} 播放 B.mp3，请先调用 unload{@link #IAudioEffectPlayer#unload} 卸载 A.mp3 ，否则会报错 AUDIO_MIXING_ERROR_LOAD_CONFLICT。
     * @param file_path 音效文件路径。支持本地文件的 URI。 <br>
     *                 预加载的文件长度不得超过 20s。 <br>
     *                 不同平台支持的音效文件格式和 start{@link #IAudioEffectPlayer#start} 一致。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 本方法只是预加载指定音效文件，只有调用 start{@link #IAudioEffectPlayer#start} 方法才开始播放指定音效文件。
     *       - 调用本方法预加载的指定音效文件可以通过 unload{@link #IAudioEffectPlayer#unload} 卸载。
     * @list 混音
     */
    
    virtual int preload(int effect_id, const char* file_path) = 0;
    /**
     * 
     * @type api
     * @brief 卸载指定音效文件。
     * @param effect_id 音效 ID
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 仅在调用 start{@link #IAudioEffectPlayer#start} 或 preload{@link #IAudioEffectPlayer#preload} 后调用此接口。
     * @list 混音
     */
    
    virtual int unload(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 卸载所有音效文件。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @list 混音
     */
    
    virtual int unloadAll() = 0;
   /**
    * 
    * @type api
    * @brief 暂停播放音效文件。
    * @param effect_id 音效 ID
    * @return
    *        - 0: 调用成功。
    *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
    * @note
    *       - 调用 start{@link #IAudioEffectPlayer#start} 方法开始播放音效文件后，可以通过调用本方法暂停播放音效文件。
    *       - 调用本方法暂停播放音效文件后，可调用 resume{@link #IAudioEffectPlayer#resume} 方法恢复播放。
    * @list 混音
    */
    
    virtual int pause(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 暂停播放所有音效文件。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用 start{@link #IAudioEffectPlayer#start} 方法开始播放音效文件后，可以通过调用本方法暂停播放所有音效文件。
     *       - 调用本方法暂停播放所有音效文件后，可调用 resumeAll{@link #IAudioEffectPlayer#resumeAll} 方法恢复所有播放。
     * @list 混音
     */
    
    virtual int pauseAll() = 0;
    /**
     * 
     * @type api
     * @brief 恢复播放音效文件。
     * @param effect_id 音效 ID
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 调用 pause{@link #IAudioEffectPlayer#pause} 方法暂停播放音效文件后，可以通过调用本方法恢复播放。
     * @list 混音
     */
    
    virtual int resume(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 恢复播放所有音效文件。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 调用 pauseAll{@link #IAudioEffectPlayer#pauseAll} 方法暂停所有正在播放音效文件后，可以通过调用本方法恢复播放。
     * @list 混音
     */
    
    virtual int resumeAll() = 0;
    /**
     * 
     * @type api
     * @brief 设置音效文件的起始播放位置。
     * @param effect_id 音效 ID
     * @param pos 音效文件起始播放位置，单位为毫秒。 <br>
     *        你可以通过 getDuration{@link #IAudioEffectPlayer#getDuration} 获取音效文件总时长，pos 的值应小于音效文件总时长。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 在播放在线文件时，调用此接口可能造成播放延迟的现象。
     *        - 仅在调用 start{@link #IAudioEffectPlayer#start} 后调用此接口。
     * @list 混音
     */
    
    virtual int setPosition(int effect_id, int pos) = 0;
    /**
     * 
     * @type api
     * @brief 获取音效文件播放进度。
     * @param effect_id 音效 ID
     * @return
     *        - >0: 成功, 音效文件播放进度，单位为毫秒。
     *        - < 0: 失败
     * @note
     *        - 在播放在线文件时，调用此接口可能造成播放延迟的现象。
     *        - 仅在调用 start{@link #IAudioEffectPlayer#start} 后调用此接口。
     * @list 混音
     */
    
    virtual int getPosition(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 调节指定音效的音量大小，包括音效文件和 PCM 音频。
     * @param effect_id 音效 ID
     * @param volume 播放音量相对原音量的比值。单位为 %。范围为 `[0, 400]`，建议范围是 `[0, 100]`。带溢出保护。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 仅在调用 start{@link #IAudioEffectPlayer#start} 后调用此接口。
     * @list 混音
     */
    
    virtual int setVolume(int effect_id, int volume) = 0;
    /**
     * 
     * @type api
     * @brief 设置所有音效的音量大小，包括音效文件和 PCM 音效。
     * @param volume 播放音量相对原音量的比值。单位为 %。范围为 `[0, 400]`，建议范围是 `[0, 100]`。带溢出保护。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该接口的优先级低于 setVolume{@link #IAudioEffectPlayer#setVolume}，即通过 setVolume{@link #IAudioEffectPlayer#setVolume} 单独设置了音量的音效 ID，不受该接口设置的影响。
     * @list 混音
     */
    
    virtual int setVolumeAll(int volume) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音量。
     * @param effect_id 音效 ID
     * @return
     *        - >0: 成功, 当前音量值。
     *        - < 0: 失败
     * @note 仅在调用 start{@link #IAudioEffectPlayer#start} 后调用此接口。
     * @list 混音
     */
    
    virtual int getVolume(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 获取音效文件时长。
     * @param effect_id 音效 ID
     * @return
     *        - >0: 成功, 音效文件时长，单位为毫秒。
     *        - < 0: 失败
     * @note 仅在调用 start{@link #IAudioEffectPlayer#start} 后调用此接口。
     * @list 混音
     */
    
    virtual int getDuration(int effect_id) = 0;
    /**
     * 
     * @type api
     * @brief 设置回调句柄。
     * @param handler 参看 IAudioEffectPlayerEventHandler{@link #IAudioEffectPlayerEventHandler}。
     * @return
     *        - 0: 成功。
     *        - < 0: 失败。
     * @list 混音
     */
    
    virtual int setEventHandler(IAudioEffectPlayerEventHandler* handler) = 0;
};

}
