#pragma once

#include <string>
#include <stdint.h>

#include "IFSDirectory.hpp"
#include "ByteBuffer.hpp"
#include "Atlas.hpp"
#include "IFSTextureDirectory.hpp"

namespace IFS
{
    class Document
    {
        private:
            // the first four bytes of every ifs file should match this signature
            const uint32_t signature = 0x6cad8f89;

            // the root folder of this document
            Directory *root;

            // the buffer for this ifs files to load file data from
            ByteBuffer *data_buffer;

        public:
            // create an ifs document from the file at the given path
            Document(std::string path);
            ~Document();

            // calls GetAtlas() on this documents tex directory, if any
            // see: IFS::TextureDirectory::GetAtlas(std::string); for details
            // returns nullptr if no tex directory is present in this document
            Atlas *GetAtlas(std::string name);

            // get the root directory of this document
            Directory *GetRoot()
            {
                return root;
            }
    };
};
