#pragma once

#include <vector>

#include "KMLNode.hpp"
#include "IFSDirectory.hpp"

namespace IFS
{
    enum TextureCompression
    {
        Uncompressed,
        LZ77,
    };

    enum TextureFormat
    {
        ARGB8888Rev,
        ARGB4444,
        DXT5,
    };

    class Image;
    class Texture
    {
        private:
            // the name of this texture
            std::string name;

            // the images of this texture
            std::vector<Image *> images;

        public:
            // create a new texture from the given directory and node with the given compression
            // directory should be the texture directory containing the image files
            // node should be a <texture> node from a texturelist.xml
            Texture(Directory *directory,
                    KML::Node *node,
                    TextureCompression compression);
            ~Texture();

            // get a pointer to the first image of this texture matching the given name
            // returns nullptr if none is found
            Image *GetImage(std::string name);

            // get the name of this texture
            std::string GetName()
            {
                return name;
            }
    };
};
