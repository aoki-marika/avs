#include "KMLDocument.hpp"

#include <stdexcept>
#include <unicode/ucnv.h>

#include "Sixbit.hpp"
#include "Utilities.hpp"

KML::Document::Document(const unsigned char *source)
{
    // create the buffer for reading the header and nodes
    ByteBuffer *node_buffer = new ByteBuffer(source);

    // ensure the given buffer matches the kml signature
    if (node_buffer->ReadU8() != signature)
        throw new std::runtime_error("Given source is not of a KML file (invalid signature)");

    // read the compression and encoding bytes
    KML::Compression compression = KML::Compression(node_buffer->ReadU8());
    KML::Encoding encoding = KML::Encoding(node_buffer->ReadU8());

    // the next byte should always be equal to NOT encoding
    if (!(node_buffer->ReadU8() == 0xFF ^ encoding))
        throw std::runtime_error("Given source is not of a valid KML file (NOT encoding incorrect)");

    // get the unicode converter for the documents encoding
    std::string encoding_name;
    switch (encoding)
    {
        case KML::Encoding::ASCII:      encoding_name = "ASCII";      break;
        case KML::Encoding::ISO_8859_1: encoding_name = "ISO-8859-1"; break;
        case KML::Encoding::EUC_JP:     encoding_name = "EUC-JP";     break;
        case KML::Encoding::CP932:      encoding_name = "cp932";      break;
        case KML::Encoding::UTF8:       encoding_name = "UTF-8";      break;
    }

    UErrorCode error = U_ZERO_ERROR;
    UConverter *converter = ucnv_open(encoding_name.c_str(), &error);
    if (U_FAILURE(error))
    {
        const char *format = "Unable to create decoding converter for encoding \"%s\" (%i)";
        char message[strlen(format) + encoding_name.length()];
        sprintf(message, format, encoding_name.c_str(), error);
        throw new std::runtime_error(message);
    }

    // read and set the end offset of the node data
    uint32_t node_end_offset = node_buffer->ReadU32() + 8;
    node_buffer->SetEnd(node_end_offset);

    // create the data buffers
    ByteBuffer *data_buffer = new ByteBuffer(source, node_end_offset);
    ByteBuffer *byte_buffer = new ByteBuffer(source, node_end_offset);
    ByteBuffer *word_buffer = new ByteBuffer(source, node_end_offset);

    // read the data size and discard it, as its unused here
    data_buffer->ReadU32();

    // create the root node
    root = new KML::Node("root");

    // the current node the parser is adding attributes/children to
    KML::Node *node = root;

    // read the document
    bool nodes_left = true;
    while (nodes_left && !node_buffer->AtEnd())
    {
        // get the type of the current node
        uint8_t type = node_buffer->ReadU8();

        // skip 0 nodes as they contain no data
        if (type == 0)
            continue;

        // get whether or not the current node is an array
        bool is_array = type & 64;
        type &= ~64;

        // get the current nodes format
        KML::NodeFormat format = formatForType(type);

        // get the current nodes name
        std::string name;
        if (format & ~KML::NodeFormat::NodeEnd && format & ~KML::NodeFormat::SectionEnd)
        {
            switch (compression)
            {
                case KML::Compression::Compressed:
                    name = Sixbit::Unpack(node_buffer);
                    break;
                case KML::Compression::Uncompressed:
                    uint8_t length = node_buffer->ReadU8();
                    unsigned char bytes[length];
                    node_buffer->ReadBytes(length, bytes);
                    name = decodeString(converter, bytes, length);
                    break;
            }
        }

        if (format & KML::NodeFormat::Attribute)
        {
            node->AddAttribute(name, grabString(converter, data_buffer));
        }
        else if (format & KML::NodeFormat::NodeEnd)
        {
            if (node->GetParent() != nullptr)
                node = node->GetParent();
        }
        else if (format & KML::NodeFormat::SectionEnd)
        {
            nodes_left = false;
        }
        else
        {
            if (format & KML::NodeFormat::NodeStart)
            {
                // create a non-value node as start nodes have no values
                KML::Node *child = new KML::Node(name);
                node->AddChild(child);
                node = child;
                continue;
            }

            // get the value and array counts
            int num_values = formatValues(format);
            int num_array = 1;
            if (num_values == format_values_dynamic)
            {
                num_values = data_buffer->ReadU32();
                is_array = true;
            }
            else if (is_array)
            {
                uint32_t count = data_buffer->ReadU32();
                num_array = count / (formatSize(format) * num_values);
            }

            // get the total amount of items and bytes
            int num_items = num_array * num_values;
            int num_bytes = num_items * formatSize(format);

            // read the bytes for this nodes data
            unsigned char bytes[num_bytes];
            if (is_array)
            {
                data_buffer->ReadBytes(num_items, bytes);
                data_buffer->SetOffset(data_buffer->GetOffset() + (num_bytes - num_items));
                data_buffer->RealignReads();
            }
            else
            {
                grabBytesAligned(data_buffer,
                                 byte_buffer,
                                 word_buffer,
                                 num_bytes,
                                 bytes);
            }

            // create the new node with the parsed values
            KML::Node *child = createNode(converter,
                                          name,
                                          format,
                                          bytes,
                                          num_bytes,
                                          num_items);
            node->AddChild(child);
            node = child;
        }
    }

    // delete the converter
    ucnv_close(converter);

    // delete the buffers
    delete word_buffer;
    delete byte_buffer;
    delete data_buffer;
    delete node_buffer;
}

