#pragma once

#include <string>
#include <stdint.h>

#include "IFSDirectory.hpp"
#include "ByteBuffer.hpp"
#include "IFSDirectoryTexture.hpp"

namespace IFS
{
    class Document
    {
        private:
            // the first four bytes of every ifs file should match this signature
            const uint32_t signature = 0x6cad8f89;

            // the root folder of this document
            IFS::Directory *root;

            // the buffer for this ifs files to load file data from
            ByteBuffer *data_buffer;

        public:
            // create an ifs document from the file at the given path
            Document(std::string path);
            ~Document();

            // get the root directory of this document
            IFS::Directory *GetRoot()
            {
                return root;
            }

            // get the texture directory of this document, if any
            // returns nullptr if none is found
            IFS::DirectoryTexture *GetTextureDirectory()
            {
                return (IFS::DirectoryTexture *)GetRoot()->GetEntry("tex");
            }
    };
};
