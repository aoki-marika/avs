#pragma once

#include <string>
#include <stdint.h>

#include "IFSDirectory.hpp"
#include "ByteBuffer.hpp"

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
    };
};
