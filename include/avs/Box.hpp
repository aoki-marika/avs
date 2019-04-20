#pragma once

#include "Drawable.hpp"
#include "Colour4.hpp"

class Box : public Drawable
{
    private:
        unsigned int uniform_colour;

    public:
        Box();

        // set the draw colour of this box to the given colour
        // defaults to white
        void SetColour(Colour4 colour);
};
