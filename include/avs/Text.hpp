#pragma once

#include "CompositeSprite.hpp"
#include "Font.hpp"

// a drawable that draws text with a given font
class Text : public CompositeSprite
{
    private:
        // the maximum length of a drawn string in this text
        unsigned int max_length;

        // the font for this text to draw with
        Font *font = nullptr;

        // the last set string for this text to draw
        // kept to potentially re-set when the font is changed
        std::string string;

        // the length of the characters to draw for the last set string
        unsigned int string_draw_length;

    protected:
        virtual void DrawVertices();

    public:
        // create a new text with the given max length and usage
        // max_length is the maximum length of a drawn string in this text
        // usage is the usage of this texts buffers, static for setting string once, and dynamic for setting it many times
        Text(unsigned int max_length, BufferUsage usage);

        // set the font for this text to draw with
        // automatically redraws the string this text is displaying with the given font
        void SetFont(Font *font);

        // set the string for this text to display
        void SetString(std::string string);
};
