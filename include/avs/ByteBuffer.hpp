#pragma once

#include <stdint.h>

class ByteBuffer
{
    private:
        // the buffer this buffer is reading from
        const unsigned char *buffer;

        // the current offset this buffer is reading from
        unsigned int offset = 0;

    public:
        ByteBuffer(const unsigned char *buffer);

        // read various data types and increment the reading offset
        uint8_t ReadU8();
        uint16_t ReadU16();
        uint32_t ReadU32();
};
