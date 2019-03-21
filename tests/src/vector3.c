#include <stdio.h>
#include <stdlib.h>
#include <avs/Vector3.hpp>

#include "utils.h"

int main()
{
    Vector3 vector = Vector3(10, -20, -30);

    // length
    assert_float_equals("length", vector.Length(), 37.416573f);

    // normalize
    assert_vector3_equals("normalize", vector.Normalize(), Vector3(0.267261f, -0.534522f, -0.801784f));

    // cross
    assert_vector3_equals("cross", vector.Cross(Vector3(-30, 10, -20)), Vector3(700, 1100, -500));

    // dot
    assert_float_equals("dot", vector.Dot(Vector3(-30, -20, 10)), -200);

    // multiply
    assert_vector3_equals("multiply", vector * 20, Vector3(200, -400, -600));

    // subtract
    assert_vector3_equals("subtract", vector - Vector3(20, 10, 50), Vector3(-10, -30, -80));

    return EXIT_SUCCESS;
}
