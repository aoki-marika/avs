#include "Camera3D.hpp"

#include <math.h>

Camera3D::Camera3D(float fov,
                   float aspect_ratio,
                   float near,
                   float far)
{
    // create the perspective projection matrix
    float fov_radians = fov / 180 * M_PI;
    float f = 1.0f / tanf(fov_radians / 2.0f);
    float ar = aspect_ratio;
    float nd = near, fd = far;
    projection = Matrix4(f / ar,  0,  0,                     0,
                         0,       f,  0,                     0,
                         0,       0,  (fd + nd) / (nd - fd), (2 * fd * nd) / (nd - fd),
                         0,       0,  -1,                    0);

    // invert the x axis so +x = right
    projection = projection * Matrix4::Scaling(Vector3(-1, 1, 1));

    // initialize the pv matrix, in case set position/target isnt called
    updateMatrix();
}

void Camera3D::SetPosition(Vector3 position)
{
    this->position = position;
    updateMatrix();
}

void Camera3D::SetTarget(Vector3 target)
{
    this->target = target;
    updateMatrix();
}

void Camera3D::updateMatrix()
{
    // calculate the current look at view matrix
    Vector3 up = Vector3(0, 1, 0);
    Vector3 z = (target - position).Normalize() * -1;
    Vector3 x = up.Cross(z).Normalize();
    Vector3 y = z.Cross(x);
    Matrix4 view = Matrix4(x.X, x.Y, x.Z, -position.Dot(x),
                           y.X, y.Y, y.Z, -position.Dot(y),
                           z.X, z.Y, z.Z, -position.Dot(z),
                           0,   0,   0,   1);

    // set the new pv matrix
    Matrix = projection * view;
}
