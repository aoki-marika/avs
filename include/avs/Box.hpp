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

        // the current state of this box
        Colour4 colour;

    protected:
        virtual void DrawVertices();

    public:
        Box();
        ~Box();

        // set the draw colour of this box to the given colour
        // defaults to white
        void SetColour(Colour4 colour);

        // transformable helper methods
        void ColourTo(Colour4 colour, double duration, Easing easing = Easing::None);
        void ColourRTo(float r, double duration, Easing easing = Easing::None);
        void ColourGTo(float g, double duration, Easing easing = Easing::None);
        void ColourBTo(float b, double duration, Easing easing = Easing::None);
        void ColourATo(float a, double duration, Easing easing = Easing::None);
};
