/*
 * Copyright (c) 2021 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Video Processor Interface
*/

#pragma once

#include "bytertc_video_frame.h"

namespace bytertc {
/**
 * 
 * @type api
 * @brief 视频处理接口类
 * @list 视频处理
 */

class IVideoProcessor
{
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IVideoProcessor() = default;
    /**
     * 
     * @type api
     * @brief 获取 RTC SDK 采集得到的视频帧，根据 registerLocalVideoProcessor{@link #IRTCEngine#registerLocalVideoProcessor} 设置的视频前处理器，进行视频前处理，最终将处理后的视频帧给到 RTC SDK 用于编码传输。
     * @param src_frame RTC SDK 采集得到的视频帧，参看 IVideoFrame{@link #IVideoFrame}。
     * @return 经过视频前处理后的视频帧，返回给 RTC SDK 供编码和传输，参看 IVideoFrame{@link #IVideoFrame}。
     * @note
     *       - 在进行视频前处理前，你需要调用 registerLocalVideoProcessor{@link #IRTCEngine#registerLocalVideoProcessor} 设置视频前处理器。
     *       - 如果需要取消视频前处理，可以将视频前处理器设置为 nullptr。
     *       - 应用层实现 `processVideoFrame` 接口时，可以直接修改 `src_frame` 的缓冲区，调用 `addRef` 方法将 `src_frame` 引用计数加一，然后将修改后的 `src_frame` 返回， 也可以创建一个新的视频帧并返回。
     *       - 对于 `processVideoFrame` 返回的视频帧，在 SDK 层消费完成后，SDK 内部总是调用这个视频帧的 releaseRef 方法。
     * @list 自定义流处理
     */
    
    virtual IVideoFrame* processVideoFrame(IVideoFrame* src_frame) = 0;
};

} 