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

    enum TextureFilter
    {
        Nearest,
        Linear,
    };

    enum TextureWrap
    {
        Clamp,
    };

    class Image;
    class Texture
    {
        private:
            // the attributes of this texture
            std::string name;
            TextureFormat format;
            TextureFilter min_filter, mag_filter;
            TextureWrap wrap_s, wrap_t;
            unsigned int width, height;

            // the images of this texture
            std::vector<Image *> images;

            // the id of the atlas for this texture
            unsigned int id;

            // get the respective data type for the given key
            TextureFormat formatForKey(std::string key);
            TextureFilter filterForKey(std::string key);
            TextureWrap wrapForKey(std::string key);

            // get the respective gl equivalent for the given data type
            unsigned int glFormat(TextureFormat format);
            unsigned int glFilter(TextureFilter format);
            unsigned int glWrap(TextureWrap format);

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

            // get the width/height of this textures atlas
            unsigned int GetWidth()
            {
                return width;
            }

            unsigned int GetHeight()
            {
                return height;
            }

            // get the id of this textures atlas
            unsigned int GetID()
            {
                return id;
            }
    };
};
