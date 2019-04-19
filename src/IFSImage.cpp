#include "IFSImage.hpp"

#include "ByteUtilities.hpp"
#include "LZ77.hpp"
#include "DXT5.hpp"

IFS::Image::Image(KML::Node *node,
                  IFS::File *file,
                  IFS::TextureCompression compression,
                  IFS::TextureFormat format)
{
    // get the images name
    name = node->GetAttribute("name");

    // get the uv and img rects
    // all rect values are multiplied by 2 and relative to the atlas
    KML::NodeU16Array uvrect_node = (KML::NodeU16Array)node->GetNode("uvrect");
    uv_rect = Rectangle(uvrect_node->GetValue(0) / 2,
                        uvrect_node->GetValue(2) / 2,
                        uvrect_node->GetValue(1) / 2,
                        uvrect_node->GetValue(3) / 2);

    KML::NodeU16Array imgrect_node = (KML::NodeU16Array)node->GetNode("imgrect");
    atlas_rect = Rectangle(imgrect_node->GetValue(0) / 2,
                           imgrect_node->GetValue(2) / 2,
                           imgrect_node->GetValue(1) / 2,
                           imgrect_node->GetValue(3) / 2);

    // convert the raw data to image data
    unsigned int data_length;
    switch (compression)
    {
        case IFS::TextureCompression::Uncompressed:
        {
            // read the raw data into the data array
            data_length = file->GetSize();
            data = new unsigned char[data_length];
            file->GetData(data);
        }
        case IFS::TextureCompression::LZ77:
        {
            // read the decompressed and compressed sizes of this images file
            unsigned char header[sizeof(uint32_t) * 2];
            file->GetData(sizeof(uint32_t) * 2, header);
            uint32_t decompressed_size = ByteUtilities::BytesToU32(header, 0);
            uint32_t compressed_size = ByteUtilities::BytesToU32(header, sizeof(uint32_t));

            // sometimes headers are missing, meaning its not actually compressed
            // the two extra u32s are moved to the end of the file
            if (file->GetSize() == compressed_size + sizeof(uint32_t) * 2)
            {
                // read the compressed data
                unsigned char compressed[compressed_size];
                file->GetData(compressed);

                // decompress the compressed data into data
                data_length = decompressed_size;
                data = new unsigned char[decompressed_size];
                LZ77::Decompress(compressed, data, compressed_size, data_length, sizeof(uint32_t) * 2);
                break;
            }
            else
            {
                // read the raw data
                unsigned char raw_data[file->GetSize()];
                file->GetData(raw_data);

                // copy the raw data without the extra header/footer u32s into data
                data_length = file->GetSize() - sizeof(uint32_t) * 4;
                data = new unsigned char[data_length];
                memcpy(data, raw_data + sizeof(uint32_t) * 2, file->GetSize() - sizeof(uint32_t) * 4);
            }
        }
    }

    // decode the image data
    decodeData(atlas_rect,
               &data,
               data_length,
               format);
}

IFS::Image::~Image()
{
    delete data;
}

void IFS::Image::padData(Rectangle rect,
                         unsigned char **data,
                         unsigned int data_length,
                         unsigned int bytes_per_pixel)
{
    // ensure the data has bytes for every pixel, and pad it with 0s if not
    unsigned int minimum_bytes = rect.GetWidth() * rect.GetHeight() * bytes_per_pixel;
    if (data_length < minimum_bytes)
        *data = (unsigned char *)realloc(*data, minimum_bytes * sizeof(unsigned char));
}

void IFS::Image::decodeData(Rectangle rect,
                            unsigned char **data,
                            unsigned int data_length,
                            IFS::TextureFormat format)
{
    unsigned int bytes_per_pixel;
    switch (format)
    {
        case IFS::TextureFormat::ARGB8888Rev:
            bytes_per_pixel = 4;
            break;
        case IFS::TextureFormat::ARGB4444:
            bytes_per_pixel = 2;
            break;
        case IFS::TextureFormat::DXT5:
            // copy the dxt5 data into a new array so it can be decoded in place
            unsigned char compressed_data[data_length];
            memcpy(compressed_data, *data, data_length * sizeof(unsigned char));

            // swap endianness
            for (int i = 0; i < data_length / 2; i++)
            {
                unsigned char values[] =
                {
                    compressed_data[i * 2 + 0],
                    compressed_data[i * 2 + 1]
                };

                compressed_data[i * 2 + 0] = values[1];
                compressed_data[i * 2 + 1] = values[0];
            }

            // decode the dxt5 data into data
            unsigned int w = atlas_rect.GetWidth(), h = atlas_rect.GetHeight();
            *data = new unsigned char[w * h * 4];
            DXT5::Decode(w, h, compressed_data, *data);
            return;
    }

    // pad the data if the given format is one that needs padding
    padData(rect, data, data_length, bytes_per_pixel);
}
