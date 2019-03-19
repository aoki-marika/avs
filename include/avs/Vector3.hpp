#pragma once

#include <math.h>

struct Vector3
{
    float X, Y, Z;

    Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
    {
    }

    Vector3() : Vector3(0, 0, 0)
    {
    }

    float Length()
    {
        return sqrtf(X * X + Y * Y + Z * Z);
    }

    Vector3 Normalize()
    {
        float length = Length();
        if (length > 0)
            return Vector3(X / length, Y / length, Z / length);
        else
            return Vector3(0, 0, 0);
    }

    Vector3 Cross(Vector3 other)
    {
        return Vector3(Y * other.Z - Z * other.Y,
                       Z * other.X - X * other.Z,
                       X * other.Y - Y * other.X);
    }

    float Dot(Vector3 other)
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    Vector3 operator*(float other)
    {
        return Vector3(X * other, Y * other, Z * other);
    }

    Vector3 operator-(Vector3 other)
    {
        return Vector3(X - other.X, Y - other.Y, Z - other.Z);
    }
};
