#ifndef VV_TEXTURE_TEXTURE3D_H
#define VV_TEXTURE_TEXTURE3D_H


#include "texture_common.h"

#include <stddef.h>


namespace virvo
{


template
<
    typename VoxelT
>
class texture< VoxelT, 3 >
{
public:

    VV_COMMON_TEX_IFACE

    texture()
    {
    }

    texture(size_t w, size_t h, size_t d)
        : width_(w)
        , height_(h)
        , depth_(d)
    {
        data.resize(w * h * d);
    }

    value_type& operator()(size_t x, size_t y, size_t z)
    {
        return data[z * width_ * height_ + y * width_ + x];
    }

    value_type const& operator()(size_t x, size_t y, size_t z) const
    {
        return data[z * width_ * height_ + y * width_ + x];
    }

    size_t width() const { return width_; }
    size_t height() const { return height_; }
    size_t depth() const { return depth_; }

private:

    size_t width_;
    size_t height_;
    size_t depth_;
};


}


#endif // VV_TEXTURE_TEXTURE3D_H


