#include "IFSImage.hpp"

#include <assert.h>

#include "ByteUtilities.hpp"
#include "LZ77.hpp"

IFS::Image::Image(KML::Node *node,
                  IFS::File *file,
                  IFS::TextureCompression compression,
                  IFS::TextureFormat format)
{
    // get the images name
    name = node->GetAttribute("name");

    // get the uv and img rects
    // all rect values are divided by 2 and relative to the atlas
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

    // read the raw data
    unsigned char raw_data[file->GetSize()];
    file->GetData(raw_data);

    // convert the raw data to image data
    switch (compression)
    {
        case IFS::TextureCompression::Uncompressed:
        {
            // copy the raw data into the data vector
            data = std::vector<unsigned char>(file->GetSize());
            for (int i = 0; i < file->GetSize(); i++)
                data[i] = raw_data[i];
            break;
        }
        case IFS::TextureCompression::LZ77:
        {
            // read the decompressed and compressed sizes
            uint32_t decompressed_size = ByteUtilities::BytesToU32(raw_data, 0);
            uint32_t compressed_size = ByteUtilities::BytesToU32(raw_data, sizeof(uint32_t));

            // sometimes headers are missing, meaning its not actually compressed
            // the two extra u32s are moved to the end of the file
            if (file->GetSize() == compressed_size + sizeof(uint32_t) * 2)
            {
                // decompress the raw data
                std::vector<unsigned char> decompressed = LZ77::Decompress(raw_data, file->GetSize(), sizeof(uint32_t) * 2);
                assert(decompressed.size() == decompressed_size);

                // pass the image data back
                data = decompressed;
                break;
            }
            else
            {
                for (int i = sizeof(uint32_t) * 2; i < file->GetSize() - sizeof(uint32_t) * 2; i++)
                    data.push_back(raw_data[i]);
                break;
            }
        }
    }

    // decode the image data
    decodeData(atlas_rect,
               &data,
               format);
}

void IFS::Image::padData(Rectangle rect,
                         std::vector<unsigned char> *data,
                         unsigned int bytes_per_pixel)
{
    // ensure the data has bytes for every pixel, and pad it with 0s if not
    unsigned int num_bytes = rect.GetWidth() * rect.GetHeight() * bytes_per_pixel;
    if (data->size() < num_bytes)
        data->insert(data->end(), num_bytes - data->size(), 0x00);
}

void IFS::Image::decodeData(Rectangle rect,
                            std::vector<unsigned char> *data,
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
            // todo: dxt5 support?
            // its a lot more complicated than the other formats and may not be needed for the assets this project uses
            // it is definitely still in use, but oddly only seems to be on "tex001"
            return;
    }

    // pad the data if the given format is one that needs padding
    padData(rect, data, bytes_per_pixel);
}
