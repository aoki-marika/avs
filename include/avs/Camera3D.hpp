#pragma once

#include "Camera.hpp"
#include "Vector3.hpp"

class Camera3D : public Camera
{
    private:
        Vector3 position, target;
        Matrix4 projection;

        void updateMatrix();

    public:
        // create a 3d perspective camera with the given vertical fov, aspect ratio, and near/far clipping distances
        // vertices drawn with this camera have a center origin, relative to the world
        // +x = right, -x = left, +y = up, -y = down
        Camera3D(float fov,
                 float aspect_ratio,
                 float near,
                 float far);

        // set this cameras position in 3d space
        void SetPosition(Vector3 position);

        // set the point for this camera to look at in 3d space
        void SetTarget(Vector3 target);
};
