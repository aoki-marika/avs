#pragma once

struct Vector2
{
    float X, Y;

    Vector2(float x, float y) : X(x), Y(y)
    {
    }

    Vector2() : Vector2(0, 0)
    {
    }
};
