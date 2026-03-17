#ifndef _BMF_MODS_ALGORITHM_ENGINE_H_
#define _BMF_MODS_ALGORITHM_ENGINE_H_

#include <vector>
#include <memory>
#include "bmf_common.h"
#include "bmf_video_frame.h"
#include "bmf_param.h"

namespace bmf_mods_lite
{
    enum {
        BMFMods_ALOGRITHM_SUPER_RESOLUTION = 0,  
        BMFMods_ALOGRITHM_DENOISE = 1, 
        BMFMods_ALOGRITHM_SHARP = 2,
        BMFMods_ALOGRITHM_NOISE_LIVE = 3,
    };
    // image quality algorithm encapsulation interface
    class BMF_MODS_EXPORT IBmfModsAlgorithmInterface
    {
    public:
        virtual ~IBmfModsAlgorithmInterface() {}

        virtual int init(Param param) = 0;
        virtual int unInit() = 0;

        virtual int processVideoFrame(VideoFrame videoframe, Param param) = 0;
        virtual int getVideoFrameOutput(VideoFrame &frame, Param &param) = 0;

        virtual int processMultiVideoFrame(std::vector<VideoFrame > videoframes, Param param) = 0;
        virtual int getMultiVideoFrameOutput(std::vector<VideoFrame > videoframes, Param param) = 0;

        // virtual int processVideoFrame(void *data) = 0;
        // virtual int getVideoFrameOutput(void *data) = 0;

        virtual int getProcessProperty(Param param) = 0;
        virtual int setInputProperty(Param attr) = 0;
        virtual int getOutputProperty(Param attr) = 0;
    };

    class BMF_MODS_EXPORT BmfAlgorithmFactory
    {
    public:
        static IBmfModsAlgorithmInterface *createAlgorithmInterface(int algorithm_type);
        static void releaseAlgorithmInterface(IBmfModsAlgorithmInterface *instance);
    };

} /* namespace bmf_mods_lite */

#endif //_BMF_ALGORITHM_ENGINE_H_
