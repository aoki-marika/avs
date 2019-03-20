#pragma once

#include "Matrix4.hpp"

// used as a base class, use Camera2D or Camera3D for functional cameras
class Camera
{
    protected:
        // this cameras current projection * view matrix
        Matrix4 Matrix;

    public:
        Matrix4 GetMatrix()
        {
            return Matrix;
        }
};
