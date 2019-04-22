#pragma once

#include <map>
#include <string>

#include "AtlasImage.hpp"
#include "UVBuffer.hpp"

typedef unsigned int GLenum;
class Atlas
{
    private:
        // the size of this atlas base texture
        unsigned int width, height;

        // the id of this atlas base texture
        unsigned int id;

        // the texture data format/type of this atlas
        GLenum format, type;

        // the images in this atlas
        std::map<std::string, AtlasImage> images;

    public:
        // create a new atlas of the given width and height, with the given properties for the base texture
        Atlas(unsigned int width,
              unsigned int height,
              GLenum format,
              GLenum type,
              GLenum min_filter, GLenum mag_filter,
              GLenum wrap_s, GLenum wrap_t);
        ~Atlas();

        // add an image to this atlas of the given name with the given image/uv position, size, and data
        // data should be raw pixel data to upload to gles, with the same format/type passed to the ctor
        void AddImage(std::string name,
                      unsigned int image_x,
                      unsigned int image_y,
                      unsigned int image_width,
                      unsigned int image_height,
                      unsigned int uv_x,
                      unsigned int uv_y,
                      unsigned int uv_width,
                      unsigned int uv_height,
                      const void *data);

        // AddImage() but x/y/width/height are used for both image and uv rects
        void AddImage(std::string name,
                      unsigned int x,
                      unsigned int y,
                      unsigned int width,
                      unsigned int height,
                      const void *data);

        // get a pointer to the first image in this atlas matching the given name
        // returns nullptr if no matches are found
        AtlasImage *GetImage(std::string name);

        // set the given uv buffers data at the given index to the uvs of the image of the given name in this atlas
        // returns silently if no image by the given name is found in this atlas
        void SetBufferData(UVBuffer *buffer,
                           unsigned int index,
                           std::string name);

        // activate GL_TEXTURE0 and bind this atlas to GL_TEXTURE_2D
        void Bind();

        // unbind this texture from GL_TEXTURE_2D
        // should be called after Bind();
        void Unbind();
};
