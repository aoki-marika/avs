#pragma once

#include <vector>

#include "IFSDirectory.hpp"
#include "IFSTexture.hpp"
#include "Atlas.hpp"

namespace IFS
{
    // tex directory
    // todo: -> TextureDirectory
    class DirectoryTexture : public Directory
    {
        private:
            // the textures of this directory
            std::vector<Texture *> textures;

        public:
            DirectoryTexture(KML::Node *node,
                             ByteBuffer *data_buffer,
                             std::string name);
            ~DirectoryTexture();

            // get a pointer to the first atlas of the given name from this directory
            // returns nullptr if no match is found
            Atlas *GetAtlas(std::string name);
    };
};
