#pragma once

#include <stdint.h>

#include "ByteBuffer.hpp"

class LZ77
{
    private:
        static const uint16_t window_size = 0x1000;
        static const uint16_t window_mask = window_size - 1;
        static const uint16_t threshold = 3;

    public:
        // decompress the compressed lz77 in the given buffer, starting from the given offset, if any
        // reads compressed_size bytes from in and writes the decompressed data to out
        // in should be of length compressed_size
        // out should be of length decompressed_size
        static void Decompress(unsigned char *in,
                               unsigned char *out,
                               unsigned int compressed_size,
                               unsigned int decompressed_size,
                               unsigned int start_offset = 0)
        {

            // decompress the data
            unsigned int inpos = start_offset, outpos = 0;
            unsigned char window[window_size] = { 0 };
            unsigned int window_cursor = 0;
            while (inpos < compressed_size)
            {
                unsigned char flag = in[inpos++];
                for (int i = 0; i < 8; i++)
                {
                    if ((flag >> i) & 1 == 1)
                    {
                        out[outpos++] = in[inpos];
                        window[window_cursor] = in[inpos];
                        window_cursor = (window_cursor + 1) & window_mask;
                        inpos++;
                    }
                    else
                    {
                        unsigned int w = in[inpos++] << 8 | in[inpos++];
                        if (w == 0)
                        {
                            return;
                        }

                        unsigned int position = (window_cursor - (w >> 4)) & window_mask;
                        unsigned int length = (w & 0x0f) + threshold;
                        for (int l = 0; l < length; l++)
                        {
                            unsigned int b = window[position & window_mask];
                            out[outpos++] = b;
                            window[window_cursor] = b;
                            window_cursor = (window_cursor + 1) & window_mask;
                            position++;
                        }
                    }
                }
            }
        }
};
