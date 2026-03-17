/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC audio Frame
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <memory>
#include "bytertc_audio_defines.h"

namespace bytertc {

/**
 * 
 * @type keytype
 * @region 音频管理
 * @brief 音频帧类型
 */

enum AudioFrameType {
    /**
     * 
     * @brief PCM 16bit
     */
    
    kAudioFrameTypePCM16 = 0
};

/**
 * 
 * @type keytype
 * @region 音频管理
 * @brief 音频帧构建类
 */

typedef struct AudioFrameBuilder {
    /**
     * 
     * @brief 音频采样率
     */
    
    AudioSampleRate sample_rate;

    /**
     * 
     * @brief 音频通道数
     */
    
    AudioChannel channel;

    /**
     * 
     * @brief 音频帧时间戳，单位：微秒
     * @note 此时间戳不是 Linux 时间戳，只是 SDK 内部时间戳，仅保证帧之间时间戳的相对关系。
     */
    
    int64_t timestamp_us = 0;

    /**
     * 
     * @brief 音频帧数据
     */
    
    uint8_t* data;

    /**
     * 
     * @brief 音频帧数据大小
     */
    
    int64_t data_size = 0;

    /**
     * 
     * @brief 是否深拷贝
     */
    
    bool deep_copy = true;
    /**
     * 
     * @hidden for internal use only
     * @brief 音频帧额外信息数据
     */
    
    uint8_t* extra_info = nullptr;
    /**
     * 
     * @hidden for internal use only
     * @brief 音频帧额外信息数据大小
     */
    
    int64_t extra_info_size = 0;
} AudioFrameBuilder;
/**
 * 
 * @type keytype
 * @brief 音频帧
 */

class IAudioFrame {
public:
    /**
     * 
     * @type api
     * @brief 获取音频帧时间戳。
     * @return 音频帧时间戳，单位：微秒
     * @list 音频管理
     */
    
    virtual int64_t timestampUs() const = 0;
    /**
     * 
     * @type api
     * @brief 获取音频采样率。参看 AudioSampleRate{@link #AudioSampleRate}
     * @return 音频采样率，单位：Hz
     * @list 音频管理
     */
    
    virtual AudioSampleRate sampleRate() const = 0;
    /**
     * 
     * @type api
     * @brief 获取音频通道数。参看 AudioChannel{@link #AudioChannel}
     * @return 音频通道数
     * @note 双声道的情况下，左右声道的音频帧数据以 LRLRLR 形式排布。
     * @list 音频管理
     */
    
    virtual AudioChannel channel() const = 0;
    /**
     * 
     * @type api
     * @brief 获取音频帧内存块地址
     * @return 音频帧内存块地址
     * @list 音频管理
     */
    
    virtual uint8_t* data() const = 0;
    /**
     * 
     * @type api
     * @brief 获取音频帧数据大小
     * @return 音频帧数据大小，单位：字节。
     * @list 音频管理
     */
    
    virtual int dataSize() const = 0;
    /**
     * 
     * @type api
     * @brief 获取音频帧类型
     * @return 音频帧类型，目前只支持 PCM，详见 AudioFrameType{@link #AudioFrameType}
     * @list 音频管理
     */
    
    virtual AudioFrameType frameType() const = 0;
    /**
     * 
     * @type api
     * @hidden for internal use only
     * @brief 获取音频帧额外信息内存块地址
     * @return 音频帧额外信息内存块地址
     * @list 音频管理
     */
        
    virtual uint8_t* extraInfo() const = 0;
    /**
     * 
     * @type api
     * @hidden for internal use only
     * @brief 获取音频帧额外信息大小
     * @return 音频帧数据额外信息大小，单位：字节。
     * @list 音频管理
     */
    
    virtual int extraInfoSize() const = 0;
    /**
     * 
     * @type api
     * @brief 释放音频帧
     * @list 音频管理
     */
    
    virtual void release() = 0;
    /**
     * 
     * @type api
     * @brief 获取音频静音标志
     * @return 是否静音数据 <br>
     *        - true: 是
     *        - false: 否
     * @list 音频管理
     */
    
    virtual bool isMutedData() const = 0;
    /**
     * 
     * @hidden constructor/destructor
     */
    
protected:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IAudioFrame() = default;
};
/**
 * 
 * @type api
 * @brief 创建 IAudioFrame
 * @param builder 音频帧构建实例，参看 AudioFrameBuilder{@link #AudioFrameBuilder}
 * @return 详见 IAudioFrame{@link #IAudioFrame}
 * @list 音频管理
 */

BYTERTC_API IAudioFrame* buildAudioFrame(const AudioFrameBuilder& builder);

/**
 * 
 * @type keytype
 * @brief 音频回调方法
 */

enum class AudioFrameCallbackMethod{
    /**
     * 
     * @brief 本地麦克风录制的音频数据回调
     */
    
