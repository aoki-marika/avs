#pragma once

#include "IFSEntry.hpp"
#include "ByteBuffer.hpp"

namespace IFS
{
    class File : public Entry
    {
        private:
            // the buffer for this file to load data from
            ByteBuffer *data_buffer;

            // the start offset and size of this files data
            int32_t data_offset, data_size;

        public:
            // create a file filesystem entry from the given kml node with the given name
            // data_buffer is the ByteBuffer for this file to read data from
            File(KML::Node *node,
                 ByteBuffer *data_buffer,
                 std::string name);

            // get the size of the data of this file
            // used for creating an output array to pass to GetData()
            int32_t GetSize()
            {
                return data_size;
            }

            // get the data of this file and put it into into the given output
            void GetData(unsigned char *out);
    };
};
