#pragma once

#include "Atlas.hpp"
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

            // the atlas of this textures images
            Atlas *atlas;

            // get the respective data type for the given key
            TextureFormat formatForKey(std::string key);
            unsigned int filterForKey(std::string key);
            unsigned int wrapForKey(std::string key);

            // get the respective gl format for the given texture format
            unsigned int glFormat(TextureFormat format);

        public:
            // create a new texture from the given directory and node with the given compression
            // directory should be the texture directory containing the image files
            // node should be a <texture> node from a texturelist.xml
            Texture(Directory *directory,
                    KML::Node *node,
                    TextureCompression compression);
            ~Texture();

            // get the name of this texture
            std::string GetName()
            {
                return name;
            }

            // get a pointer to the atlas of this texture
            Atlas *GetAtlas()
            {
                return atlas;
            }
    };
};
