#include "ByteBuffer.hpp"

#include <stdexcept>

#include "Utilities.hpp"

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
    uint8_t value = Utilities::BytesToU8(buffer, offset);
    offset += sizeof(uint8_t);
    return value;
}

uint16_t ByteBuffer::ReadU16()
{
    checkEnd(offset);
    uint16_t value = Utilities::BytesToU16(buffer, offset);
    offset += sizeof(uint16_t);
    return value;
}

int32_t ByteBuffer::ReadS32()
{
    checkEnd(offset);
    int32_t value = Utilities::BytesToS32(buffer, offset);
    offset += sizeof(int32_t);
    return value;
}

uint32_t ByteBuffer::ReadU32()
{
    checkEnd(offset);
    uint32_t value = Utilities::BytesToU32(buffer, offset);
    offset += sizeof(uint32_t);
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
