#include "FreeTypeFont.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GLES2/gl2.h>

#include "FreeType.hpp"

FreeTypeFont::FreeTypeFont(std::string path,
                           long face_index,
                           unsigned int character_height)
{
    // the last freetype error
    FT_Error error;

    // load the given face from the given font
    FT_Face face;
    error = FT_New_Face(FreeType::Library, path.c_str(), face_index, &face);
    if (error != FT_Err_Ok)
    {
        const char *format = "Unable to get face %ld from font at \"%s\" (0x%02x)";
        char message[strlen(format) + path.size()];
        sprintf(message, format, face_index, path.c_str(), error);
        throw new std::runtime_error(message);
    }

    // select the unicode charmap
    error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if (error != FT_Err_Ok)
    {
        const char *format = "Error selecting Unicode charmap from font at \"%s\" (0x%02x)";
        char message[strlen(format) + path.size()];
        sprintf(message, format, path.c_str(), error);
        throw new std::runtime_error(message);
    }

    // set the faces height to the given height
    error = FT_Set_Pixel_Sizes(face, 0, character_height);
    if (error != FT_Err_Ok)
    {
        const char *format = "Error setting character height of %ld from font at \"%s\" (0x%02x)";
        char message[strlen(format) + path.size()];
        sprintf(message, format, character_height, path.c_str(), error);
        throw new std::runtime_error(message);
    }

    // get the width of the glyph atlas
    // try to find the smallest square that can hold every glyph if each glyph was square
    int atlas_width = 0;
    while (atlas_width * atlas_width < face->num_glyphs * character_height * character_height)
        atlas_width += character_height;

    // ensure the width is a power of two
    atlas_width = ((atlas_width + 1) / 2) * 2;

    // get the atlas height
    int atlas_height = atlas_width;

    // ensure the atlas width and/or height dont exceed the maximum texture size
    // if push come to shove and the volume needed for all the glyphs is greater than the
    // max texture size, then just cut off the remaining glyphs
    GLint max_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
    if (atlas_width > max_size)
    {
        atlas_height += atlas_width - max_size;
        atlas_width = max_size;
    }

    if (atlas_height > max_size)
        atlas_height = max_size;

    // generate the glyph atlas
    atlas = new Atlas(atlas_width,
                      atlas_height,
                      GL_LUMINANCE_ALPHA,
                      GL_UNSIGNED_BYTE,
                      GL_LINEAR,
                      GL_LINEAR,
                      GL_CLAMP_TO_EDGE,
                      GL_CLAMP_TO_EDGE);

    // add all the glyphs to the atlas
    FT_ULong code; //current character code
    FT_UInt i;     //current glyph index
    char32_t c;    //current character
    Vector2 p;     //current glyph position

    code = FT_Get_First_Char(face, &i);
    while (i != 0)
    {
        // get the current character
        c = (char32_t)code;

        // load and render the current character
        error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error != FT_Err_Ok)
        {
            fprintf(stderr, "FreeTypeFont: Unable to load/render character 0x%04x (0x%02x)\n", code, error);

            // skip to the next character
            code = FT_Get_Next_Char(face, code, &i);
            continue;
        }

        // create a new buffer for the current glyph that holds all the needed values
        // if uploading just GL_ALPHA, then a more specialized shader would be needed for coloured text
        // but with this using GL_LUMINANCE_ALPHA it can just multiply by a given colour and share it with all other fonts
        unsigned int buffer_size = face->glyph->bitmap.width * face->glyph->bitmap.rows * pixel_size;
        unsigned char buffer[buffer_size];
        for (int i = 0; i < buffer_size; i += pixel_size)
        {
            buffer[i] = 0xff;
            buffer[i + 1] = face->glyph->bitmap.buffer[i / pixel_size];
        }

        // add the glyph to the atlas
        std::string image = std::to_string(c);
        atlas->AddImage(image,
                        p.X,
                        p.Y,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        buffer);

        // add the glyph to the map
        glyphs[c] =
        {
            .Image = image,
            .Bearing = Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            .Advance = Vector2(face->glyph->advance.x >> 6, face->glyph->advance.y >> 6),
        };

        // increment the next glyphs position
        // wrap to the next line if the current could clip the next glyph
        p.X += face->glyph->bitmap.width;
        if (p.X + character_height >= atlas_width)
        {
            p.X = 0;
            p.Y += character_height;
        }

        // break if the next row of glyphs exceed the atlas height
        if (p.Y + character_height >= atlas_height)
            break;

        // iterate to the next character
        code = FT_Get_Next_Char(face, code, &i);
    }

    // close the face as the glyphs are now cached
    FT_Done_Face(face);
}

FreeTypeFont::~FreeTypeFont()
{
    delete atlas;
}

Font::Glyph *FreeTypeFont::GetGlyph(char32_t character)
{
    // return nullptr if the given character is not in this font
    if (glyphs.find(character) == glyphs.end())
        return nullptr;

    // return the glyph for the given character
    return &glyphs[character];
}
