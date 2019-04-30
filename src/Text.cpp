#include "Text.hpp"

#include <locale>
#include <codecvt>

Text::Text(unsigned int max_length,
           BufferUsage usage) : CompositeSprite(max_length, usage), max_length(max_length)
{
}

void Text::SetFont(Font *font)
{
    // set the composite sprites atlas
    this->SetAtlas(font->GetAtlas());

    // store the font for drawing
    this->font = font;

    // redraw the string if there are any characters to draw
    if (string.size() > 0)
        this->SetString(string);
}

void Text::SetString(std::string string)
{
    // dont bother if theres no font set as nothing can be done
    if (font == nullptr)
        return;

    // the array of sprites for the characters of the given string
    std::vector<CompositeSprite::Sprite> sprites(max_length);

    // convert the given string to utf32 for extended characters (mainly japanese)
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string string_u32 = converter.from_bytes(string.data());

    // add sprites for each character in the given string
    int i = 0; //current sprite index
    Vector2 p; //current glyph position
    for (auto c: string_u32)
    {
        // get the current characters glyph
        Font::Glyph *glyph = font->GetGlyph(c);
        if (glyph == nullptr)
            continue;

        // get the current glyphs image
        AtlasImage *image = font->GetAtlas()->GetImage(glyph->Image);
        if (image == nullptr)
            continue;

        // add the sprite for this glyph
        sprites[i] = CompositeSprite::Sprite(p.X + glyph->Bearing.X,
                                             p.Y - (image->Height - glyph->Bearing.Y),
                                             glyph->Image);

        // increment the position and index for the next glyph
        p.X += glyph->Advance.X;
        p.Y += glyph->Advance.Y;
        i++;

        // break if the string is exceeding the maximum length as no more glyphs can be added
        if (i >= max_length)
            break;
    }

    // store the string values
    this->string = string;
    this->string_draw_length = i;

    // write the sprites to the composite sprite
    this->SetSprites(&sprites);
}

void Text::DrawVertices()
{
    // draw the character sprites for the last set string, maxed out at max_length
    DrawSprites((string_draw_length > max_length) ? max_length : string_draw_length);
}
