#pragma once

#include "Camera.hpp"

class Camera2D : public Camera
{
    public:
        // create a 2d camera with the given resolution in pixels, with the given z clipping range
        // vertices drawn with this camera have their units in pixels and a bottom left origin
        // +x = right, -x = left, +y = up, -y = down
        Camera2D(float width, float height, float near, float far);
};
