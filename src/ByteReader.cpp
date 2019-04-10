#include "ByteReader.hpp"

ByteReader::ByteReader(const unsigned char *bytes)
{
    this->bytes = bytes;
}

void ByteReader::ReadBytes(unsigned int offset,
                           unsigned int num_bytes,
                           unsigned char *out)
{
    for (int i = 0; i < num_bytes; i++)
        out[i] = bytes[offset + i];
}
