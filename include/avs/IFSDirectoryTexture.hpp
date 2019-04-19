#pragma once

#include <vector>

#include "IFSDirectory.hpp"
#include "IFSTexture.hpp"

namespace IFS
{
    // tex directory
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

            // get a pointer to the first texture of this directory matching the given name
            // returns nullptr if none is found
            Texture *GetTexture(std::string name);
    };
};
