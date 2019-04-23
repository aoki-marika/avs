#pragma once

#include <map>

#include "Font.hpp"
#include "Atlas.hpp"

class ImageFont : public Font
{
    private:
        // the atlas for this image font to get character images from
        Atlas *atlas;

        // the character spacing of this image font
        float spacing;

        // the map of characters to images in this image fonts atlas
        std::map<char, std::string> images;

    public:
        // create a new image font with the given atlas and character spacing
        // atlas is the atlas for this font to get character images from
        // spacing is the spacing between characters (all image fonts are monospaced)
        ImageFont(Atlas *atlas, float spacing);

        // map the given character to the given image name in this fonts atlas
        void SetCharacter(char character, std::string image);

        std::string *GetImage(char character);

        Atlas *GetAtlas()
        {
            return atlas;
        }

        float GetSpacing(char character)
        {
            return spacing;
        }
};
