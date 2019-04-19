#pragma once

struct UV
{
    float U, V;

    UV(float u, float v) : U(u), V(v)
    {
    }

    UV() : UV(0, 0)
    {
    }
};
