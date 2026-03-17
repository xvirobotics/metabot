#ifndef _BMF_MODS_BUFFER_H_
#define _BMF_MODS_BUFFER_H_

#include "bmf_common.h"
#include <memory>

namespace bmf_mods_lite
{
    class BMF_MODS_EXPORT Buffer
    {
    public:
        class BufferImpl;
        Buffer(uint8_t *data, int size, void (*free)(void *opaque, uint8_t *data), void *opaque);
        virtual ~Buffer();

    private:
        std::shared_ptr<BufferImpl> buffer_impl_;
    };
};
#endif