KML::Document::~Document()
{
    delete root;
}

KML::NodeFormat KML::Document::formatForType(uint8_t type)
{
    switch (type)
    {
        // special types
        case 1:  return KML::NodeFormat::NodeStart;                                                   //node start

        // 1x values
        case 2:  return (KML::NodeFormat)(KML::NodeFormat::S8 | KML::NodeFormat::OneValues);          //s8
        case 3:  return (KML::NodeFormat)(KML::NodeFormat::U8 | KML::NodeFormat::OneValues);          //u8
        case 4:  return (KML::NodeFormat)(KML::NodeFormat::S16 | KML::NodeFormat::OneValues);         //s16
        case 5:  return (KML::NodeFormat)(KML::NodeFormat::U16 | KML::NodeFormat::OneValues);         //u16
        case 6:  return (KML::NodeFormat)(KML::NodeFormat::S32 | KML::NodeFormat::OneValues);         //s32
        case 7:  return (KML::NodeFormat)(KML::NodeFormat::U32 | KML::NodeFormat::OneValues);         //u32
        case 8:  return (KML::NodeFormat)(KML::NodeFormat::S64 | KML::NodeFormat::OneValues);         //s64
        case 9:  return (KML::NodeFormat)(KML::NodeFormat::U64 | KML::NodeFormat::OneValues);         //u64
        case 10: return (KML::NodeFormat)(KML::NodeFormat::Binary | KML::NodeFormat::OneValues);      //bin
        case 11: return (KML::NodeFormat)(KML::NodeFormat::String | KML::NodeFormat::OneValues);      //str
        case 12: return (KML::NodeFormat)(KML::NodeFormat::IPv4 | KML::NodeFormat::FourValues);       //ip4
        case 13: return (KML::NodeFormat)(KML::NodeFormat::Timestamp | KML::NodeFormat::OneValues);   //time
        case 14: return (KML::NodeFormat)(KML::NodeFormat::Float | KML::NodeFormat::OneValues);       //float
        case 15: return (KML::NodeFormat)(KML::NodeFormat::Double | KML::NodeFormat::OneValues);      //double

        // 2x values
        case 16: return (KML::NodeFormat)(KML::NodeFormat::S8 | KML::NodeFormat::TwoValues);          //2s8
        case 17: return (KML::NodeFormat)(KML::NodeFormat::U8 | KML::NodeFormat::TwoValues);          //2u8
        case 18: return (KML::NodeFormat)(KML::NodeFormat::S16 | KML::NodeFormat::TwoValues);         //2s16
        case 19: return (KML::NodeFormat)(KML::NodeFormat::U16 | KML::NodeFormat::TwoValues);         //2u16
        case 20: return (KML::NodeFormat)(KML::NodeFormat::S32 | KML::NodeFormat::TwoValues);         //2s32
        case 21: return (KML::NodeFormat)(KML::NodeFormat::U32 | KML::NodeFormat::TwoValues);         //2u32
        case 22: return (KML::NodeFormat)(KML::NodeFormat::S64 | KML::NodeFormat::TwoValues);         //2s64/vs64
        case 23: return (KML::NodeFormat)(KML::NodeFormat::U64 | KML::NodeFormat::TwoValues);         //2u64/vu64
        case 24: return (KML::NodeFormat)(KML::NodeFormat::Float | KML::NodeFormat::TwoValues);       //2f
        case 25: return (KML::NodeFormat)(KML::NodeFormat::Double | KML::NodeFormat::TwoValues);      //2d/vd

        // 3x values
        case 26: return (KML::NodeFormat)(KML::NodeFormat::S8 | KML::NodeFormat::ThreeValues);        //3s8
        case 27: return (KML::NodeFormat)(KML::NodeFormat::U8 | KML::NodeFormat::ThreeValues);        //3u8
        case 28: return (KML::NodeFormat)(KML::NodeFormat::S16 | KML::NodeFormat::ThreeValues);       //3s16
        case 29: return (KML::NodeFormat)(KML::NodeFormat::U16 | KML::NodeFormat::ThreeValues);       //3u16
        case 30: return (KML::NodeFormat)(KML::NodeFormat::S32 | KML::NodeFormat::ThreeValues);       //3s32
        case 31: return (KML::NodeFormat)(KML::NodeFormat::U32 | KML::NodeFormat::ThreeValues);       //3u32
        case 32: return (KML::NodeFormat)(KML::NodeFormat::S64 | KML::NodeFormat::ThreeValues);       //3s64
        case 33: return (KML::NodeFormat)(KML::NodeFormat::U64 | KML::NodeFormat::ThreeValues);       //3u64
        case 34: return (KML::NodeFormat)(KML::NodeFormat::Float | KML::NodeFormat::ThreeValues);     //3f
        case 35: return (KML::NodeFormat)(KML::NodeFormat::Double | KML::NodeFormat::ThreeValues);    //3d

        // 4x values
        case 36: return (KML::NodeFormat)(KML::NodeFormat::S8 | KML::NodeFormat::FourValues);         //4s8
        case 37: return (KML::NodeFormat)(KML::NodeFormat::U8 | KML::NodeFormat::FourValues);         //4u8
        case 38: return (KML::NodeFormat)(KML::NodeFormat::S16 | KML::NodeFormat::FourValues);        //4s16
        case 39: return (KML::NodeFormat)(KML::NodeFormat::U16 | KML::NodeFormat::FourValues);        //4u16
        case 40: return (KML::NodeFormat)(KML::NodeFormat::S32 | KML::NodeFormat::FourValues);        //4s32
        case 41: return (KML::NodeFormat)(KML::NodeFormat::U32 | KML::NodeFormat::FourValues);        //4u32
        case 42: return (KML::NodeFormat)(KML::NodeFormat::S64 | KML::NodeFormat::FourValues);        //4s64
        case 43: return (KML::NodeFormat)(KML::NodeFormat::U64 | KML::NodeFormat::FourValues);        //4u64
        case 44: return (KML::NodeFormat)(KML::NodeFormat::Float | KML::NodeFormat::FourValues);      //4f
        case 45: return (KML::NodeFormat)(KML::NodeFormat::Double | KML::NodeFormat::FourValues);     //4d

        // attribute
        case 46: return (KML::NodeFormat)(KML::NodeFormat::Attribute | KML::NodeFormat::OneValues);   //attr

        // 16x values
        case 48: return (KML::NodeFormat)(KML::NodeFormat::S8 | KML::NodeFormat::SixteenValues);      //vs8
        case 49: return (KML::NodeFormat)(KML::NodeFormat::U8 | KML::NodeFormat::SixteenValues);      //vu8

        // 8x values
        case 50: return (KML::NodeFormat)(KML::NodeFormat::S16 | KML::NodeFormat::EightValues);       //vs16
        case 51: return (KML::NodeFormat)(KML::NodeFormat::U16 | KML::NodeFormat::EightValues);       //vu16

        // boolean
        case 52: return (KML::NodeFormat)(KML::NodeFormat::Bool | KML::NodeFormat::OneValues);        //bool
        case 53: return (KML::NodeFormat)(KML::NodeFormat::Bool | KML::NodeFormat::TwoValues);        //2b
        case 54: return (KML::NodeFormat)(KML::NodeFormat::Bool | KML::NodeFormat::ThreeValues);      //3b
        case 55: return (KML::NodeFormat)(KML::NodeFormat::Bool | KML::NodeFormat::FourValues);       //4b
        case 56: return (KML::NodeFormat)(KML::NodeFormat::Bool | KML::NodeFormat::SixteenValues);    //vb

        // special types
        case 190: return KML::NodeFormat::NodeEnd;                                                    //node end
        case 191: return KML::NodeFormat::SectionEnd;                                                 //section end

        // default
        default: return (KML::NodeFormat)(0);
    }
}

