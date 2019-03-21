#include <avs/Matrix4.hpp>

#include "utils.h"

int main()
{
    Matrix4 matrix, translation, scaling, correct;

    // translate by 5,5,5
    translation = Matrix4::Translation(Vector3(5, 5, 5));
    matrix = Matrix4::Identity() * translation;
    correct = Matrix4(1, 0, 0, 5,
                      0, 1, 0, 5,
                      0, 0, 1, 5,
                      0, 0, 0, 1);
    assert_matrix4_equals("translation 5,5,5", matrix, correct);

    // scale by 2,2,2
    scaling = Matrix4::Scaling(Vector3(2, 2, 2));
    matrix = matrix * scaling;
    correct = Matrix4(2, 0, 0, 5,
                      0, 2, 0, 5,
                      0, 0, 2, 5,
                      0, 0, 0, 1);
    assert_matrix4_equals("scaling 2,2,2", matrix, correct);

    // translate by -2,1,-3
    translation = Matrix4::Scaling(Vector3(-2, 1, -3));
    matrix = matrix * translation;
    correct = Matrix4(-4,  0,  0,  5,
                       0,  2,  0,  5,
                       0,  0, -6,  5,
                       0,  0,  0,  1);
    assert_matrix4_equals("translation -2,1,-3", matrix, correct);

    // scale by -10 -5 20
    scaling = Matrix4::Scaling(Vector3(-10, -5, 20));
    matrix = matrix * scaling;
    correct = Matrix4(  40,    0,    0,    5,
                         0,  -10,    0,    5,
                         0,    0, -120,    5,
                         0,    0,    0,    1);
    assert_matrix4_equals("scaling -10,-5,20", matrix, correct);

    return EXIT_SUCCESS;
}
