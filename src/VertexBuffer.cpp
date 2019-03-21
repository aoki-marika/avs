#include "VertexBuffer.hpp"

#include <stdlib.h>
#include <GLES2/gl2.h>

VertexBuffer::VertexBuffer(int max_vertices, VertexBufferUsage usage)
{
    GLenum gl_usage;
    switch (usage)
    {
        case VertexBufferUsage::Static:
            gl_usage = GL_STATIC_DRAW;
            break;
        case VertexBufferUsage::Dynamic:
            gl_usage = GL_DYNAMIC_DRAW;
            break;
    }

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, verticesSize(max_vertices), NULL, gl_usage);

    this->max_vertices = max_vertices;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id);
}

int VertexBuffer::verticesSize(int n)
{
    return n * vertex_values * sizeof(float);
}

void VertexBuffer::setVertices(int start_index, int size, float *values)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, verticesSize(start_index), verticesSize(size), values);
}
