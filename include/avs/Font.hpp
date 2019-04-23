#pragma once

#include "Atlas.hpp"

class Font
{
    public:
        // get the atlas containing this fonts characters
        virtual Atlas *GetAtlas() = 0;

        // get the image name for the given character in this fonts atlas
        // returns nullptr if the given character is not in this font
        virtual std::string *GetImage(char character) = 0;

        // get the spacing for after the given character in this font
        virtual float GetSpacing(char character) = 0;
};
