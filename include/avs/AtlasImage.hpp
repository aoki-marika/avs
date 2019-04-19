#pragma once

#include "UV.hpp"

struct AtlasImage
{
    // the size of this image
    unsigned int Width, Height;

    // the uv coordinates of this image
    // named after the vertices these apply to in VertexBuffer::Quad
    UV A, B, C, D;
};
