#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avs/Vector3.hpp>
#include <avs/Matrix4.hpp>

void fprintf_vector3(FILE *stream, Vector3 vector)
{
    fprintf(stream, "%.6f,%.6f,%.6f\n", vector.X, vector.Y, vector.Z);
}

void fprintf_matrix4(FILE *stream, Matrix4 matrix)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
            fprintf(stream, "%.6f ", matrix.Values[x][y]);
        fprintf(stream, "\n");
    }
}

bool float_equals(float lhs, float rhs)
{
    return fabs(lhs - rhs) <= 0.0001;
}

void assert_float_equals(const char *name, float value, float expected)
{
    if (!float_equals(value, expected))
    {
        fprintf(stderr, "%s: floats were not equal\n");
        fprintf(stderr, "expected:%.6f\n", expected);
        fprintf(stderr, "got:%.6f\n", value);

        exit(EXIT_FAILURE);
    }
}

void assert_vector3_equals(const char *name, Vector3 value, Vector3 expected)
{
    if (!float_equals(value.X, expected.X) ||
        !float_equals(value.Y, expected.Y) ||
        !float_equals(value.Z, expected.Z))
    {
        fprintf(stderr, "%s: vector3s were not equal\n");
        fprintf(stderr, "expected:\n");
        fprintf_vector3(stderr, expected);
        fprintf(stderr, "got:\n");
        fprintf_vector3(stderr, value);

        exit(EXIT_FAILURE);
    }
}

void assert_matrix4_equals(const char *name, Matrix4 value, Matrix4 expected)
{
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (!float_equals(value.Values[x][y], expected.Values[x][y]))
            {
                fprintf(stderr, "%s: matrix4s were not equal\n", name);
                fprintf(stderr, "expected:\n");
                fprintf_matrix4(stderr, expected);

                fprintf(stderr, "got:\n");
                fprintf_matrix4(stderr, value);

                exit(EXIT_FAILURE);
            }
        }
    }
}
