#pragma once

struct Colour4
{
    // the red, green, blue, and alpha channels of this colour on a scale of 0 to 1
    float R, G, B, A;

    Colour4(float r, float g, float b, float a) : R(r), G(g), B(b), A(a)
    {
    }

    Colour4() : Colour4(0, 0, 0, 0)
    {
    }

    // global colours
    static const Colour4 White()
    {
        return Colour4(1, 1, 1, 1);
    }
};
