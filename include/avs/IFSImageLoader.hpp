#pragma once

#include <string>

#include "KMLNode.hpp"
#include "IFSFile.hpp"
#include "IFSTexture.hpp"
#include "Atlas.hpp"
#include "ByteUtilities.hpp"
#include "LZ77.hpp"
#include "DXT5.hpp"

namespace IFS
{
    class ImageLoader
    {
        private:
            // pad the given image data to ensure it has enough bytes for every pixel
            // if the given data has enough bytes then it is unmodified
            static void padData(unsigned char **data,
                                unsigned int data_length,
                                unsigned int width,
                                unsigned int height,
                                unsigned int bytes_per_pixel)
            {
                unsigned int minimum_length = width * height * bytes_per_pixel;
                if (data_length < minimum_length)
                    *data = (unsigned char *)realloc(*data, minimum_length);
            }

        public:
            // load the image from the given node and file with the given compression and format onto the given atlas
            // node should be an <image> node from texturelist.xml
            // file should be the images file from the tex directory
            static void Load(KML::Node *node,
                             IFS::File *file,
                             TextureCompression compression,
                             TextureFormat format,
                             Atlas *atlas)
            {
                // get the images name
                std::string name = node->GetAttribute("name");

                // get the uv and image rects
                // all rect values are multiplied by 2
                KML::NodeU16Array uvrect_node = (KML::NodeU16Array)node->GetNode("uvrect");
                unsigned int uv_x = uvrect_node->GetValue(0) / 2;
                unsigned int uv_y = uvrect_node->GetValue(2) / 2;
                unsigned int uv_width = (uvrect_node->GetValue(1) / 2) - uv_x;
                unsigned int uv_height = (uvrect_node->GetValue(3) / 2) - uv_y;

                KML::NodeU16Array imgrect_node = (KML::NodeU16Array)node->GetNode("imgrect");
                unsigned int image_x = imgrect_node->GetValue(0) / 2;
                unsigned int image_y = imgrect_node->GetValue(2) / 2;
                unsigned int image_width = (imgrect_node->GetValue(1) / 2) - image_x;
                unsigned int image_height = (imgrect_node->GetValue(3) / 2) - image_y;

                // convert the raw data to image data
                unsigned char *data;
                unsigned int data_length;
                switch (compression)
                {
                    case TextureCompression::Uncompressed:
                    {
                        data_length = file->GetSize();
                        data = new unsigned char[data_length];
                        file->GetData(data);
                        break;
                    }
                    case TextureCompression::LZ77:
                    {
                        // read the uncompressed and compressed sizes of the file
                        unsigned char header[sizeof(uint32_t) * 2];
                        file->GetData(sizeof(uint32_t) * 2, header);
                        uint32_t uncompressed_size = ByteUtilities::BytesToU32(header, 0);
                        uint32_t compressed_size = ByteUtilities::BytesToU32(header, sizeof(uint32_t));

                        // sometimes headers are missing, meaning its not actually compressed
                        // in that case the two extra u32s are moved to the end of the file
                        if (file->GetSize() == compressed_size + sizeof(uint32_t) * 2)
                        {
                            // read the compressed data
                            unsigned char compressed[compressed_size];
                            file->GetData(compressed);

                            // decompress the compressed data into data
                            data_length = uncompressed_size;
                            data = new unsigned char[uncompressed_size];
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
                            memcpy(data, raw_data + sizeof(uint32_t) * 2, data_length);
                        }

                        break;
                    }
                }

                // pad the data if needed if the format is argb8888rev/argb4444
                // decode the data if its dxt5
                switch (format)
                {
                    case TextureFormat::ARGB8888Rev:
                    {
                        padData(&data, data_length, image_width, image_height, 4);
                        break;
                    }
                    case TextureFormat::ARGB4444:
                    {
                        padData(&data, data_length, image_width, image_height, 2);
                        break;
                    }
                    case TextureFormat::DXT5:
                    {
                        // copy the dxt5 data into a new array so it can be decoded in place
                        unsigned char compressed_data[data_length];
                        memcpy(compressed_data, data, data_length);

                        // swap endianness
                        for (int i = 0; i < data_length / 2; i++)
                        {
                            unsigned char values[2] =
                            {
                                compressed_data[i * 2 + 0],
                                compressed_data[i * 2 + 1],
                            };

                            compressed_data[i * 2 + 0] = values[1];
                            compressed_data[i * 2 + 1] = values[0];
                        }

                        // decode the dxt5 data into data
                        // width * height pixels at 4 bytes per pixel (rgba)
                        data = new unsigned char[image_width * image_height * 4];
                        DXT5::Decode(image_width, image_height, compressed_data, data);
                        break;
                    }
                }

                // upload the data to the given atlas
                atlas->AddImage(name,
                                image_x,
                                image_y,
                                image_width,
                                image_height,
                                uv_x,
                                uv_y,
                                uv_width,
                                uv_height,
                                data);

                // delete the data as its now been uploaded
                delete data;
            }
    };
};