int KML::Document::formatValues(KML::NodeFormat format)
{
    // dynamic values types
    if (format & KML::NodeFormat::Binary ||
        format & KML::NodeFormat::String)
        return format_values_dynamic;

         if (format & KML::NodeFormat::OneValues)     return 1;
    else if (format & KML::NodeFormat::TwoValues)     return 2;
    else if (format & KML::NodeFormat::ThreeValues)   return 3;
    else if (format & KML::NodeFormat::FourValues)    return 4;
    else if (format & KML::NodeFormat::EightValues)   return 8;
    else if (format & KML::NodeFormat::SixteenValues) return 16;
}

KML::NodeFormat KML::Document::formatRemoveValues(KML::NodeFormat format)
{
    int format_values_removed = format;
    format_values_removed &= ~KML::NodeFormat::OneValues;
    format_values_removed &= ~KML::NodeFormat::TwoValues;
    format_values_removed &= ~KML::NodeFormat::ThreeValues;
    format_values_removed &= ~KML::NodeFormat::FourValues;
    format_values_removed &= ~KML::NodeFormat::EightValues;
    format_values_removed &= ~KML::NodeFormat::SixteenValues;
    return (KML::NodeFormat)(format_values_removed);
}

size_t KML::Document::formatSize(KML::NodeFormat format)
{
    switch (formatRemoveValues(format))
    {
        case KML::NodeFormat::S8:          return sizeof(int8_t);
        case KML::NodeFormat::U8:          return sizeof(uint8_t);
        case KML::NodeFormat::S16:         return sizeof(int16_t);
        case KML::NodeFormat::U16:         return sizeof(uint16_t);
        case KML::NodeFormat::S32:         return sizeof(int32_t);
        case KML::NodeFormat::U32:         return sizeof(uint32_t);
        case KML::NodeFormat::S64:         return sizeof(int64_t);
        case KML::NodeFormat::U64:         return sizeof(uint64_t);
        case KML::NodeFormat::Binary:      return sizeof(unsigned char);
        case KML::NodeFormat::String:      return sizeof(char);
        case KML::NodeFormat::IPv4:        return sizeof(uint8_t);
        case KML::NodeFormat::Timestamp:   return sizeof(uint32_t); //todo: correct?
        case KML::NodeFormat::Float:       return sizeof(float);
        case KML::NodeFormat::Double:      return sizeof(double);
        case KML::NodeFormat::Bool:        return sizeof(signed char);
    }
}

