#include "FreeType.hpp"

#include <stdexcept>
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library FreeType::Library;

void FreeType::Init()
{
    FT_Error error = FT_Init_FreeType(&FreeType::Library);
    if (error != FT_Err_Ok)
    {
        const char *format = "Unable to initialize FreeType (0x%02x)";
        char message[strlen(format)];
        sprintf(message, format, error);
        throw new std::runtime_error(message);
    }
}

void FreeType::Deinit()
{
    FT_Done_FreeType(FreeType::Library);
}
