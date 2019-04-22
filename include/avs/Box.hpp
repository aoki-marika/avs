#pragma once

#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "Colour4.hpp"

// a coloured quad
class Box : public Drawable
{
    private:
        int attrib_vertex_position;
        int uniform_colour;

        // the buffer for the vertices of this box
        VertexBuffer *vertex_buffer;

    protected:
        virtual void DrawVertices();

    public:
        Box();
        ~Box();

        // set the draw colour of this box to the given colour
        // defaults to white
        void SetColour(Colour4 colour);
};
