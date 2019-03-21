#include <stdlib.h>
#include <avs/Camera3D.hpp>

#include "utils.h"

int main()
{
    float width, height;
    Camera3D *camera;
    Matrix4 correct;

    // 1280x720 position and target
    width = 1280; height = 720;
    camera = new Camera3D(45,
                          width / height,
                          0.1f,
                          100.0f);

    // 0,0,-40 to 0,0,0
    camera->SetPosition(Vector3(0, 0, -40));
    camera->SetTarget(Vector3(0, 0, 0));
    correct = Matrix4(1.357995f, 0.000000f, 0.000000f, 0.000000f,
                      0.000000f, 2.414213f, 0.000000f, 0.000000f,
                      0.000000f, 0.000000f, 1.002002f, 39.879879f,
                      0.000000f, 0.000000f, 1.000000f, 40.000000f);
    assert_matrix4_equals("pv matrix 1280x720 from 0,0,-40 to 0,0,0", camera->GetMatrix(), correct);

    // 10,-5,30 to 2,7,3
    camera->SetPosition(Vector3(10, -5, 30));
    camera->SetTarget(Vector3(2, 7, 3));
    correct = Matrix4(-1.302043f,  0.000000f,  0.385791f,   1.446714f,
                       0.268869f,  2.220969f,  0.907432f, -18.806816f,
                      -0.261872f,  0.392808f, -0.883817f,  30.897078f,
                      -0.261349f,  0.392023f, -0.882052f,  31.035147f);
    assert_matrix4_equals("pv matrix 1280x720 from 10,-5,30 to 2,7,3", camera->GetMatrix(), correct);
    delete camera;

    // 720x1280 position and target
    width = 720, height = 1280;
    camera = new Camera3D(90,
                          width / height,
                          1.0f,
                          10.0f);

    // -20,-40,-60 to 60,40,20
    camera->SetPosition(Vector3(-20, -40, -60));
    camera->SetTarget(Vector3(60, 40, 20));
    correct = Matrix4( 1.257079f,  0.000000f, -1.257079f, -50.283150f,
                      -0.408248f,  0.816497f, -0.408248f,   0.000000f,
                       0.705650f,  0.705650f,  0.705650f,  82.455811f,
                       0.577350f,  0.577350f,  0.577350f,  69.282028f);
    assert_matrix4_equals("pv matrix 720x1280 from -20,-40,-60 to 60,40,20", camera->GetMatrix(), correct);

    // 20,40,60 to -60,-40,-20
    camera->SetPosition(Vector3(20, 40, 60));
    camera->SetTarget(Vector3(-60, -40, -20));
    correct = Matrix4(-1.257079f,  0.000000f,  1.257079f, -50.283150f,
                      -0.408248f,  0.816497f, -0.408248f,   0.000000f,
                      -0.705650f, -0.705650f, -0.705650f,  82.455811f,
                      -0.577350f, -0.577350f, -0.577350f,  69.282028f);
    assert_matrix4_equals("pv matrix 720x1280 from 20,40,60 to -60,-40,-20", camera->GetMatrix(), correct);
    delete camera;

    return EXIT_SUCCESS;
}
