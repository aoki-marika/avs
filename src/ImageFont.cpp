#include "ImageFont.hpp"

ImageFont::ImageFont(Atlas *atlas, float spacing) : atlas(atlas), spacing(spacing)
{
}

void ImageFont::SetCharacter(char character, std::string image)
{
    // ensure the given name is in this image fonts atlas
    if (atlas->GetImage(image) == nullptr)
        return;

    // write the given values to this image fonts character map
    images[character] = image;
}

std::string *ImageFont::GetImage(char character)
{
    // return nullptr if the given character is not mapped
    if (images.find(character) == images.end())
        return nullptr;

    // return the image name for the given character
    return &images[character];
}
