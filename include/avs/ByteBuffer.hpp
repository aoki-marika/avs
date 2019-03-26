#pragma once

#include <stdint.h>
#include <limits.h>

class ByteBuffer
{
    private:
        // the buffer this buffer is reading from
        const unsigned char *buffer;

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

        // read various data types and increment the reading offset
        uint8_t ReadU8();
        uint16_t ReadU16();
        int32_t ReadS32();
        uint32_t ReadU32();
};
