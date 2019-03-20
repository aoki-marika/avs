#pragma once

#include "Vector3.hpp"

struct Matrix4
{
    float Values[4][4];

    Matrix4(float v00, float v10, float v20, float v30,
            float v01, float v11, float v21, float v31,
            float v02, float v12, float v22, float v32,
            float v03, float v13, float v23, float v33)
    {
        Values[0][0] = v00; Values[1][0] = v10; Values[2][0] = v20; Values[3][0] = v30;
        Values[0][1] = v01; Values[1][1] = v11; Values[2][1] = v21; Values[3][1] = v31;
        Values[0][2] = v02; Values[1][2] = v12; Values[2][2] = v22; Values[3][2] = v32;
        Values[0][3] = v03; Values[1][3] = v13; Values[2][3] = v23; Values[3][3] = v33;
    }

    Matrix4() : Matrix4(0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0)
    {
    }

    static Matrix4 Identity()
    {
        return Matrix4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);
    }

    static Matrix4 Translation(Vector3 offset)
    {
        float x = offset.X, y = offset.Y, z = offset.Z;
        return Matrix4(1, 0, 0, x,
                       0, 1, 0, y,
                       0, 0, 1, z,
                       0, 0, 0, 1);
    }

    static Matrix4 Scaling(Vector3 scale)
    {
        float x = scale.X, y = scale.Y, z = scale.Z;
        return Matrix4(x, 0, 0, 0,
                       0, y, 0, 0,
                       0, 0, z, 0,
                       0, 0, 0, 1);
    }

    Matrix4 operator*(Matrix4 other)
    {
        Matrix4 result;

        // multiply this matrix with the given matrix
        for(int x = 0; x < 4; x++)
        {
            for(int y = 0; y < 4; y++)
            {
                float sum = 0;

                for(int i = 0; i < 4; i++)
                    sum += this->Values[i][y] * other.Values[x][i];

                result.Values[x][y] = sum;
            }
        }

        return result;
    }
};
