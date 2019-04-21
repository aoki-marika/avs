#pragma once

#include "Drawable.hpp"
#include "UVBuffer.hpp"
#include "Atlas.hpp"

class Sprite : public Drawable
{
    private:
        unsigned int attrib_vertex_uv;
        unsigned int uniform_sampler;

        // the buffer for the uvs of this sprite
        UVBuffer *uv_buffer;

        // the atlas this sprite is getting images from
        Atlas *atlas;

    protected:
        virtual void BeginDraw();
        virtual void EndDraw();

    public:
        Sprite();
        ~Sprite();

        // set the image of this sprite to the image of the given name in the given atlas
        void SetImage(Atlas *atlas, std::string name);
};
