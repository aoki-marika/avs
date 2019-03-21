#include "Camera2D.hpp"

#include <math.h>

Camera2D::Camera2D(float width, float height)
{
    // create the orthographic projection matrix
    // width/height in size (to make units 1:1 to pixels) with 0,0 in the center
    // -x = left, x = right, -y = down, y = up
    float l = -width / 2, r = width / 2;
    float b = height / 2, t = -height / 2;
    float n = 1, f = 0;
    float tx = -(r + l) / (r - l);
    float ty = -(t + b) / (t - b);
    float tz = -(f + n) / (f - n);
    Matrix = Matrix4(2 / (r - l), 0,                  0,           tx,
                     0,           2 / (t - b),        0,           ty,
                     0,           0,                  2 / (f - n), tz,
                     0,           0,                  0,           1);

    // offset by half width/height to make 0,0 the bottom left of the window
    Matrix = Matrix * Matrix4::Translation(Vector3(-width / 2, -height / 2, 0));
}
