/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Audio Device Manager
*/

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {
/**
 * 
 * @deprecated since 3.42 and will be deleted in 3.51, use IAudioDeviceCollection{@link #IAudioDeviceCollection} and IVideoDeviceCollection{@link #IVideoDeviceCollection} instead.
 * @type api
 * @brief 音视频设备相关的信息
 * @list 引擎管理
 */

class IDeviceCollection {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    IDeviceCollection() {
    }
    /**
     * 
     * @type api
     * @brief 获取当前系统内音视频设备数量
     * @return 音视频设备数量
     * @list 引擎管理
     */
    
    virtual int getCount() = 0;
    /**
     * 
     * @type api
     * @brief 根据索引号，获取设备信息
     * @param index 设备索引号，从 0 开始，注意需小于 getCount{@link #IAudioDeviceCollection#getCount} 返回值。
     * @param device_name 设备名称
     * @param device_id 设备 ID
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 引擎管理
     */
    
    virtual int getDevice(int index, char device_name[MAX_DEVICE_ID_LENGTH], char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 释放当前 IAudioDeviceCollection{@link #IAudioDeviceCollection} 对象占用的资源。
     * @note 不需要返回音视频设备相关信息列表时应该调用本方法释放相关资源。
     * @list 引擎管理
     */
    
    virtual void release() = 0;
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
    
    virtual ~IDeviceCollection() {
    }
};
/**
 * 
 * @hidden(Android,iOS)
 * @type api
 * @brief 音频设备相关的信息
 * @list 引擎管理
 */

class IAudioDeviceCollection : public IDeviceCollection {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    IAudioDeviceCollection() {};
    /**
     * 
     * @type api
     * @brief 获取当前系统内音频设备数量
     * @return 音频设备数量
     * @list 引擎管理
     */
    
    virtual int getCount() override {return 0;};
    /**
     * 
     * @type api
     * @brief 根据索引号，获取设备信息
     * @param index 设备索引号，从 0 开始，注意需小于 getCount{@link #IAudioDeviceCollection#getCount} 返回值。
     * @param device_name 设备名称
     * @param device_id 设备 ID
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 引擎管理
     */
    
    virtual int getDevice(int index, char device_name[MAX_DEVICE_ID_LENGTH], char device_id[MAX_DEVICE_ID_LENGTH]) override {return 0;};
    /**
     * 
     * @type api
     * @brief 释放当前 IAudioDeviceCollection{@link #IAudioDeviceCollection} 对象占用的资源。
     * @note 不需要返回音频设备相关信息列表时应该调用本方法释放相关资源。
     * @list 引擎管理
     */
    
    virtual void release()override {};
    /**
     * 
     * @type api
     * @brief 根据索引号，获取设备信息
     * @param index 设备索引号，从 0 开始，注意需小于 getCount{@link #IAudioDeviceCollection#getCount} 返回值。
     * @param audio_device_info 设备信息，详见 AudioDeviceInfo{@link #AudioDeviceInfo}
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 引擎管理
     */
    
    virtual int getDevice(int index, AudioDeviceInfo* audio_device_info) = 0;
protected:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IAudioDeviceCollection() {};
};
/**
 * 
 * @hidden(Android,iOS)
 * @type api
 * @brief 音频设备管理类
 * @list 引擎管理
 */

class IAudioDeviceManager {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    IAudioDeviceManager() {
    }
    /**
     * 
     * @type api
     * @brief 获取当前系统内音频播放设备列表。如果后续设备有变更，你会收到 `onAudioMediaDeviceStateChanged` 回调通知，然后需要重新调用本接口以获得新的设备列表。
     * @return 包含系统中所有音频播放设备的列表，参看 IAudioDeviceCollection{@link #IAudioDeviceCollection}。 <br>
     * 等待超时后会返回空列表。超时时间默认为 10 s。建议通过 onAudioDeviceStateChanged{@link #IRTCEngineEventHandler#onAudioDeviceStateChanged} 监听到 `kMediaDeviceListUpdated` 后，再次调用本接口获取。
     * @note 你可以在收到 onAudioDeviceStateChanged{@link #IRTCEngineEventHandler#onAudioDeviceStateChanged} 了解设备变更后，重新调用本接口以获得新的设备列表。
     * @list 音频管理
     */
    
    virtual IAudioDeviceCollection* enumerateAudioPlaybackDevices() = 0;
    /**
     * 
     * @type api
     * @brief 获取当前系统内音频采集设备列表。如果后续设备有变更，你需要重新调用本接口以获得新的设备列表。
     * @return 一个包含系统中所有音频采集设备列表的对象，详见 IAudioDeviceCollection{@link #IAudioDeviceCollection}。 <br>
     * 等待超时后会返回空列表。超时时间默认为 10 s。建议通过 onAudioDeviceStateChanged{@link #IRTCEngineEventHandler#onAudioDeviceStateChanged} 监听到 `kMediaDeviceListUpdated` 后，再次调用本接口获取。
     * @note 
     *       - 你可以在收到 onAudioDeviceStateChanged{@link #IRTCEngineEventHandler#onAudioDeviceStateChanged} 了解设备变更后，重新调用本接口以获得新的设备列表。
     *       - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual IAudioDeviceCollection* enumerateAudioCaptureDevices() = 0;
    /**
     * 
     * @type api
     * @brief 设置音频播放路由是否跟随系统。
     * @param followed <br>
     *        - true: 跟随。此时，调用 setAudioPlaybackDevice{@link #IAudioDeviceManager#setAudioPlaybackDevice} 会失败。默认值。
     *        - false: 不跟随系统。此时，可以调用 setAudioPlaybackDevice{@link #IAudioDeviceManager#setAudioPlaybackDevice} 进行设置。
     * @list 音频管理
     */
    
    virtual void followSystemPlaybackDevice(bool followed) = 0;
    /**
     * 
     * @type api
     * @brief 设置音频采集路由是否跟随系统。
     * @param followed <br>
     *        - true: 跟随。此时，调用 setAudioCaptureDevice{@link #IAudioDeviceManager#setAudioCaptureDevice} 会失败。默认值。
     *        - false: 不跟随系统。此时，可以调用 setAudioCaptureDevice{@link #IAudioDeviceManager#setAudioCaptureDevice} 进行设置。
     * @note 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual void followSystemCaptureDevice(bool followed) = 0;
    /**
     * 
     * @type api
     * @brief 设置音频播放设备。
     * @param device_id 音频播放设备 ID，可通过 enumerateAudioPlaybackDevices{@link #IAudioDeviceManager#enumerateAudioPlaybackDevices} <br>
     * 获取。
     * @return
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 当你调用 followSystemPlaybackDevice{@link #IAudioDeviceManager#followSystemPlaybackDevice} 设置音频播放设备跟随系统后，将无法调用此接口设置音频播放设备。
     * @list 音频管理
     */
    
    virtual int setAudioPlaybackDevice(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 设置音频采集设备。
     * @param device_id 音频采集设备 ID，可通过 enumerateAudioCaptureDevices{@link #IAudioDeviceManager#enumerateAudioCaptureDevices} 获取。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 
     *        - 当你调用 followSystemCaptureDevice{@link #IAudioDeviceManager#followSystemCaptureDevice} 设置音频采集设备跟随系统后，将无法调用此接口设置音频采集设备。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int setAudioCaptureDevice(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 设置当前音频播放设备音量
     * @param volume 音频播放设备音量，取值范围为 [0,255], 超出此范围设置无效。 <br>
     *       - [0,25] 接近无声；
     *       - [25,75] 为低音量；
     *       - [76,204] 为中音量；
     *       - [205,255] 为高音量。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @list 音频管理
     */
    
    virtual int setAudioPlaybackDeviceVolume(unsigned int volume) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音频播放设备音量
     * @param volume 音频播放设备音量，范围应在 [0,255] 内。 <br>
     *       - [0,25] 接近无声；
     *       - [25,75] 为低音量；
     *       - [76,204] 为中音量；
     *       - [205,255] 为高音量。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @list 音频管理
     */
    
    virtual int getAudioPlaybackDeviceVolume(unsigned int* volume) = 0;
    /**
     * 
     * @type api
     * @brief 设置当前音频采集设备音量
     * @param volume 音频采集设备音量，取值范围为 [0,255], 超出此范围设置无效。 <br>
     *       - [0,25] 接近无声；
     *       - [25,75] 为低音量；
     *       - [76,204] 为中音量；
     *       - [205,255] 为高音量。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功。将影响 onLocalAudioPropertiesReport{@link #IRTCEngineEventHandler#onLocalAudioPropertiesReport} 回调的音量信息。
     *        - < 0：方法调用失败。
     * @note 
     * + 对于已静音设备，音量设置会在调用 setAudioCaptureDeviceMute{@link #IAudioDeviceManager#setAudioCaptureDeviceMute} 取消静音后生效。
     * + 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int setAudioCaptureDeviceVolume(unsigned int volume) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音频采集设备音量
     * @param volume 音频采集设备音量，范围应在 [0,255] 内。 <br>
     *       - [0,25] 接近无声；
     *       - [25,75] 为低音量；
     *       - [76,204] 为中音量；
     *       - [205,255] 为高音量。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int getAudioCaptureDeviceVolume(unsigned int* volume) = 0;
    /**
     * 
     * @type api
     * @brief 设置当前音频播放设备静音状态，默认为非静音。
     * @param mute <br>
     *       - true：静音
     *       - false：非静音
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @list 音频管理
     */
    
    virtual int setAudioPlaybackDeviceMute(bool mute) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音频播放设备是否静音的信息。
     * @param mute <br>
     *       - true：静音
     *       - false：非静音
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @list 音频管理
     */
    
    virtual int getAudioPlaybackDeviceMute(bool* mute) = 0;
    /**
     * 
     * @type api
     * @brief 设置当前音频采集设备静音状态，默认为非静音。
     * @param mute <br>
     *       - true：静音
     *       - false：非静音
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note
     *      - 该方法用于静音整个系统的音频采集。你也可以仅对麦克风采集到的音频信号做静音处理，而不影响媒体播放器的音乐声音，具体参看 muteAudioCapture{@link #IRTCEngine#muteAudioCapture} 方法说明。
     *      - 设该方法为 `true` 静音后仍可通过 setAudioCaptureDeviceVolume{@link #IAudioDeviceManager#setAudioCaptureDeviceVolume} 调整采集音量，调整后的音量会在取消静音后生效。
     *      - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int setAudioCaptureDeviceMute(bool mute) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音频采集设备是否静音的信息。
     * @param mute <br>
     *       - true：静音
     *       - false：非静音
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int getAudioCaptureDeviceMute(bool* mute) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音频播放设备 ID。
     * @param device_id 音频播放设备 ID
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @list 音频管理
     */
    
    virtual int getAudioPlaybackDevice(char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前音频采集设备 ID。
     * @param device_id 音频采集设备 ID，使用方负责按 MAX_DEVICE_ID_LENGTH 大小，分配与释放内存
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int getAudioCaptureDevice(char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 尝试初始化音频播放设备，可检测出设备不存在、权限被拒绝/禁用等异常问题。
     * @param device_id 设备索引号
     * @return 设备状态错误码 <br>
     *        - 0: 设备检测结果正常
     *        - -1: 接口状态不正确，例如在正常启动采集后再调用该接口进行检测
     *        - -2: 采集设备无麦克风权限，尝试初始化设备失败
     *        - -3: 设备不存在，当前没有设备或设备被移除时返回
     *        - -4: 设备音频格式不支持
     *        - -5: 其它原因错误
     * @note 1. 该接口需在进房前调用； <br>
     *        2. 检测成功不代表设备一定可以启动成功，还可能因设备被其他应用进程独占，或 CPU/内存不足等原因导致启动失败。
     * @list 音频管理
     */
    
    virtual int initAudioPlaybackDeviceForTest(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 尝试初始化音频采集设备，可检测设备不存在、权限被拒绝/禁用等异常问题。
     * @param device_id 设备索引
     * @return 设备状态错误码 <br>
     *        - 0: 设备检测结果正常
     *        - -1: 接口状态不正确，例如在正常启动采集后再调用该接口进行检测
     *        - -2: 采集设备无麦克风权限，尝试初始化设备失败
     *        - -3: 设备不存在，当前没有设备或设备被移除时返回
     *        - -4: 设备音频格式不支持
     *        - -5: 其它原因错误
     * @note 
     *        - 该接口需在进房前调用; <br>
     *        - 检测成功不代表设备一定可以启动成功，还可能因设备被其他应用进程独占，或 CPU/内存不足等原因导致启动失败。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int initAudioCaptureDeviceForTest(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 启动音频播放设备检测。测试启动后，循环播放指定的音频文件，同时将通过 `onAudioPlaybackDeviceTestVolume` 回调播放时的音量信息。
     * @param test_audio_file_path 指定播放设备检测的音频文件网络地址。支持的格式包括 mp3，aac，m4a，3gp 和 wav。
     * @param indication_interval 设置 `onAudioPlaybackDeviceTestVolume` 音量回调的时间间隔，推荐设置为 200 毫秒或以上。单位为毫秒。最小值为 10 毫秒。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note
     *       - 该方法可在进房前和进房后调用，不可与其它音频设备测试功能同时应用。
     *       - 调用 stopAudioPlaybackDeviceTest{@link #IAudioDeviceManager#stopAudioPlaybackDeviceTest} 可以停止测试。
     * @list 音频管理
     */
    
     virtual int startAudioPlaybackDeviceTest(const char* test_audio_file_path, int indication_interval) = 0;
    /**
     * 
     * @type api
     * @brief 停止音频播放测试。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 调用 startAudioPlaybackDeviceTest{@link #IAudioDeviceManager#startAudioPlaybackDeviceTest} 后，调用本方法停止测试。
     * @list 音频管理
     */
    
    virtual int stopAudioPlaybackDeviceTest() = 0;
    /**
     * 
     * @hidden(iOS, Linux)
     * @type api
     * @brief 开始音频采集设备和音频播放设备测试。
     * @param indication_interval 测试中会收到 `onLocalAudioPropertiesReport` 回调，本参数指定了该周期回调的时间间隔，单位为毫秒。建议设置到大于 200 毫秒。最小不得少于 10 毫秒。
     * @return 方法调用结果 <br>
     *       - 0：方法调用成功
     *       - < 0：方法调用失败
     * @note
     *       - 该方法在进房前后均可调用。且不可与其它音频设备测试功能同时应用。
     *       - 调用本接口 30 s 后，采集自动停止，并开始播放采集到的声音。录音播放完毕后，设备测试流程自动结束。你也可以在 30 s 内调用 stopAudioDeviceRecordAndPlayTest{@link #IAudioDeviceManager#stopAudioDeviceRecordAndPlayTest} 来停止采集并开始播放此前采集到的声音。
     *       - 调用 stopAudioDevicePlayTest{@link #IAudioDeviceManager#stopAudioDevicePlayTest} 可以停止音频设备采集和播放测试。
     *       - 你不应在测试过程中，调用 `enableAudioPropertiesReport` 注册音量提示回调。
     *       - 该方法仅在本地进行音频设备测试，不涉及网络连接。
     *       - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int startAudioDeviceRecordTest(int indication_interval) = 0;
    /**
     * 
     * @hidden(iOS, Linux)
     * @type api
     * @brief 停止采集本地音频，并开始播放采集到的声音。录音播放完毕后，设备测试流程结束。 <br>
     *        调用 startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest} 30 s 内调用本接口来停止采集并开始播放此前采集到的声音。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note 
     *        - 调用本接口开始播放录音后，可以在播放过程中调用 stopAudioDevicePlayTest{@link #IAudioDeviceManager#stopAudioDevicePlayTest} 停止播放。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 音频管理
     */
    
    virtual int stopAudioDeviceRecordAndPlayTest() = 0;
    /**
     * 
     * @hidden(iOS, Linux)
     * @type api
     * @brief 停止由调用 startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest} 开始的音频播放设备测试。 <br>
     *        在音频播放设备测试自动结束前，可调用本接口停止音频采集与播放测试。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @list 音频管理
     */
    
    virtual int stopAudioDevicePlayTest() = 0;

    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IAudioDeviceManager() {
    }
};

}  