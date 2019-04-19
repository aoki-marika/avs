#pragma once

#include <array>

#include "Buffer.hpp"
#include "Vector3.hpp"

class VertexBuffer : public Buffer
{
    private:
        static const unsigned int num_vertex_components = 3; //x, y, z
        static const unsigned int num_quad_vertices = 2 * 3; //two triangles

        // the number of vertices this buffer is allocated to hold
        unsigned int num_vertices;

    protected:
        // set the vertices starting at the given index to the given vertices
        // index is in vertices, not values
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
        VertexBuffer(unsigned int num_vertices,
                     BufferUsage usage);

        // creates a static vertex buffer prefilled with a quad from the given points
        // the caller is responsible for deleting the returned vertex buffer
        // the points are intended to be in clockwise order (typically top left, top right, bottom right, bottom left)
        static VertexBuffer *Quad(Vector3 a,
                                  Vector3 b,
                                  Vector3 c,
                                  Vector3 d)
        {
            VertexBuffer *buffer = new VertexBuffer(num_quad_vertices, BufferUsage::Static);
            const std::array<Vector3, num_quad_vertices> vertices =
            {
                a, b, d,
                b, c, d,
            };

            buffer->SetVertices(0, &vertices);
            return buffer;
        }

        // draw all the vertices in this buffer
        // should call BindAttribute() first to bind vertices to the vertex shader
        void DrawAll();
};
