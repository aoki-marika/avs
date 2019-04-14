#include "IFSTexture.hpp"

#include <iostream>

#include "IFSImage.hpp"

IFS::Texture::Texture(IFS::Directory *directory,
                      KML::Node *node,
                      IFS::TextureCompression compression)
{
    // get the textures attributes
    name = node->GetAttribute("name");
    format = formatForKey(node->GetAttribute("format"));
    min_filter = filterForKey(node->GetAttribute("min_filter"));
    mag_filter = filterForKey(node->GetAttribute("mag_filter"));
    wrap_s = wrapForKey(node->GetAttribute("wrap_s"));
    wrap_t = wrapForKey(node->GetAttribute("wrap_t"));

    // get the textures size
    KML::NodeU16Array size_node = (KML::NodeU16Array)node->GetNode("size");
    uint16_t width = size_node->GetValue(0);
    uint16_t height = size_node->GetValue(1);

    // read all the images
    for (auto i: node->GetNodes("image"))
    {
        // get the file for the current image
        IFS::File *file = (IFS::File *)directory->GetEntry(i->GetAttribute("name"));
        if (file == nullptr)
            continue;

        // create the image
        images.push_back(new IFS::Image(i, file, compression, format));
    }
}

IFS::Texture::~Texture()
{
    for (auto i: images)
        delete i;
}

IFS::Image *IFS::Texture::GetImage(std::string name)
{
    for (auto i: images)
        if (i->GetName() == name)
            return i;

    return nullptr;
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
