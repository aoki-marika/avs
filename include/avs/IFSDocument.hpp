#pragma once

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
            // todo: use a FILE or similar so only loaded files are in memory
            // the current plan is to add support for reading from a FILE to ByteBuffer, and pass a file path here
            Document(const unsigned char *source);
            ~Document();
    };
};
