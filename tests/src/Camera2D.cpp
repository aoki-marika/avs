#include <stdlib.h>
#include <avs/Camera2D.hpp>

#include "Assert.hpp"

int main()
{
    // todo: update to reflect new bottom-left origin
    // add note about how its to keep vertices consistent between 2d and 3d
    // also probably update Camera2D ctor to take near/far values
    Camera2D *camera;
    Matrix4 expected;

    // pv matrix 1280x720
    camera = new Camera2D(1280, 720, -100, 100);
    expected = Matrix4( 0.001563f,  0.000000f,  0.000000f, -1.000000f,
                        0.000000f,  0.002778f,  0.000000f, -1.000000f,
                        0.000000f,  0.000000f,  0.010000f,  0.000000f,
                        0.000000f,  0.000000f,  0.000000f,  1.000000f);
    Assert::Matrix4Equals("PV matrix 1280x720", camera->GetMatrix(), expected);
    delete camera;

    // pv matrix 720x1280
    camera = new Camera2D(720, 1280, 50, -50);
    expected = Matrix4( 0.002778f,  0.000000f,  0.000000f, -1.000000f,
                        0.000000f,  0.001563f,  0.000000f, -1.000000f,
                        0.000000f,  0.000000f, -0.020000f,  0.000000f,
                        0.000000f,  0.000000f,  0.000000f,  1.000000);
    Assert::Matrix4Equals("PV matrix 720x1280", camera->GetMatrix(), expected);
    delete camera;

    return EXIT_SUCCESS;
}
