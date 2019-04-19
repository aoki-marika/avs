#include "IFSTexture.hpp"

#include <iostream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

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
    width = size_node->GetValue(0);
    height = size_node->GetValue(1);

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

    // generate the atlas
    // get the gl equivalent of this textures format
    GLenum gl_format = glFormat(format);

    // get the gl data type
    GLenum gl_type = GL_UNSIGNED_BYTE;
    if (format == IFS::TextureFormat::ARGB4444)
        gl_type = GL_UNSIGNED_SHORT_4_4_4_4;

    // generate the base texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, gl_type, NULL);

    // add all the images as subimages
    for (auto i: images)
    {
        Rectangle r = i->GetAtlas();
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        r.StartX,
                        r.StartY,
                        r.GetWidth(),
                        r.GetHeight(),
                        gl_format,
                        GL_UNSIGNED_BYTE,
                        i->GetData());
    }

    // set the atlas parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter(min_filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter(mag_filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap(wrap_s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap(wrap_t));
}

IFS::Texture::~Texture()
{
    for (auto i: images)
        delete i;

    glDeleteTextures(1, &id);
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
