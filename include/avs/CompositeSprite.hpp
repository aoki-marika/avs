#pragma once

#include <vector>

#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "UVBuffer.hpp"
#include "Atlas.hpp"
#include "Colour4.hpp"

// a drawable that draws multiple textures with a single draw call
// note: Draw() draws all sprites, DrawSprites() draws max_sprites sprites
class CompositeSprite : public Drawable
{
    private:
        int attrib_vertex_position, attrib_vertex_uv;
        int uniform_sampler, uniform_colour;

        // the maximum number of sprites in this composite sprite
        unsigned int max_sprites;

        // the buffer for the vertices/uvs of this composite sprite
        VertexBuffer *vertex_buffer;
        UVBuffer *uv_buffer;

        // the atlas this composite sprite is getting images from
        Atlas *atlas = nullptr;

    protected:
        virtual void DrawVertices();

        // bind and draw the given number of sprites from this composite sprite
        void DrawSprites(unsigned int num_sprites);

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
        void SetSprites(std::vector<Sprite> *sprites);

        // set the colour of this composite sprite to the given colour
        // this colour is used to multiply this composite sprites sprites image colours
        // defaults to white
        void SetColour(Colour4 colour);

        // draw the given number of sprites from this composite sprite with the given camera
        void DrawSprites(unsigned int num_sprites, Camera *camera);
};
