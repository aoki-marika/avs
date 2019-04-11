#pragma once

#include <string>

enum StringEncoding
{
    ASCII,
    ISO_8859_1,
    EUC_JP,
    CP932,
    UTF8,
};

struct UConverter;
class StringConverter
{
    private:
        UConverter *converter;

    public:
        // create a string converter that encodes/decodes for the given encoding
        StringConverter(StringEncoding encoding);
        ~StringConverter();

        // get the size of a buffer to pass to Encode.
        // string should be the same string as the one passed to Encode.
        unsigned int GetBufferSize(std::string string);

        // encode the given string and store the bytes in the given output
        // out_bytes should be the length returned by GetBufferSize
        // returns the number of bytes written to out_bytes
        unsigned int Encode(std::string string, unsigned char *out_bytes);

        // decode the given bytes to a string
        std::string Decode(unsigned char *bytes, unsigned int num_bytes);
};
