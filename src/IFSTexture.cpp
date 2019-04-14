#include "IFSTexture.hpp"

#include "IFSImage.hpp"

IFS::Texture::Texture(IFS::Directory *directory,
                      KML::Node *node,
                      IFS::TextureCompression compression)
{
    // get the textures name
    name = node->GetAttribute("name");

    // get the textures format
    IFS::TextureFormat format;
    std::string format_key = node->GetAttribute("format");
         if (format_key == "argb8888rev") format = IFS::TextureFormat::ARGB8888Rev;
    else if (format_key == "argb4444")    format = IFS::TextureFormat::ARGB4444;
    else if (format_key == "dxt5")        format = IFS::TextureFormat::DXT5;

    // todo: read mag, min, and wrap attributes and pass to gles

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
