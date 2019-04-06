#pragma once

#include "IFSDirectoryMD5.hpp"

namespace IFS
{
    // tex directory
    class DirectoryTexture : public DirectoryMD5
    {
        public:
            DirectoryTexture(KML::Node *node,
                             ByteBuffer *data_buffer,
                             std::string name);
    };
};
