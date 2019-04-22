#pragma once

#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "UVBuffer.hpp"
#include "Atlas.hpp"

class Sprite : public Drawable
{
    private:
        unsigned int attrib_vertex_position, attrib_vertex_uv;
        unsigned int uniform_sampler;

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
};
