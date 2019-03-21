#include <stdlib.h>
#include <avs/Camera2D.hpp>

#include "utils.h"

int main()
{
    Camera2D *camera;
    Matrix4 correct;

    // pv matrix 1280x720
    camera = new Camera2D(1280, 720);
    correct = Matrix4( 0.001563f,  0.000000f,  0.000000f, -1.000000f,
                       0.000000f, -0.002778f,  0.000000f,  1.000000f,
                       0.000000f,  0.000000f, -2.000000f,  1.000000f,
                       0.000000f,  0.000000f,  0.000000f,  1.000000f);
    assert_matrix4_equals("pv matrix 1280x720", camera->GetMatrix(), correct);
    delete camera;

    // pv matrix 720x1280
    camera = new Camera2D(720, 1280);
    correct = Matrix4( 0.002778f,  0.000000f,  0.000000f, -1.000000f,
                       0.000000f, -0.001563f,  0.000000f,  1.000000f,
                       0.000000f,  0.000000f, -2.000000f,  1.000000f,
                       0.000000f,  0.000000f,  0.000000f,  1.000000);
    assert_matrix4_equals("pv matrix 720x1280", camera->GetMatrix(), correct);
    delete camera;

    return EXIT_SUCCESS;
}
