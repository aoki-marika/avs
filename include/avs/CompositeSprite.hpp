#pragma once

#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "UVBuffer.hpp"
#include "Atlas.hpp"

class CompositeSprite : public Drawable
{
    private:
        unsigned int attrib_vertex_position, attrib_vertex_uv;
        unsigned int uniform_sampler;

        // the buffer for the vertices/uvs of this composite sprite
        VertexBuffer *vertex_buffer;
        UVBuffer *uv_buffer;

        // the atlas this composite sprite is getting images from
        Atlas *atlas = nullptr;

    protected:
        virtual void BeginDraw();
        virtual void EndDraw();
        virtual void DrawVertices();

    public:
        // a sprite in a composite sprite
        struct Sprite
        {
            // the x and y draw position of this sprite
            float X, Y;

            // the name of this sprites image in the composite sprites current atlas
            std::string Image;

            Sprite(float x, float y, std::string image) : X(x), Y(y), Image(image)
            {
            }

            Sprite() : Sprite(0, 0, "")
            {
            }
        };

        // create a new composite sprite with the given max amount of sprites and the given usage
        // a composite sprite can only draw as many sprites as given here
        // the given usage is for the uv buffer, use static for a one time set and dynamic for many sets
        // note: defaults size to 1,1,1 so sprite vertices arent multiplied by zero and not visible by default
        // size *can* be used here to scale the set sprites, but it shouldnt be when scale is implemented
        CompositeSprite(unsigned int max_sprites,
                        BufferUsage usage);
        ~CompositeSprite();

        // set the atlas for this composite sprite to get images from
        void SetAtlas(Atlas *atlas);

        // set the sprites of this image to the given sprites
        template<std::size_t size>
        void SetSprites(std::array<Sprite, size> *sprites)
        {
            // return early if theres no atlas set, as nothing can be done
            if (atlas == nullptr)
                return;

            // add each sprites vertices/uvs to the respective buffers
            for (int i = 0; i < size; i++)
            {
                // get the current sprite
                Sprite *sprite = &(*sprites)[i];

                // get the image for the current sprite
                AtlasImage *image = atlas->GetImage(sprite->Image);
                if (image == nullptr)
                    continue;

                // pass the vertices to the vertex buffer
                float sx = sprite->X, ex = sprite->X + image->Width;
                float sy = sprite->Y, ey = sprite->Y + image->Height;
                vertex_buffer->SetQuad(i * 6,
                                       Vector3(sx, sy, 0),
                                       Vector3(sx, ey, 0),
                                       Vector3(ex, ey, 0),
                                       Vector3(ex, sy, 0));

                // pass the uvs to the uv buffer
                atlas->SetBufferData(uv_buffer, i * 6, sprite->Image);
            }
        }
};