std::string KML::Document::decodeString(UConverter *converter, unsigned char *bytes, uint8_t length)
{
    // convert the bytes into unicode
    const char *characters = (char *)bytes;
    const uint8_t unicode_bytes = length * 2;
    UChar unicode[unicode_bytes];
    UErrorCode error = U_ZERO_ERROR;
    ucnv_toUChars(converter, unicode, unicode_bytes, characters, length, &error);

    if (U_FAILURE(error))
    {
        const char *format = "Unable to convert bytes to Unicode (%i)";
        char message[strlen(format)];
        sprintf(message, format, error);
        throw new std::runtime_error(message);
    }

    // convert the unicode characters into a string
    UnicodeString unicode_string = UnicodeString(unicode);
    std::string string;
    unicode_string.toUTF8String(string);

    // return the string
    return string;
}

std::string KML::Document::grabString(UConverter *converter, ByteBuffer *source_buffer)
{
    // read the string bytes
    uint32_t length = source_buffer->ReadS32();
    unsigned char bytes[length];
    source_buffer->ReadBytes(length, bytes);
    source_buffer->RealignReads();

    // decode and return the string
    return decodeString(converter, bytes, length);
}

void KML::Document::grabBytesAligned(ByteBuffer *data_buffer,
                                     ByteBuffer *byte_buffer,
                                     ByteBuffer *word_buffer,
                                     int num_bytes,
                                     unsigned char *out)
{
    if (byte_buffer->GetOffset() % 4 == 0)
        byte_buffer->SetOffset(data_buffer->GetOffset());
    if (word_buffer->GetOffset() % 4 == 0)
        word_buffer->SetOffset(data_buffer->GetOffset());

    // get the buffer to read from
    ByteBuffer *read_buffer;
    bool realign;
    switch (num_bytes)
    {
        case 1:  read_buffer = byte_buffer; realign = false; break;
        case 2:  read_buffer = word_buffer; realign = false; break;
        default: read_buffer = data_buffer; realign = true;  break;
    }

    // read the bytes into the given output
    read_buffer->ReadBytes(num_bytes, out);
    if (realign)
        read_buffer->RealignReads();

    // get the aligned trailing offset after the read bytes
    unsigned int trailing;
    if (byte_buffer->GetOffset() > word_buffer->GetOffset())
        trailing = byte_buffer->GetOffset();
    else
        trailing = word_buffer->GetOffset();

    // realign the data buffer if it isnt already
    if (data_buffer->GetOffset() < trailing)
    {
        data_buffer->SetOffset(trailing);
        data_buffer->RealignReads();
    }
}

