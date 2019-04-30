#pragma once

#include <map>

#include "Font.hpp"

struct FT_FaceRec_;
typedef FT_FaceRec_ *FT_Face;
class FreeTypeFont : public Font
{
    private:
        // the size of each uploaded pixel for glyph images, in bytes (luminance and alpha)
        const unsigned int pixel_size = 2;

        // the atlas for characters in this font
        Atlas *atlas;

        // the map of characters to glyph data in this font
        std::map<char32_t, Font::Glyph> glyphs;

    public:
        // create a new freetype font from the given path with the face of the given index with the given character height (in pixels)
        // as this font uses freetype to load glyphs, it supports all of the below formats
        // ttf, ttc, cff, woff, otf, otc, pfa, pfb, cid, sfnt, pcf, fnt, bdf, pfr
        FreeTypeFont(std::string path,
                     long face_index,
                     unsigned int character_height);
        ~FreeTypeFont();

        Font::Glyph *GetGlyph(char32_t character);

        Atlas *GetAtlas()
        {
            return atlas;
        }
};
