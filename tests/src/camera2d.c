#include <stdlib.h>
#include <avs/Camera2D.hpp>

#include "utils.h"

int main()
{
    Camera2D *camera;
    Matrix4 pv, correct;

    // pv matrix 1280x720
    camera = new Camera2D(1280, 720);
    pv = camera->GetMatrix();
    correct = Matrix4( 0.001563f,  0.000000f,  0.000000f, -1.000000f,
                       0.000000f, -0.002778f,  0.000000f,  1.000000f,
                       0.000000f,  0.000000f, -2.000000f,  1.000000f,
                       0.000000f,  0.000000f,  0.000000f,  1.000000f);
    assert_matrix_equals("pv matrix 1280x720", pv, correct);
    delete camera;

    // pv matrix 720x1280
    camera = new Camera2D(720, 1280);
    pv = camera->GetMatrix();
    correct = Matrix4( 0.002778f,  0.000000f,  0.000000f, -1.000000f,
                       0.000000f, -0.001563f,  0.000000f,  1.000000f,
                       0.000000f,  0.000000f, -2.000000f,  1.000000f,
                       0.000000f,  0.000000f,  0.000000f,  1.000000);
    assert_matrix_equals("pv matrix 720x1280", pv, correct);
    delete camera;

    return EXIT_SUCCESS;
}
