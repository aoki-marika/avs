#include "ImageFont.hpp"

ImageFont::ImageFont(Atlas *atlas, float spacing) : atlas(atlas), spacing(spacing)
{
}

void ImageFont::SetGlyph(char32_t character, std::string image)
{
    // get the atlas image to ensure the given image exists
    AtlasImage *atlas_image = atlas->GetImage(image);
    if (atlas_image == nullptr)
        return;

    // write the given values to this image fonts glyph map
    glyphs[character] =
    {
        .Image = image,
        .Bearing = Vector2(0, 0),
        .Advance = Vector2(atlas_image->Width + spacing, 0),
    };
}

Font::Glyph *ImageFont::GetGlyph(char32_t character)
{
    // return nullptr if the given character is not mapped
    if (glyphs.find(character) == glyphs.end())
        return nullptr;

    // return the glyph for the given character
    return &glyphs[character];
}
