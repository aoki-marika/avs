#pragma once

#include "Reader.hpp"

class ByteReader : public Reader
{
    private:
        // the bytes for this reader to read from
        const unsigned char *bytes;

    public:
        // create a byte reader for the given bytes
        ByteReader(const unsigned char *bytes);

        void ReadBytes(unsigned int offset,
                       unsigned int num_bytes,
                       unsigned char *out);
};
