#include "Atlas.hpp"

#include <GLES2/gl2.h>

Atlas::Atlas(unsigned int width,
             unsigned int height,
             GLenum format,
             GLenum type,
             GLenum min_filter, GLenum mag_filter,
             GLenum wrap_s, GLenum wrap_t) : width(width), height(height), format(format), type(type)
{
    // generate the base texture for subimages to be placed onto
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, NULL);

    // set the atlas parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
}

Atlas::~Atlas()
{
    glDeleteTextures(1, &id);
}

void Atlas::AddImage(std::string name,
                     unsigned int x,
                     unsigned int y,
                     unsigned int width,
                     unsigned int height,
                     const unsigned char *data)
{
    // add the image to the base texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, type, data);

    // add the images to this atlas images
    float sx = x / this->width, ex = (x + width) / this->height;
    float sy = y / this->height, ey = (y + height) / this->height;
    images[name] =
    {
        .Width = width,
        .Height = height,
        .A = UV(sx, sy),
        .B = UV(sx, ey),
        .C = UV(ex, ey),
        .D = UV(ex, sy),
    };
}

void Atlas::GetImageSize(std::string name,
                         unsigned int *width,
                         unsigned int *height)
{
    // return early if the given uv name is not in this atlas
    if (images.find(name) == images.end())
        return;

    // write the width/height to the given pointers
    AtlasImage i = images[name];
    *width = i.Width;
    *height = i.Height;
}

void Atlas::SetBufferData(UVBuffer *buffer,
                          unsigned int index,
                          std::string name)
{
    // return early if the given uv name is not in this atlas
    if (images.find(name) == images.end())
        return;

    // write the uvs to the given buffer
    // note: these are mapped to the vertex layout of VertexBuffer::Quad
    // if that changes this will need to change too
    AtlasImage i = images[name];
    const std::array<UV, 6> data =
    {
        i.A, i.B, i.D,
        i.B, i.C, i.D,
    };

    buffer->SetUVs(index, &data);
}

void Atlas::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Atlas::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}