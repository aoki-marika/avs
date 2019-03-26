#include "ByteBuffer.hpp"

#include <stdexcept>

ByteBuffer::ByteBuffer(const unsigned char *buffer, unsigned int start_offset)
{
    this->buffer = buffer;
    this->offset = start_offset;
}

void ByteBuffer::SetEnd(unsigned int end_offset)
{
    this->end_offset = end_offset;
}

bool ByteBuffer::AtEnd()
{
    return offset >= end_offset - 1;
}

void ByteBuffer::ReadBytes(unsigned int num_bytes, unsigned char *out)
{
    checkEnd(offset);

    for (int i = 0; i < num_bytes; i++)
        out[i] = buffer[offset + i];

    offset += num_bytes;
}

uint8_t ByteBuffer::ReadU8()
{
    checkEnd(offset);

    uint8_t value = buffer[offset];

    offset += 1;
    return value;
}

uint16_t ByteBuffer::ReadU16()
{
    checkEnd(offset);

    uint16_t value = buffer[offset + 0] << 8 |
                     buffer[offset + 1] << 0;

    offset += 2;
    return value;
}

int32_t ByteBuffer::ReadS32()
{
    checkEnd(offset);

    uint32_t value = buffer[offset + 0] << 24 |
                     buffer[offset + 1] << 16 |
                     buffer[offset + 2] << 8 |
                     buffer[offset + 3] << 0;

    offset += 4;
    return value;
}

uint32_t ByteBuffer::ReadU32()
{
    checkEnd(offset);

    uint32_t value = buffer[offset + 0] << 24 |
                     buffer[offset + 1] << 16 |
                     buffer[offset + 2] << 8 |
                     buffer[offset + 3] << 0;

    offset += 4;
    return value;
}

void ByteBuffer::RealignReads(unsigned int size)
{
    while (offset % size)
        offset++;
}

void ByteBuffer::checkEnd(unsigned int new_offset)
{
    if (new_offset >= end_offset)
        throw new std::runtime_error("This buffer has reached the end of it's data");
}
