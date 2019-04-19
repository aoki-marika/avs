#include "VertexBuffer.hpp"

#include <GLES2/gl2.h>

VertexBuffer::VertexBuffer(unsigned int num_vertices,
                           BufferUsage usage) : Buffer(num_vertex_components, num_vertices, usage), num_vertices(num_vertices)
{
}

void VertexBuffer::DrawAll()
{
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}
