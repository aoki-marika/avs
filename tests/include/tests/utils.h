#pragma once

#include <stdio.h>
#include <stdlib.h>

void fprintf_matrix(FILE *stream, Matrix4 matrix)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
            fprintf(stream, "%.6f ", matrix.Values[x][y]);
        fprintf(stream, "\n");
    }
}

void assert_matrix_equals(const char *name, Matrix4 matrix, Matrix4 expected)
{
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            // compare with an epsilon
            if (fabs(matrix.Values[x][y] - expected.Values[x][y]) > 0.0001)
            {
                fprintf(stderr, "matrix: %s: matrices were not equal\n", name);
                fprintf(stderr, "expected:\n");
                fprintf_matrix(stderr, expected);

                fprintf(stderr, "got:\n");
                fprintf_matrix(stderr, matrix);

                exit(EXIT_FAILURE);
            }
        }
    }
}
