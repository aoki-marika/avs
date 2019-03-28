#include <avs/Matrix4.hpp>

#include "Assert.hpp"

int main()
{
    Matrix4 matrix, translation, scaling, expected;

    // translate by 5,5,5
    translation = Matrix4::Translation(Vector3(5, 5, 5));
    matrix = Matrix4::Identity() * translation;
    expected = Matrix4(1, 0, 0, 5,
                       0, 1, 0, 5,
                       0, 0, 1, 5,
                       0, 0, 0, 1);
    Assert::Matrix4Equals("Translation 5,5,5", matrix, expected);

    // scale by 2,2,2
    scaling = Matrix4::Scaling(Vector3(2, 2, 2));
    matrix = matrix * scaling;
    expected = Matrix4(2, 0, 0, 5,
                       0, 2, 0, 5,
                       0, 0, 2, 5,
                       0, 0, 0, 1);
    Assert::Matrix4Equals("Scaling 2,2,2", matrix, expected);

    // translate by -2,1,-3
    translation = Matrix4::Scaling(Vector3(-2, 1, -3));
    matrix = matrix * translation;
    expected = Matrix4(-4,  0,  0,  5,
                        0,  2,  0,  5,
                        0,  0, -6,  5,
                        0,  0,  0,  1);
    Assert::Matrix4Equals("Translation -2,1,-3", matrix, expected);

    // scale by -10 -5 20
    scaling = Matrix4::Scaling(Vector3(-10, -5, 20));
    matrix = matrix * scaling;
    expected = Matrix4(  40,    0,    0,    5,
                          0,  -10,    0,    5,
                          0,    0, -120,    5,
                          0,    0,    0,    1);
    Assert::Matrix4Equals("Scaling -10,-5,20", matrix, expected);

    return EXIT_SUCCESS;
}
