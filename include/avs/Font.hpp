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

            // the offset from the start drawing point to draw this glyph at
            Vector2 Bearing;

            // how much to advance the cursor for drawing the next glyph after this glyph
            Vector2 Advance;
        };

        // get the atlas containing this fonts characters
        virtual Atlas *GetAtlas() = 0;

        // get the glyph in this font for the given character
        // returns nullptr if this font has no glyph for the given character
        virtual Glyph *GetGlyph(char32_t character) = 0;
};
