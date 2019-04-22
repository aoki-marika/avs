#pragma once

#include <array>

#include "Buffer.hpp"
#include "Vector3.hpp"
#include "VertexConstants.hpp"

class VertexBuffer : public Buffer
{
    private:
        // the number of components in a vertex (x, y, z)
        static const unsigned int num_vertex_components = 3;

        // the number of vertices this buffer is allocated to hold
        unsigned int num_vertices;

    protected:
        // set the vertices starting at the given index to the given vertices
        // index is in vertices, not components
        template <std::size_t size>
        void SetVertices(unsigned int index, const std::array<Vector3, size> *vertices)
        {
            // convert the given vector3 array to a float array
            float data[size * num_vertex_components];
            for (int i = 0; i < size * num_vertex_components; i += num_vertex_components)
            {
                Vector3 v = (* vertices)[i / num_vertex_components];
                data[i + 0] = v.X;
                data[i + 1] = v.Y;
                data[i + 2] = v.Z;
            }

            // set the values in the buffer
            SetData(index * num_vertex_components, size * num_vertex_components, data);
        }

    public:
        // create a new vertex buffer with the given number of vertices and usage
        VertexBuffer(unsigned int num_vertices,
                     BufferUsage usage);

        // creates a static vertex buffer prefilled with a quad from the given points
        // the caller is responsible for deleting the returned vertex buffer
        // see SetQuad() for details on the point parameters
        static VertexBuffer *Quad(Vector3 a,
                                  Vector3 b,
                                  Vector3 c,
                                  Vector3 d)
        {
            VertexBuffer *buffer = new VertexBuffer(VertexConstants::QUAD_VERTICES, BufferUsage::Static);
            buffer->SetQuad(0, a, b, c, d);

            return buffer;
        }

        // set the vertices starting at the given index to a quad with the given points
        // the points are intended to be in clockwise order (typically top left, top right, bottom right, bottom left)
        // index is in vertices, not components
        void SetQuad(unsigned int index,
                     Vector3 a,
                     Vector3 b,
                     Vector3 c,
                     Vector3 d);

        // draw all the vertices in this buffer
        // should call BindAttribute() first to bind vertices to the vertex shader
        void DrawAll();
};
