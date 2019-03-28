#pragma once

#include <stdint.h>
#include <string>

#include "ByteBuffer.hpp"
#include "KMLNode.hpp"

struct UConverter;

namespace KML
{
    enum Compression
    {
        Compressed   = 0x42,
        Uncompressed = 0x45,
    };

    enum Encoding
    {
        ASCII      = 0x20,
        ISO_8859_1 = 0x40,
        EUC_JP     = 0x60,
        CP932      = 0x80,
        UTF8       = 0xa0,
    };

    class Document
    {
        private:
            // the first byte of every kml file should match this signature
            const uint8_t signature = 0xa0;

            // returned by formatValues() to indicate that the given format has a dynamic number of values
            const int format_values_dynamic = 0;

            // the root node of this document
            Node *root;

            // get the node format for the given node type
            NodeFormat formatForType(uint8_t type);

            // get the number of values for the given node format
            int formatValues(NodeFormat format);

            // remove the value count flags from the given format
            NodeFormat formatRemoveValues(NodeFormat format);

            // get the size in bytes of one of the given node format
            size_t formatSize(NodeFormat format);

            // decode the string from the given bytes with the given converter
            std::string decodeString(UConverter *converter, unsigned char *bytes, uint8_t length);

            // grab the string at the current reading offset of the given buffer
            std::string grabString(UConverter *converter, ByteBuffer *source_buffer);

            // grab the number of data bytes at the current reading offset of the given data buffer
            void grabBytesAligned(ByteBuffer *data_buffer,
                                  ByteBuffer *byte_buffer,
                                  ByteBuffer *word_buffer,
                                  int num_bytes,
                                  unsigned char *out);

            // create a node with the given name and bytes of the given format
            Node *createNode(UConverter *converter,
                             std::string name,
                             KML::NodeFormat format,
                             unsigned char *bytes,
                             int num_bytes,
                             int num_items);

        public:
            Document(const unsigned char *source);
            ~Document();

            // get the root node of this document
            Node *GetRoot()
            {
                return root;
            }
    };
};