KML::Node *KML::Document::createNode(UConverter *converter,
                                     std::string name,
                                     KML::NodeFormat format,
                                     unsigned char *bytes,
                                     int num_bytes,
                                     int num_items)
{
    size_t item_size = formatSize(format);

    switch (formatRemoveValues(format))
    {
        case KML::NodeFormat::S8:      return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToS8);
        case KML::NodeFormat::U8:      return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToU8);
        case KML::NodeFormat::S16:     return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToS16);
        case KML::NodeFormat::U16:     return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToU16);
        case KML::NodeFormat::S32:     return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToS32);
        case KML::NodeFormat::Timestamp: //timestamps are u32s
        case KML::NodeFormat::U32:     return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToU32);
        case KML::NodeFormat::S64:     return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToS64);
        case KML::NodeFormat::U64:     return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToU64);
        case KML::NodeFormat::String:  return new KML::NodeValue<std::string>(name, decodeString(converter, bytes, num_bytes));
        case KML::NodeFormat::Float:   return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToFloat);
        case KML::NodeFormat::Double:  return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToDouble);
        case KML::NodeFormat::Bool:    return KML::Node::FromBytes(name, bytes, num_items, item_size, Utilities::BytesToBool);

        // default to an array of the given bytes
        default:
        {
            std::vector<unsigned char> values(num_bytes);
            for (int i = 0; i < num_bytes; i++)
                values[i] = bytes[i];
            return new KML::NodeArray<unsigned char>(name, values);
        }
    }
}
