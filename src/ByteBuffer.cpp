#include "ByteBuffer.hpp"

ByteBuffer::ByteBuffer(const unsigned char *buffer)
{
    this->buffer = buffer;
}

uint8_t ByteBuffer::ReadU8()
{
    uint8_t value = buffer[offset];
    offset += 1;
    return value;
}

uint16_t ByteBuffer::ReadU16()
{
    uint16_t value = buffer[offset + 0] << 8 |
                     buffer[offset + 1] << 0;
    offset += 2;
    return value;
}

uint32_t ByteBuffer::ReadU32()
{
    uint32_t value = buffer[offset + 0] << 24 |
                     buffer[offset + 1] << 16 |
                     buffer[offset + 2] << 8 |
                     buffer[offset + 3] << 0;
    offset += 4;
    return value;
}
