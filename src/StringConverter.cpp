#include "StringConverter.hpp"

#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <unicode/ucnv.h>

StringConverter::StringConverter(StringEncoding encoding)
{
    // get the icu name of the encoding
    std::string name;
    switch (encoding)
    {
        case StringEncoding::ASCII:      name = "ASCII";      break;
        case StringEncoding::ISO_8859_1: name = "ISO-8859-1"; break;
        case StringEncoding::EUC_JP:     name = "EUC-JP";     break;
        case StringEncoding::CP932:      name = "cp932";      break;
        case StringEncoding::UTF8:       name = "UTF-8";      break;
    }

    // create the converter
    UErrorCode error = U_ZERO_ERROR;
    converter = ucnv_open(name.c_str(), &error);
    if (U_FAILURE(error))
    {
        const char *format = "Unable to create converter for encoding \"%s\" (%i)";
        char message[strlen(format) + name.length()];
        sprintf(message, format, name.c_str(), error);
        throw new std::runtime_error(message);
    }
}

StringConverter::~StringConverter()
{
    ucnv_close(converter);
}

unsigned int StringConverter::GetBufferSize(std::string string)
{
    return UCNV_GET_MAX_BYTES_FOR_STRING(string.length(), ucnv_getMaxCharSize(converter));
}

unsigned int StringConverter::Encode(std::string string, unsigned char *out_bytes)
{
    // the last icu error
    UErrorCode error = U_ZERO_ERROR;

    // get the unicode characters of the given string
    const uint32_t num_unicode_characters = string.length();
    UChar unicode_characters[num_unicode_characters];
    ucnv_toUChars(converter, unicode_characters, num_unicode_characters, string.c_str(), string.length(), &error);

    // decode the unicode characters
    const uint32_t num_characters = GetBufferSize(string);
    char characters[num_characters];
    int32_t length = ucnv_fromUChars(converter, characters, num_characters, unicode_characters, num_unicode_characters, &error);

    if (U_FAILURE(error))
    {
        const char *format = "Unable to encode string \"%s\" to Unicode (%i)";
        char message[strlen(format) + string.length()];
        sprintf(message, format, string.c_str(), error);
        throw new std::runtime_error(message);
    }

    // write the characters to the given output buffer
    for (int i = 0; i < length; i++)
        out_bytes[i] = (unsigned char)characters[i];

    return length;
}

std::string StringConverter::Decode(unsigned char *bytes, unsigned int num_bytes)
{
    // convert the given bytes to unicode
    const char *characters = (char *)bytes;
    const uint32_t num_unicode_characters = num_bytes * 2;
    UChar unicode_characters[num_unicode_characters];
    UErrorCode error = U_ZERO_ERROR;
    ucnv_toUChars(converter, unicode_characters, num_unicode_characters, characters, num_bytes, &error);

    if (U_FAILURE(error))
    {
        const char *format = "Unable to convert bytes to Unicode (%i)";
        char message[strlen(format)];
        sprintf(message, format, error);
        throw new std::runtime_error(message);
    }

    // convert the unicode characters into a string
    UnicodeString unicode_string = UnicodeString(unicode_characters);
    std::string string;
    unicode_string.toUTF8String(string);

    // return the string
    return string;
}
