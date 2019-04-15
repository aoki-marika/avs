#pragma once

#include "Rectangle.hpp"
#include "KMLNode.hpp"
#include "IFSFile.hpp"
#include "IFSTexture.hpp"

namespace IFS
{
    class Image
    {
        private:
            // the name of this image
            std::string name;

            // the atlas and uv rects of this image
            Rectangle uv_rect, atlas_rect;

            // the uncompressed, decoded data of this image
            unsigned char *data;

            // pad the given image data to ensure it has enough bytes for all of its pixels
            void padData(Rectangle rect,
                         unsigned char **data,
                         unsigned int data_length,
                         unsigned int bytes_per_pixel);

            // decode the given image data with the given rect and format
            void decodeData(Rectangle rect,
                            unsigned char **data,
                            unsigned int data_length,
                            TextureFormat format);

        public:
            // create a new image from the given node and file with the given compression and format
            // node should be a <image> node from a texturelist.xml
            Image(KML::Node *node,
                  File *file,
                  TextureCompression compression,
                  TextureFormat format);
            ~Image();

            // get the name of this image
            std::string GetName()
            {
                return name;
            }

            // get the uv rectangle of this image
            Rectangle GetUV()
            {
                return uv_rect;
            }

            // get the atlas rectangle of this image
            Rectangle GetAtlas()
            {
                return atlas_rect;
            }

            // get the data array of this image
            unsigned char *GetData()
            {
                return data;
            }
    };
};
