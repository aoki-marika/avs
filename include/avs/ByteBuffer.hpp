#pragma once

#include <stdint.h>
#include <limits.h>

#include "Reader.hpp"

class ByteBuffer
{
    private:
        // the reader this buffer is reading from
        Reader *reader;

        // the current offset this buffer is reading from
        unsigned int offset;

        // the end offset of the data of this buffer
        unsigned int end_offset = UINT_MAX;

        // check if the given offset is beyond end_offset, and if it is then it throws an exception
        void checkEnd(unsigned int new_offset);

    public:
        ByteBuffer(const unsigned char *buffer, unsigned int start_offset = 0);

        // get the current reading offset of this buffer
        unsigned int GetOffset()
        {
            return offset;
        }

        // set the current reading offset of this buffer
        void SetOffset(unsigned int new_offset)
        {
            offset = new_offset;
        }

        // set the end offset of the data of this buffer
        // defaults to no end
        void SetEnd(unsigned int end_offset);

        // returns whether or not this buffer has read all of its data
        bool AtEnd();

        // read num_bytes bytes from this buffer into out, and increment the reading offset
        // out should be num_bytes in size
        void ReadBytes(unsigned int num_bytes, unsigned char *out);

        // read various data types and increment the reading offset
        uint8_t ReadU8();
        uint16_t ReadU16();
        int32_t ReadS32();
        uint32_t ReadU32();

        // realign the reading position to a grid of the given size in bytes
        void RealignReads(unsigned int size = 4);
};
