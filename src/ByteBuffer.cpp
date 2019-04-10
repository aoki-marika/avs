#include "ByteBuffer.hpp"

#include <stdexcept>

#include "Utilities.hpp"
#include "ByteReader.hpp"
#include "FileReader.hpp"

ByteBuffer::ByteBuffer(const unsigned char *buffer, unsigned int start_offset)
{
    this->reader = new ByteReader(buffer);
    this->offset = start_offset;
}

ByteBuffer::ByteBuffer(std::string path, unsigned int start_offset)
{
    this->reader = new FileReader(path);
    this->offset = start_offset;
}

ByteBuffer::~ByteBuffer()
{
    delete reader;
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
    reader->ReadBytes(offset, num_bytes, out);
    offset += num_bytes;
}

uint8_t ByteBuffer::ReadU8()
{
    checkEnd(offset);

    unsigned char bytes[sizeof(uint8_t)];
    reader->ReadBytes(offset, sizeof(uint8_t), bytes);
    offset += sizeof(uint8_t);

    return Utilities::BytesToU8(bytes, 0);
}

uint16_t ByteBuffer::ReadU16()
{
    checkEnd(offset);

    unsigned char bytes[sizeof(uint16_t)];
    reader->ReadBytes(offset, sizeof(uint16_t), bytes);
    offset += sizeof(uint16_t);

    return Utilities::BytesToU16(bytes, 0);
}

int32_t ByteBuffer::ReadS32()
{
    checkEnd(offset);

    unsigned char bytes[sizeof(int32_t)];
    reader->ReadBytes(offset, sizeof(int32_t), bytes);
    offset += sizeof(int32_t);

    return Utilities::BytesToS32(bytes, 0);
}

uint32_t ByteBuffer::ReadU32()
{
    checkEnd(offset);

    unsigned char bytes[sizeof(uint32_t)];
    reader->ReadBytes(offset, sizeof(uint32_t), bytes);
    offset += sizeof(uint32_t);

    return Utilities::BytesToU32(bytes, 0);
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
