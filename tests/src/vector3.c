#include <stdio.h>
#include <stdlib.h>
#include <avs/Vector3.hpp>

#include "Assert.hpp"

int main()
{
    Vector3 vector = Vector3(10, -20, -30);

    // length
    Assert::FloatEquals("Length", vector.Length(), 37.416573f);

    // normalize
    Assert::Vector3Equals("Normalize", vector.Normalize(), Vector3(0.267261f, -0.534522f, -0.801784f));

    // cross
    Assert::Vector3Equals("Cross", vector.Cross(Vector3(-30, 10, -20)), Vector3(700, 1100, -500));

    // dot
    Assert::FloatEquals("Dot", vector.Dot(Vector3(-30, -20, 10)), -200);

    // multiply
    Assert::Vector3Equals("Multiply", vector * 20, Vector3(200, -400, -600));

    // subtract
    Assert::Vector3Equals("Subtract", vector - Vector3(20, 10, 50), Vector3(-10, -30, -80));

    return EXIT_SUCCESS;
}
