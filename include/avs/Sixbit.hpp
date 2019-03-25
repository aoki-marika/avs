#pragma once

#include <stdint.h>
#include <string>

#include "ByteBuffer.hpp"

class Sixbit
{
    private:
        static char characterForId(unsigned int id)
        {
            switch (id)
            {
                case 0:  return '0';
                case 1:  return '1';
                case 2:  return '2';
                case 3:  return '3';
                case 4:  return '4';
                case 5:  return '5';
                case 6:  return '6';
                case 7:  return '7';
                case 8:  return '8';
                case 9:  return '9';
                case 10: return ':';
                case 11: return 'A';
                case 12: return 'B';
                case 13: return 'C';
                case 14: return 'D';
                case 15: return 'E';
                case 16: return 'F';
                case 17: return 'G';
                case 18: return 'H';
                case 19: return 'I';
                case 20: return 'J';
                case 21: return 'K';
                case 22: return 'L';
                case 23: return 'M';
                case 24: return 'N';
                case 25: return 'O';
                case 26: return 'P';
                case 27: return 'Q';
                case 28: return 'R';
                case 29: return 'S';
                case 30: return 'T';
                case 31: return 'U';
                case 32: return 'V';
                case 33: return 'W';
                case 34: return 'X';
                case 35: return 'Y';
                case 36: return 'Z';
                case 37: return '_';
                case 38: return 'a';
                case 39: return 'b';
                case 40: return 'c';
                case 41: return 'd';
                case 42: return 'e';
                case 43: return 'f';
                case 44: return 'g';
                case 45: return 'h';
                case 46: return 'i';
                case 47: return 'j';
                case 48: return 'k';
                case 49: return 'l';
                case 50: return 'm';
                case 51: return 'n';
                case 52: return 'o';
                case 53: return 'p';
                case 54: return 'q';
                case 55: return 'r';
                case 56: return 's';
                case 57: return 't';
                case 58: return 'u';
                case 59: return 'v';
                case 60: return 'w';
                case 61: return 'x';
                case 62: return 'y';
                case 63: return 'z';
            }
        }

    public:
        // unpack the sixbit string at the reading offset of the given source buffer
        static std::string Unpack(ByteBuffer *source_buffer)
        {
            // get the length of the string
            uint8_t length = source_buffer->ReadU8();

            // get the length of the string in bits and bytes
            unsigned int length_bits = length * 6;
            unsigned int length_bytes = (length_bits + 7) / 8;

            // get the string data padding
            unsigned int padding = 8 - (length_bits % 8);
            if (padding == 8)
                padding = 0;

            // read the string bytes
            unsigned char bytes[length_bytes];
            source_buffer->ReadBytes(length_bytes, bytes);

            // convert the bytes to a big endian unsigned int
            unsigned int bits = 0;
            for (int i = 0; i < length_bytes; i++)
                bits = (bits << 8) | bytes[i];
            bits >>= padding;

            // get the characters for bits
            char result[length + 1];
            for (int i = length - 1; i >= 0; i--)
            {
                result[i] = characterForId(bits & 0b111111);
                bits >>= 6;
            }
            result[length] = '\0';

            // return the decoded string
            return std::string(result);
        }
};
