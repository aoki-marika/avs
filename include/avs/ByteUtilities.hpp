#pragma once

#include <string.h>
#include <stdint.h>
#include <math.h>

namespace ByteUtilities
{
    // note: all of these functions are designed for big endian in konamis binary file formats
    // dont rely on them to work properly in other cases

    static int8_t BytesToS8(const unsigned char *bytes, int offset)
    {
        return (int8_t)bytes[offset];
    }

    static int16_t BytesToS16(const unsigned char *bytes, int offset)
    {
        return (int16_t)bytes[offset + 0] << 8 |
               (int16_t)bytes[offset + 1] << 0;
    }

    static int32_t BytesToS32(const unsigned char *bytes, int offset)
    {
        return (int32_t)bytes[offset + 0] << 24 |
               (int32_t)bytes[offset + 1] << 16 |
               (int32_t)bytes[offset + 2] << 8 |
               (int32_t)bytes[offset + 3] << 0;
    }

    static int64_t BytesToS64(const unsigned char *bytes, int offset)
    {
        return (int64_t)bytes[offset + 0] << 56 |
               (int64_t)bytes[offset + 1] << 48 |
               (int64_t)bytes[offset + 2] << 40 |
               (int64_t)bytes[offset + 3] << 32 |
               (int64_t)bytes[offset + 4] << 24 |
               (int64_t)bytes[offset + 5] << 16 |
               (int64_t)bytes[offset + 6] << 8 |
               (int64_t)bytes[offset + 7] << 0;
    }

    static uint8_t BytesToU8(const unsigned char *bytes, int offset)
    {
        return (uint8_t)BytesToS8(bytes, offset);
    }

    static uint16_t BytesToU16(const unsigned char *bytes, int offset)
    {
        return (uint16_t)BytesToS16(bytes, offset);
    }

    static uint32_t BytesToU32(const unsigned char *bytes, int offset)
    {
        return (uint32_t)BytesToS32(bytes, offset);
    }

    static uint64_t BytesToU64(const unsigned char *bytes, int offset)
    {
        return (uint64_t)BytesToS64(bytes, offset);
    }

    static float BytesToFloat(const unsigned char *bytes, int offset)
    {
        int32_t bits = BytesToS32(bytes, offset);
        float value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    static double BytesToDouble(const unsigned char *bytes, int offset)
    {
        int64_t bits = BytesToS64(bytes, offset);
        double value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    static bool BytesToBool(const unsigned char *bytes, int offset)
    {
        if (bytes[offset] == 0x01)
            return true;
        else
            return false;
    }
};
