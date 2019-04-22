#pragma once

#include "Camera.hpp"

class Camera2D : public Camera
{
    public:
        // create a 2d camera with the given resolution in pixels
        // vertices drawn with this camera have their units in pixels and a top left origin
        // +x = right, -x = left, +y = down, -y = up
        Camera2D(float width, float height);
};
