#pragma once

#include <string>
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

        // the base offset of this buffer
        // this offset is used as the base offset when setting offset
        // e.g. a base offset of 36 and an offset of 4 will have a final offset of 40
        unsigned int base_offset;

        // the end offset of the data of this buffer
        unsigned int end_offset = UINT_MAX;

    public:
        // create a buffer from the given byte array with the given start and base offsets
        ByteBuffer(const unsigned char *buffer,
                   unsigned int start_offset = 0,
                   unsigned int base_offset = 0);

        // create a buffer from the file at the given path with the given start and base offsets
        ByteBuffer(std::string path,
                   unsigned int start_offset = 0,
                   unsigned int base_offset = 0);

        ~ByteBuffer();

        // get the current reading offset of this buffer
        unsigned int GetOffset()
        {
            return offset;
        }

        // set the current reading offset of this buffer
        void SetOffset(unsigned int new_offset)
        {
            offset = base_offset + new_offset;
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
        unsigned char ReadByte();

        // realign the reading position to a grid of the given size in bytes
        void RealignReads(unsigned int size = 4);
};
