#pragma once

#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "UVBuffer.hpp"
#include "Atlas.hpp"
#include "Colour4.hpp"

class Sprite : public Drawable
{
    private:
        int attrib_vertex_position, attrib_vertex_uv;
        int uniform_sampler, uniform_colour;

        // the buffer for the vertices/uvs of this sprite
        VertexBuffer *vertex_buffer;
        UVBuffer *uv_buffer;

        // the atlas this sprite is getting images from
        Atlas *atlas = nullptr;

    protected:
        virtual void DrawVertices();

    public:
        Sprite();
        ~Sprite();

        // set the image of this sprite to the image of the given name in the given atlas
        void SetImage(Atlas *atlas, std::string name);

        // set the colour of this sprite to the given colour
        // this colour is used to multiply this sprites images colours
        // defaults to white
        void SetColour(Colour4 colour);
};
