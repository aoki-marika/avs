#pragma once

#include <vector>

#include "IFSDirectory.hpp"
#include "IFSTexture.hpp"
#include "Atlas.hpp"

namespace IFS
{
    // tex directory
    class TextureDirectory : public Directory
    {
        private:
            // the textures of this directory
            std::vector<Texture *> textures;

        public:
            TextureDirectory(KML::Node *node,
                             ByteBuffer *data_buffer,
                             std::string name);
            ~TextureDirectory();

            // get a pointer to the first atlas of the given name from this directory
            // returns nullptr if no match is found
            Atlas *GetAtlas(std::string name);
    };
};
