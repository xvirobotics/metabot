/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Interface Lite
 */

#pragma once

#include "bytertc_engine.h"
#include "rtc/bytertc_defines.h"
#include "rtc/bytertc_video_defines_ex.h"
#include "bytertc_engine_event_handler.h"

namespace bytertc {

/**
 * 
 * @hidden
 */
class IRTCRoomEx;
/**
 * 
 * @hidden
 */
class IVideoSource;
/**
 * 
 * @hidden
 */
class IPublisher;
/**
 * 
 * @type api
 * @brief IRTCEngineEx class
 */
class IRTCEngineEx : public IRTCEngine {
public:
    /**
     * 
     * @type api
     * @brief 创建 IRTCRoomEx{@link #IRTCRoomEx} 实例。<br>
     *        调用此方法仅返回一个房间实例，你仍需调用 joinRoom{@link #IRTCRoom#joinRoom} 才能真正地创建/加入房间。
     * @param room_id 标识通话房间的房间 ID。该字符串符合正则表达式：`[a-zA-Z0-9_@\-\.]{1,128}`。
     * @return 创建的 IRTCRoomEx{@link #IRTCRoomEx} 房间实例。
     * @note 请勿使用同样的 roomId 创建多个房间，否则后创建的房间实例会替换先创建的房间实例。
     */
    virtual IRTCRoomEx* createRTCRoomEx(const char* room_id) = 0;
    /**
     * 
     * @type api
     * @brief 设置视频源的相关配置。
     * @param stream_index 源对应的流的索引。参看 StreamIndex{@link #StreamIndex}。
     * @param config 参看 VideoSourceConfig{@link #VideoSourceConfig}。
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    virtual int setVideoSource(StreamIndex stream_index, const VideoSourceConfig& config) = 0;
    /**
     * 
     * @type api
     * @brief 推送外部视频帧。
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param frame 设置视频帧，参看 IVideoFrame{@link #IVideoFrame}。
     * @return 方法调用结果：<br>
     *        + 0：成功；<br>
     *        + <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note  
     *       + 支持格式：I420, NV12, RGBA, BGRA, ARGB  <br>
     *       + 该函数运行在用户调用线程内  <br>
     *       + 推送外部视频帧前，必须调用 setVideoSource{@link #IRTCEngineEx#setVideoSource} 开启外部视频源采集。
     */
    virtual int pushExternalVideoFrame(StreamIndex stream_index, const VideoFrameData& frame) = 0;
    /**
     * 
     * @type api
     * @brief 设置流的优先级。<br>
     *        在网络条件有限的情况下，优先保障高优先级视频流的清晰度和流畅度，优先保障高优先级流的音频流畅度和音质。
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param priority 参看 StreamPriority{@link #StreamPriority}。
     */
    virtual int setLocalStreamPriority(StreamIndex stream_index, StreamPriority priority) = 0;
    /**
     * 
     * @type api
     * @brief 立即开启内部视频采集。默认为关闭状态。  <br>
     *        内部视频采集指：使用 RTC SDK 内置视频采集模块，进行采集。<br>
     *        调用该方法后，本地用户会收到 onVideoDeviceStateChanged{@link #IRTCEngineEventHandler#onVideoDeviceStateChanged} 的回调。  <br>
     *        本地用户在非隐身状态下调用该方法后，房间中的其他用户会收到 onUserStartVideoCapture{@link #IRTCEngineEventHandler#onUserStartVideoCapture} 的回调。
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param device_id 设备 ID
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  
     *       + 调用 stopVideoCapture{@link #IRTCEngineEx#stopVideoCapture} 可以停止内部视频采集。否则，只有当销毁引擎实例时，内部视频采集才会停止。  <br>
     *       + 创建引擎后，无论是否发布视频数据，你都可以调用该方法开启内部视频采集。只有当（内部或外部）视频采集开始以后视频流才会发布。  <br>
     *       + 如果需要从自定义视频采集切换为内部视频采集，你必须先关闭自定义采集，再调用此方法手动开启内部采集。
     */
    virtual int startVideoCapture(StreamIndex stream_index, const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 立即关闭内部视频采集。默认为关闭状态。  <br>
     *        内部视频采集指：使用 RTC SDK 内置视频采集模块，进行采集。<br>
     *        调用该方法后，本地用户会收到 onVideoDeviceStateChanged{@link #IRTCEngineEventHandler#onVideoDeviceStateChanged} 的回调。  <br>
     *        非隐身用户进房后调用该方法，房间中的其他用户会收到 onUserStopVideoCapture{@link #IRTCEngineEventHandler#onUserStopVideoCapture} 的回调。
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  
     *       + 调用 startVideoCapture{@link #IRTCEngine#startVideoCapture} 可以开启内部视频采集。  <br>
     *       + 如果不调用本方法停止内部视频采集，则只有当销毁引擎实例时，内部视频采集才会停止。
     */
    virtual int stopVideoCapture(StreamIndex stream_index) = 0;
   /**
    * 
    * @type api
    * @brief 设置 RTC SDK 内部采集时的视频采集参数。<br>
    *        如果你的项目使用了 SDK 内部采集模块，可以通过本接口指定视频采集参数，包括模式、分辨率、帧率。
    * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
    * @param video_capture_config 视频采集参数。参看: VideoCaptureConfig{@link #VideoCaptureConfig}。
    * @return  
    *        + 0: 成功；  <br>
    *        + < 0: 失败；  
    * @note  
    * + 本接口在引擎创建后可调用，调用后立即生效。建议在调用 startVideoCapture{@link #IRTCEngineEx#startVideoCapture} 前调用本接口。<br>
    * + 如果调用本接口前使用内部模块开始视频采集，采集参数默认为 Auto 模式。
    */
    virtual int setVideoCaptureConfig(StreamIndex stream_index, const VideoCaptureConfig& video_capture_config) = 0;

    /**
     * 
     * @type api
     * @brief 设置音频源的整体音量。
     * @param stream_index 音频源对应的流的索引。参看 StreamIndex{@link #StreamIndex}。
     * @param volume 音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。<br>
     *               只改变音频数据的音量信息，不涉及本端硬件的音量调节。<br>
     *               为保证更好的通话质量，建议将 volume 值设为 [0,100]。
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    virtual int setAudioSourceVolume(StreamIndex stream_index, int volume) = 0;
    /**
     * 
     * @type api
     * @brief 设置音频源的内容。单个音频源的内容可以包含一个或多个原始音频来源。
     * @param stream_index 音频源对应的流的索引。参看 StreamIndex{@link #StreamIndex}。
     * @param config 内容构成。参看 AudioContentTypeConfig{@link #AudioContentTypeConfig}。
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    virtual int setAudioContentType(StreamIndex stream_index, const AudioContentTypeConfig& config) = 0;
    /**
     * 
     * @type api
     * @brief 设置音频流的编码配置。
     * @param stream_index 音频流的索引。参看 StreamIndex{@link #StreamIndex}。
     * @param config 编码设置。参看 AudioEncodeConfig{@link #AudioEncodeConfig}。
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    virtual int setAudioEncodeConfig(StreamIndex stream_index, const AudioEncodeConfig& config) = 0;


    /**
     * 
     * @type api
     * @brief 设置多路流编码配置，在开启simulcast功能下可同时设置4路分辨率的流
     * @param stream_index 视频频流的索引。参看 StreamIndex{@link #StreamIndex}。
     * @param solutions 编码设置。参看 VideoEncoderConfig{@link #VideoEncoderConfig}。
     * @param solution_num 需要传入的solutions数组大小。
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    virtual int setVideoEncoderConfig(StreamIndex stream_index, const VideoEncoderConfig* solutions,
                                      int solution_num) = 0;

    /**
    * 
    * @type api
    * @brief 创建 IRTCEngineEx 实例。  <br>
    *        如果当前进程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。  <br>
    *        如果当前进程中已创建了引擎实例，再次调用此方法时，会返回已创建的引擎实例。
    * @param config 创建引擎参数配置，详见 EngineConfig{@link #EngineConfig}
    * @param event_handler SDK 回调给应用层的 Callback 对象，详见 IRTCEngineEventHandler{@link #IRTCEngineEventHandler} 。
    * @return IRTCEngineEx：创建成功。返回一个可用的 IRTCEngineEx{@link #IRTCEngineEx} 实例
    */
    static BYTERTC_STATIC_API IRTCEngineEx* createRTCEngineEx(const EngineConfig& config, IRTCEngineEventHandler* event_handler);

    /**
* 
* @type api
* @brief 创建 IRTCEngineEx 实例。  <br>
*        如果当前进程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。  <br>
*        如果当前进程中已创建了引擎实例，再次调用此方法时，会返回新的引擎实例。
* @param config 创建引擎参数配置，详见 EngineConfig{@link #EngineConfig}
* @param event_handler SDK 回调给应用层的 Callback 对象，详见 IRTCEngineEventHandler{@link #IRTCEngineEventHandler} 。
* @return IRTCEngineEx：创建成功。返回一个可用的 IRTCEngineEx{@link #IRTCEngineEx} 实例
*/
    static BYTERTC_STATIC_API IRTCEngineEx* createRTCEngineExMulti(const EngineConfig& config, IRTCEngineEventHandler* event_handler);

    /**
     * 
     * @type api
     * @brief 销毁由 IRTCEngineEx 实例，并释放所有相关资源。
     * @note  
     *        + 请确保和需要销毁的 IRTCEngineEx 实例相关的业务场景全部结束后，才调用此方法。如果在多线程场景下，调用此接口后，又调用了其他 IRTCEngineEx 相关接口，可能导致 SDK 崩溃。该方法在调用之后，会销毁所有和此 IRTCEngineEx 实例相关的内存，并且停止与媒体服务器的任何交互。  <br>
     *        + 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
     */
    static BYTERTC_STATIC_API void destroyRTCEngineEx();

    /**
 * 
 * @hidden internal use only
 * @type apx
 * @brief 销毁由 createRTCEngineExMulti{@link #IRTCEngine#createRTCEngineExMulti} 所创建的引擎实例，并释放所有相关资源。
 * @note
 *      - 请确保和需要销毁的 IRTCEngineEx{@link #IRTCEngineEx} 实例相关的业务场景全部结束后，才调此方法
 *      - 该方法在调用之后，会销毁所有和此 IRTCEngineEx{@link #IRTCEngineEx} 实例相关的内存，并且停止与媒体服务器的任何交互
 *      - 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
 * @list 引擎管理
 */
    static BYTERTC_STATIC_API void destroyRTCEngineExMulti(IRTCEngineEx* instance_multi);
};

}  
