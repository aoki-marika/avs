#pragma once

#include <vector>
#include <stdint.h>

#include "ByteBuffer.hpp"

class LZ77
{
    private:
        static const uint16_t threshold = 3;

    public:
        // decompress the given lz77 compressed data, from the given start offset (if any)
        // returns a vector of the decompressed bytes of length decompressed_length
        static std::vector<unsigned char> Decompress(unsigned char *data,
                                                     unsigned int data_length,
                                                     unsigned int decompressed_length,
                                                     unsigned int start_offset = 0)
        {
            // create the reading buffer and decompressed vector
            ByteBuffer *buffer = new ByteBuffer(data);
            std::vector<unsigned char> decompressed(decompressed_length);

            // the current offset to insert new bytes into decompressed at
            unsigned int offset = 0;

            // advance the buffer to the given start offset
            unsigned char ignore[start_offset];
            buffer->ReadBytes(start_offset, ignore);

            // decompress the given data
            while (true)
            {
                unsigned char flag = buffer->ReadByte();

                for (int i = 0; i < 8; i++)
                {
                    if ((flag >> i) & 1 == 1)
                    {
                        decompressed[offset] = buffer->ReadByte();
                        offset++;
                    }
                    else
                    {
                        int width = buffer->ReadU16();
                        int position = width >> 4;
                        int length = (width & 0x0f) + threshold;

                        // return the decompressed data if at the end of the data
                        if (position == 0)
                        {
                            delete buffer;
                            return decompressed;
                        }

                        if (position > offset)
                        {
                            int difference = position - offset;
                            if (length < difference)
                                difference = length;

                            for (int e = 0; e < difference; e++)
                            {
                                decompressed[offset + e] = 0;
                                offset++;
                            }

                            length -= difference;
                        }

                        if (-position + length < 0)
                        {
                            int start = offset - position;
                            for (int e = start; e < start + length; e++)
                            {
                                decompressed[offset] = decompressed[e];
                                offset++;
                            }
                        }
                        else
                        {
                            for (int e = 0; e < length; e++)
                            {
                                decompressed[offset] = decompressed[offset - position];
                                offset++;
                            }
                        }
                    }
                }
            }
        }
};
