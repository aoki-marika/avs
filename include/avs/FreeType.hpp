#pragma once

#include <string>

struct FT_LibraryRec_;
typedef FT_LibraryRec_ *FT_Library;
struct FT_FaceRec_;
typedef FT_FaceRec_ *FT_Face;

// small wrapper around a global freetype instance
class FreeType
{
    public:
        // the global freetype library
        static FT_Library Library;

        // init/deinit the global freetype instance
        // should be called at the beginning and end of the program, respectively
        static void Init();
        static void Deinit();
};
