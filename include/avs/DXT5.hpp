#pragma once

#include "ByteBuffer.hpp"

// note: dxt5 manually creates integers from bytes instead of Read[S/U][8/16/32/64] as it uses little endian
// and is probably the only thing in the project that will use it, so its not worth going through and adding support for it
class DXT5
{
    private:
        // convert the rgb565 values in the given colour to rgb888
        // writes the rgb888 channel values to the given channel pointers
        static void rgb565ToRgb888(uint16_t colour,
                                   uint8_t *r,
                                   uint8_t *g,
                                   uint8_t *b)
        {
            int temp;

            // for some reason blue and green are swapped in the dxt5 textures ive tested
            // just swapping the assignment here makes all the colours correct

            // red
            temp = (colour >> 11) * 255 + 16;
            *r = (temp / 32 + temp) / 32;

            // green
            temp = (colour & 0x001f) * 255 + 16;
            *g = (temp / 32 + temp) / 32;

            // blue
            temp = ((colour & 0x07e0) >> 5) * 255 + 32;
            *b = (temp / 64 + temp) / 64;
        }

        // decompress the block at the given x and y position from the given buffer into the given output
        // x and y positions are in blocks, width and height are in pixels
        static void decompressBlock(ByteBuffer *buffer,
                                    unsigned int x,
                                    unsigned int y,
                                    unsigned int width,
                                    unsigned int height,
                                    unsigned char *out)
        {
            // read the alpha values
            uint8_t alpha0 = buffer->ReadByte();
            uint8_t alpha1 = buffer->ReadByte();

            // read the alpha mask
            uint64_t alpha_mask = (uint64_t)buffer->ReadByte();
            alpha_mask += (uint64_t)buffer->ReadByte() << 8;
            alpha_mask += (uint64_t)buffer->ReadByte() << 16;
            alpha_mask += (uint64_t)buffer->ReadByte() << 24;
            alpha_mask += (uint64_t)buffer->ReadByte() << 32;
            alpha_mask += (uint64_t)buffer->ReadByte() << 40;

            // read the rgb565 colours
            uint16_t c0 = buffer->ReadByte() << 0 |
                          buffer->ReadByte() << 8;
            uint16_t c1 = buffer->ReadByte() << 0 |
                          buffer->ReadByte() << 8;

            // convert the rgb565 colours to rgb888 channels
            uint8_t r0, g0, b0;
            uint8_t r1, g1, b1;
            rgb565ToRgb888(c0, &r0, &b0, &g0);
            rgb565ToRgb888(c1, &r1, &b1, &g1);

            // read the lookup table
            uint32_t lookup_table = buffer->ReadByte() << 0 |
                                    buffer->ReadByte() << 8 |
                                    buffer->ReadByte() << 16 |
                                    buffer->ReadByte() << 24;

            // for each pixel in this block
            for (int block_y = 0; block_y < 4; block_y++)
            {
                for (int block_x = 0; block_x < 4; block_x++)
                {
                    // the rgba values of this pixel
                    uint8_t r = 0, g = 0, b = 0, a = 255;

                    // get the correct alpha value
                    unsigned int alpha_index = (alpha_mask >> 3 * (4 * block_y + block_x)) & 0x07;
                    if (alpha_index == 0)
                        a = alpha0;
                    else if (alpha_index == 1)
                        a = alpha1;
                    else if (alpha0 > alpha1)
                        a = ((8 - alpha_index) * alpha0 + (alpha_index - 1) * alpha1) / 7;
                    else if (alpha_index == 6)
                        a = 0;
                    else if (alpha_index == 7)
                        a = 255;
                    else
                        a = ((6 - alpha_index) * alpha0 + (alpha_index - 1) * alpha1) / 5;

                    // get the correct rgb channel values
                    unsigned int index = (lookup_table >> 2 * (4 * block_y + block_x)) & 0x03;
                    switch (index)
                    {
                        case 0:
                            r = r0;
                            g = g0;
                            b = b0;
                            break;
                        case 1:
                            r = r1;
                            g = g1;
                            b = b1;
                            break;
                        case 2:
                            r = (2 * r0 + r1) / 3;
                            g = (2 * g0 + g1) / 3;
                            b = (2 * b0 + b1) / 3;
                            break;
                        case 3:
                            r = (r0 + 2 * r1) / 3;
                            g = (g0 + 2 * g1) / 3;
                            b = (b0 + 2 * b1) / 3;
                            break;
                    }

                    // ensure this pixel is in range of the image
                    int px = (x << 2) + block_x;
                    int py = (y << 2) + block_y;
                    if ((px < width) && (py < height))
                    {
                        // get the index of this pixel in the output buffer and write it
                        int offset = ((py * width) + px) << 2;
                        out[offset + 0] = r;
                        out[offset + 1] = g;
                        out[offset + 2] = b;
                        out[offset + 3] = a;
                    }
                }
            }
        }

    public:
        // decode a dxt5 encoded image of the given width and height from in
        // writes the decoded rgba pixel data to out
        // out should be of length width * height * 4
        static void Decode(unsigned int width,
                           unsigned int height,
                           unsigned char *in,
                           unsigned char *out)
        {
            // get the number of blocks in the image
            int blocks_x = (width + 3) / 4;
            int blocks_y = (height + 3) / 4;

            // create the reading buffer
            ByteBuffer *buffer = new ByteBuffer(in);

            // decompress all the blocks
            for (int y = 0; y < blocks_y; y++)
                for (int x = 0; x < blocks_x; x++)
                    decompressBlock(buffer, x, y, width, height, out);

            // delete the buffer now that the data is read
            delete buffer;
        }
};
