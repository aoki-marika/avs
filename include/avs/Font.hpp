#pragma once

#include "Vector2.hpp"
#include "Atlas.hpp"

class Font
{
    public:
        struct Glyph
        {
            // the name of the image of this glyph in its fonts atlas
            std::string Image;

            // the bearing and advance of this glyph, in pixels
            Vector2 Bearing, Advance;
        };

        // get the atlas containing this fonts characters
        virtual Atlas *GetAtlas() = 0;

        // get the glyph in this font for the given character
        // returns nullptr if this font has no glyph for the given character
        virtual Glyph *GetGlyph(char32_t character) = 0;
};
