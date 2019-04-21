#include "IFSTexture.hpp"

#include <iostream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "IFSImage.hpp"

IFS::Texture::Texture(IFS::Directory *directory,
                      KML::Node *node,
                      IFS::TextureCompression compression)
{
    // get this textures attributes
    name = node->GetAttribute("name");
    TextureFormat format = formatForKey(node->GetAttribute("format"));
    TextureFilter min_filter = filterForKey(node->GetAttribute("min_filter"));
    TextureFilter mag_filter = filterForKey(node->GetAttribute("mag_filter"));
    TextureWrap wrap_s = wrapForKey(node->GetAttribute("wrap_s"));
    TextureWrap wrap_t = wrapForKey(node->GetAttribute("wrap_t"));

    // generate the atlas
    KML::NodeU16Array size_node = (KML::NodeU16Array)node->GetNode("size");
    atlas = new Atlas(size_node->GetValue(0),
                      size_node->GetValue(1),
                      glFormat(format),
                      (format == IFS::TextureFormat::ARGB4444) ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_BYTE,
                      glFilter(min_filter),
                      glFilter(mag_filter),
                      glWrap(wrap_s),
                      glWrap(wrap_t));

    // read all the images
    for (auto i: node->GetNodes("image"))
    {
        // get the file for the current image
        IFS::File *file = (IFS::File *)directory->GetEntry(i->GetAttribute("name"));
        if (file == nullptr)
            continue;

        // create the image, add it to the atlas, and delete it
        IFS::Image *image = new IFS::Image(i, file, compression, format);
        image->AddToAtlas(atlas);
        delete image;
    }
}

IFS::Texture::~Texture()
{
    delete atlas;
}

IFS::TextureFormat IFS::Texture::formatForKey(std::string key)
{
    if (key == "argb8888rev")
        return IFS::TextureFormat::ARGB8888Rev;
    else if (key == "argb4444")
        return IFS::TextureFormat::ARGB4444;
    else if (key == "dxt5")
        return IFS::TextureFormat::DXT5;
    else
        std::cerr << "IFS: Unknown texture format \"" << key << "\"" << std::endl;
}

IFS::TextureFilter IFS::Texture::filterForKey(std::string key)
{
    if (key == "nearest")
        return IFS::TextureFilter::Nearest;
    else if (key == "linear")
        return IFS::TextureFilter::Linear;
    else
        std::cerr << "IFS: Unknown texture filter \"" << key << "\"" << std::endl;
}

IFS::TextureWrap IFS::Texture::wrapForKey(std::string key)
{
    if (key == "clamp")
        return IFS::TextureWrap::Clamp;
    else
        std::cerr << "IFS: Unknown texture wrap \"" << key << "\"" << std::endl;
}

unsigned int IFS::Texture::glFormat(IFS::TextureFormat format)
{
    switch (format)
    {
        case IFS::TextureFormat::ARGB8888Rev: return GL_BGRA_EXT;
        case IFS::TextureFormat::ARGB4444:    return GL_BGRA_EXT;
        case IFS::TextureFormat::DXT5:        return GL_RGBA;
    }
}

unsigned int IFS::Texture::glFilter(IFS::TextureFilter filter)
{
    switch (filter)
    {
        case IFS::TextureFilter::Nearest: return GL_NEAREST;
        case IFS::TextureFilter::Linear:  return GL_LINEAR;
    }
}

unsigned int IFS::Texture::glWrap(IFS::TextureWrap wrap)
{
    switch (wrap)
    {
        case IFS::TextureWrap::Clamp: return GL_CLAMP_TO_EDGE;
    }
}
