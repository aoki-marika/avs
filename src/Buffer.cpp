#include "Buffer.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>

Buffer::Buffer(unsigned int num_value_components,
               unsigned int num_values,
               BufferUsage usage) : num_value_components(num_value_components), num_values(num_values)
{
    // get the gl equivalent of the given usage
    GLenum gl_usage;
    switch (usage)
    {
        case BufferUsage::Static:  gl_usage = GL_STATIC_DRAW;  break;
        case BufferUsage::Dynamic: gl_usage = GL_DYNAMIC_DRAW; break;
    }

    // generate the buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER,  num_value_components * sizeof(float) * num_values, NULL, gl_usage);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &id);
}

void Buffer::BindAttribute(unsigned int attribute)
{
    glEnableVertexAttribArray(attribute);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glVertexAttribPointer(attribute, num_value_components, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Buffer::UnbindAttribute(unsigned int attribute)
{
    glDisableVertexAttribArray(attribute);
}

void Buffer::SetData(unsigned int index,
                     unsigned int size,
                     const float *data)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(float), size * sizeof(float), data);
}
