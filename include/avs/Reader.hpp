#pragma once

// generic protocol for reading bytes from various sources
class Reader
{
    public:
        // read num_bytes bytes at offset from this reader into out
        virtual void ReadBytes(unsigned int offset,
                               unsigned int num_bytes,
                               unsigned char *out) = 0;
};
