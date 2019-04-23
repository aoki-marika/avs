#include "Text.hpp"

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

    // add sprites for each character in the given string
    int n = 0; //current sprite index
    float x = 0; //current character x position
    for (auto c: string)
    {
        // get the name of this characters image
        std::string *name = font->GetImage(c);
        if (name == nullptr)
            continue;

        // get this characters image
        AtlasImage *image = font->GetAtlas()->GetImage(*name);
        if (image == nullptr)
            continue;

        // add the sprite for this character
        sprites[n] = CompositeSprite::Sprite(x, 0, *name);

        // increment the position and index for the next character sprite
        x += image->Width + font->GetSpacing(c);
        n++;

        // break if the string is exceeding the maximum length as no more character sprites can be added
        if (n >= max_length)
            break;
    }

    // store the string values
    this->string = string;
    this->string_draw_length = n;

    // write the sprites to the composite sprite
    this->SetSprites(&sprites);
}

void Text::DrawVertices()
{
    // draw the character sprites for the last set string, maxed out at max_length
    DrawSprites((string_draw_length > max_length) ? max_length : string_draw_length);
}
