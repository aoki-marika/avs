#pragma once

#include <vector>

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
            std::vector<unsigned char> data;

            // pad the given image data to ensure it has enough bytes for all of its pixels
            void padData(Rectangle rect,
                         std::vector<unsigned char> *data,
                         unsigned int bytes_per_pixel);

            // decode the given image data with the given rect and format
            void decodeData(Rectangle rect,
                            std::vector<unsigned char> *data,
                            TextureFormat format);

        public:
            // create a new image from the given node and file with the given compression and format
            // node should be a <image> node from a texturelist.xml
            Image(KML::Node *node,
                  File *file,
                  TextureCompression compression,
                  TextureFormat format);

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

            // get a pointer to the data vector of this image
            std::vector<unsigned char> *GetData()
            {
                return &data;
            }
    };
};
