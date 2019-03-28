#include <stdlib.h>
#include <avs/Camera2D.hpp>

#include "Assert.hpp"

int main()
{
    Camera2D *camera;
    Matrix4 expected;

    // pv matrix 1280x720
    camera = new Camera2D(1280, 720);
    expected = Matrix4( 0.001563f,  0.000000f,  0.000000f, -1.000000f,
                        0.000000f, -0.002778f,  0.000000f,  1.000000f,
                        0.000000f,  0.000000f, -2.000000f,  1.000000f,
                        0.000000f,  0.000000f,  0.000000f,  1.000000f);
    Assert::Matrix4Equals("PV matrix 1280x720", camera->GetMatrix(), expected);
    delete camera;

    // pv matrix 720x1280
    camera = new Camera2D(720, 1280);
    expected = Matrix4( 0.002778f,  0.000000f,  0.000000f, -1.000000f,
                        0.000000f, -0.001563f,  0.000000f,  1.000000f,
                        0.000000f,  0.000000f, -2.000000f,  1.000000f,
                        0.000000f,  0.000000f,  0.000000f,  1.000000);
    Assert::Matrix4Equals("PV matrix 720x1280", camera->GetMatrix(), expected);
    delete camera;

    return EXIT_SUCCESS;
}
