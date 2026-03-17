
#ifndef _BMF_MODS_VIDEOFRAME_H_
#define _BMF_MODS_VIDEOFRAME_H_

#include "bmf_common.h"
#include "bmf_buffer.h"
#include <vector>
#include <memory>
#include <string>
namespace bmf_mods_lite
{

    enum {
        BMF_MODS_LITE_PIXEL_FORMAT_RGBA = 0,  
        BMF_MODS_LITE_PIXEL_FORMAT_YUV420P = 100, 
        BMF_MODS_LITE_PIXEL_FORMAT_TEXTURE2D = 200,
        BMF_MODS_LITE_PIXEL_FORMAT_OES_TEXTURE = 300,
        BMF_MODS_LITE_PIXEL_FORMAT_CVPIXEL_BUFFER = 400
    };
    
    class BMF_MODS_EXPORT VideoFrame
    {
        class VideoFrameImpl;

    public:
        VideoFrame();
        virtual ~VideoFrame();
        // VideoFrame(const VideoFrame &) = default;
        // VideoFrame &operator=(const VideoFrame &) = default;
        // VideoFrame(VideoFrame &&) = default;
        VideoFrame(std::vector<void *> data, int width, int height, std::vector<int> line_size, int pixel_format, int color_model, std::shared_ptr<Buffer> buffer);
        // image properties
        int width() ;
        int height() ;
        int format() ;
        int color_model() ;
        std::vector<void*> data();
        std::vector<int> line_size();
        std::shared_ptr<Buffer> buffer();

    protected:
        std::shared_ptr<VideoFrameImpl> video_impl_;
    };

} // namespace bmf_sdk
#endif