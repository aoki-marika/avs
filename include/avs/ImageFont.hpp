#pragma once

#include <map>

#include "Font.hpp"
#include "Atlas.hpp"

class ImageFont : public Font
{
    private:
        // the atlas for this image font to get glyph images from
        Atlas *atlas;

        // the spacing between glyphs in this font
        float spacing;

        // the map of characters to glyph data in this font
        std::map<char32_t, Font::Glyph> glyphs;

    public:
        // create a new image font from the given atlas with the given inter-glyph spacing
        ImageFont(Atlas *atlas, float spacing);

        // map the given character to the given glyph image in this fonts atlas
        void SetGlyph(char32_t character, std::string image);

        Font::Glyph *GetGlyph(char32_t character);

        Atlas *GetAtlas()
        {
            return atlas;
        }
};
