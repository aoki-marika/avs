#pragma once

#include "UV.hpp"

struct AtlasImage
{
    // the size of this image
    unsigned int Width, Height;

    // the uvs of this image
    // named after the vertices these apply to in VertexBuffer::Quad
    UV A, B, C, D;
};