    kRecord,
    /**
     * 
     * @brief 订阅的远端所有用户混音后的音频数据回调
     */
    
    kPlayback,
    /**
     * 
     * @brief 本地麦克风录制和订阅的远端所有用户混音后的音频数据回调
     */
    
    kMixed,
    /**
     * 
     * @brief 订阅的远端每个用户混音前的音频数据回调
     */
    
    kRemoteUser,
    /**
     * 
     * @brief 本地屏幕录制的音频数据回调
     */
    
    kRecordScreen,
    /**
     * 
     * @brief 本地麦克风录制和本地 `MediaPlayer`, `EffectPlayer` 播放的音频混音后的音频数据回调
     */
    
    kCaptureMixed,
};
/**
 * 
 * @type callback
 * @brief 音频数据回调观察者 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。 <br>
 * 本接口类中的回调周期均为 20 ms。
 * @list 自定义流处理
 */

class IAudioFrameObserver {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IAudioFrameObserver() {
    }
    /**
     * 
     * @hidden for internal use only
     * @valid since 3.50
     */
    
    virtual void onRecordAudioFrameOriginal(const IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 返回麦克风录制的音频数据
     * @param audio_frame 音频数据, 详见：IAudioFrame{@link #IAudioFrame}
     * @list 自定义流处理
     */
    
    virtual void onRecordAudioFrame(const IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 返回订阅的所有远端用户混音后的音频数据。
     * @param audio_frame 音频数据, 详见：IAudioFrame{@link #IAudioFrame}
     * @list 自定义流处理
     */
    
    virtual void onPlaybackAudioFrame(const IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 返回远端单个用户的音频数据
     * @param stream_id 远端流 ID。
     * @param stream_info 远端流信息，参看 StreamInfo{@link #StreamInfo}。
     * @param audio_frame 音频数据， 参看 IAudioFrame{@link #IAudioFrame}。
     * @note 此回调在播放线程调用。不要在此回调中做任何耗时的事情，否则可能会影响整个音频播放链路。
     * @list 自定义流处理
     */
    
    virtual void onRemoteUserAudioFrame(const char* stream_id, const StreamInfo& stream_info, const IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 返回本地麦克风录制和订阅的所有远端用户混音后的音频数据
     * @param audio_frame 音频数据, 详见：IAudioFrame{@link #IAudioFrame}
     * @list 自定义流处理
     */
    
    virtual void onMixedAudioFrame(const IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 返回本地屏幕录制的音频数据
     * @param audio_frame 音频数据, 详见：IAudioFrame{@link #IAudioFrame}
     * @list 自定义流处理
     */
    
    virtual void onRecordScreenAudioFrame(const IAudioFrame& audio_frame) {
    }
    /**
     * 
     * @type callback
     * @brief 返回本地麦克风录制的音频数据，本地 `MediaPlayer` / `EffectPlayer` 播放音频文件混音后的音频数据
     * @param audio_frame 音频数据, 详见：IAudioFrame{@link #IAudioFrame}
     * @list 自定义流处理
     */
     
    virtual void onCaptureMixedAudioFrame(const IAudioFrame& audio_frame) = 0;

};
/**
 * 
 * @type callback
 * @brief 自定义音频处理器。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list Custom Stream Processing
 */

class IAudioFrameProcessor{
public:
    /**
     * 
     * @type callback
     * @brief 回调本地采集的音频帧地址，供自定义音频处理。
     * @param audio_frame 音频帧地址，参看 IAudioFrame{@link #IAudioFrame}
     * @note
     *        - 完成自定义音频处理后，SDK 会对处理后的音频帧进行编码，并传输到远端。
     *        - 调用 enableAudioProcessor{@link #IRTCEngine#enableAudioProcessor}，并在参数中选择本地采集的音频时，每 10 ms 收到此回调。
     * @list 自定义流处理
     */
    
    virtual int onProcessRecordAudioFrame(IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 回调远端音频混音的音频帧地址，供自定义音频处理。
     * @param audio_frame 音频帧地址，参看 IAudioFrame{@link #IAudioFrame}
     * @note 调用 enableAudioProcessor{@link #IRTCEngine#enableAudioProcessor}，并在参数中选择远端音频流的的混音音频时，每 10 ms 收到此回调。
     * @list 自定义流处理
     */
    
    virtual int onProcessPlayBackAudioFrame(IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 回调单个远端用户的音频帧地址，供自定义音频处理。
     * @param stream_id 远端流 ID。
     * @param stream_info 远端流信息， 参看 StreamInfo{@link #StreamInfo}。
     * @param audio_frame 音频帧地址，参看 IAudioFrame{@link #IAudioFrame}
     * @note 调用 enableAudioProcessor{@link #IRTCEngine#enableAudioProcessor}，并在参数中选择各个远端音频流时，每 10 ms 收到此回调。
     * @list 自定义流处理
     */
    
    virtual int onProcessRemoteUserAudioFrame(const char* stream_id, const StreamInfo& stream_info, IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @hidden(macOS, Windows, Linux)
     * @valid since 3.50
     * @type callback
     * @brief 软件耳返音频数据的回调。你可根据此回调自定义处理音频。 <br>
     *        耳返音频中包含通过调用 `setVoiceReverbType` 和 `setVoiceChangerType` 设置的音频特效。
     * @param audio_frame 音频帧地址。参看 IAudioFrame{@link #IAudioFrame}。
     * @return
     *        - 0： 成功。
     *        - < 0： 失败。
     * @note
     *        - 此数据处理只影响软件耳返音频数据。
     *        - 要启用此回调，必须调用 enableAudioProcessor{@link #IRTCEngine#enableAudioProcessor}，并选择耳返音频，每 10 ms 收到此回调。
     * @list Custom Stream Processing
     */
    
    virtual int onProcessEarMonitorAudioFrame(IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @type callback
     * @brief 屏幕共享的音频帧地址回调。你可根据此回调自定义处理音频。
     * @param audio_frame 音频帧地址，参看 IAudioFrame{@link #IAudioFrame}。
     * @note 调用 enableAudioProcessor{@link #IRTCEngine#enableAudioProcessor}，把返回给音频处理器的音频类型设置为屏幕共享音频后，每 10 ms 收到此回调。
     * @list 自定义流处理
     */
    
    virtual int onProcessScreenAudioFrame(IAudioFrame& audio_frame) = 0;
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IAudioFrameProcessor() {
    }
};
/**
 * 
 * @type keytype
 * @hidden for internal use only
 * @region 音频管理
 * @brief 音频帧构建类
 */

typedef struct EncodedAudioFrameData {
    /**
     * 
     * @brief 音频编码类型
     */
    
    AudioCodecType codec_type = kAudioCodecTypeNone;
    /**
     * 
     * @brief 音频数据
     */
    
    uint8_t* data = nullptr;
    /**
     * 
     * @brief 音频数据大小
     */
    
    int size = 0;
    /**
     * 
     * @brief 音频时间戳，单位：微秒
     */
    
    int64_t timestamp_us = 0;
    /**
     * 
     * @brief 音频采样率
     */
    
    int sample_rate = 0;
    /**
     * 
     * @brief 音频通道数
     */
    
    int channel_num = 0;
    /**
     * 
     * @brief 音频帧大小，单位：毫秒
     */
    
    int frame_size_ms = 0;
    /**
     * 
     * @hidden for internal use only
     * @brief 音频帧额外信息数据
     */
    
    uint8_t* extra_info = nullptr;
    /**
     * 
     * @hidden for internal use only
     * @brief 音频帧额外信息数据大小
     */
    
    int extra_info_size = 0;
} EncodedAudioFrameData;

/**
 * 
 * @type callback
 * @hidden for internal use only
 * @brief 远端音频帧监测器
 * @list 自定义流处理
 */

class IRemoteEncodedAudioFrameObserver  {
public:
    virtual ~IRemoteEncodedAudioFrameObserver () {
    }
    /**
     * 
     * @type callback
     * @hidden for internal use only
     * @brief 调用 RegisterRemoteEncodedAudioFrameObserver 后，SDK 收到远端音频帧信息时，回调该事件
     * @param stream_id 远端流 ID
     * @param stream_info 远端流信息，参看 StreamInfo{@link #StreamInfo}
     * @param audio_stream 远端音频帧信息，参看 EncodedAudioFrameData{@link #EncodedAudioFrameData}
     * @list 自定义流处理
     */
    
    virtual void onRemoteEncodedAudioFrame(const char* stream_id, 
            const StreamInfo& stream_info, const EncodedAudioFrameData& audio_stream) = 0;
};

}  
