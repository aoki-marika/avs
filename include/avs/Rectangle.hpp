#pragma once

struct Rectangle
{
    unsigned int StartX, StartY;
    unsigned int EndX, EndY;

    Rectangle(unsigned int start_x,
              unsigned int start_y,
              unsigned int end_x,
              unsigned int end_y) : StartX(start_x), StartY(start_y), EndX(end_x), EndY(end_y)
    {
    }

    unsigned int GetWidth()
    {
        return EndX - StartX;
    }

    unsigned int GetHeight()
    {
        return EndY - StartY;
    }
};
