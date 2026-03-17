/*
 * Copyright (c) 2021 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Audio Mixing Manager
*/

#pragma once

#include "bytertc_audio_frame.h"

namespace bytertc {
/**
 * 
 * @hidden(Linux)
 * @type callback
 * @brief 本地音频文件混音的音频帧观察者。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 自定义流处理
 */

class IAudioFileFrameObserver {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IAudioFileFrameObserver(){}
    /**
     * 
     * @type callback
     * @brief 当本地音频文件混音时，回调播放的音频帧。
     * @param mix_id 混音 ID。
     * @param audio_frame 参看 IAudioFrame{@link #IAudioFrame}。
     * @list 自定义流处理
     */
    
    virtual void onAudioFileFrame(int mix_id, const IAudioFrame& audio_frame) = 0;
};

}  