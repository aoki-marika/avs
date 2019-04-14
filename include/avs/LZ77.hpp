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
        // returns a vector of the decompressed bytes
        static std::vector<unsigned char> Decompress(unsigned char *data,
                                                     unsigned int data_length,
                                                     unsigned int start_offset = 0)
        {
            // create the reading buffer and decompressed vector
            ByteBuffer *buffer = new ByteBuffer(data);
            std::vector<unsigned char> decompressed;

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
                        decompressed.push_back(buffer->ReadByte());
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

                        if (position > decompressed.size())
                        {
                            int difference = position - decompressed.size();
                            if (length < difference)
                                difference = length;

                            decompressed.insert(decompressed.end(), difference, 0);
                            length -= difference;
                        }

                        if (-position + length < 0)
                        {
                            int start = decompressed.size() - position;
                            for (int e = start; e < start + length; e++)
                                decompressed.push_back(decompressed[e]);
                        }
                        else
                        {
                            for (int e = 0; e < length; e++)
                                decompressed.push_back(decompressed[decompressed.size() - position]);
                        }
                    }
                }
            }
        }
};
