#pragma once

#include <assert.h>
#include <array>

#include "Vector3.hpp"

typedef unsigned int GLuint;

enum class VertexBufferUsage
{
    // the vertex buffer is meant to have its vertices assigned once
    Static,

    // the vertex buffer is meant to have its vertices assigned many times
    Dynamic,
};

class VertexBuffer
{
    private:
        static const int vertex_values = 3;
        static const int triangle_vertices = 3;
        static const int quad_vertices = triangle_vertices * 2;

        // the gl id of the buffer for this vertex buffer
        GLuint id;

        // the maximum number of vertices this buffer can hold
        int max_vertices;

        // get the size of n vertices
        int verticesSize(int n);

        // set vertices at start_index of size to the given vertex point values
        // start_index and size are in vertices, not values
        void setVertices(int start_index, int size, float *values);

    public:
        VertexBuffer(int max_vertices, VertexBufferUsage usage);
        ~VertexBuffer();

        // set the vertices of this vertex buffer from the given start index to the given vertices
        // start_index is in vertices, not values
        template<std::size_t size>
        void SetVertices(int start_index, const std::array<Vector3, size> &vertices)
        {
            // assert that start_index and size are in range of the allocated buffer
            assert(start_index >= 0 && start_index + size <= max_vertices);

            // convert the given vertices to values that can be uploaded to the buffer
            float values[size * vertex_values];
            for (int i = 0; i < size * vertex_values; i += vertex_values)
            {
                Vector3 vertex = vertices[i / vertex_values];
                values[i] = vertex.X;
                values[i + 1] = vertex.Y;
                values[i + 2] = vertex.Z;
            }

            setVertices(start_index, size, values);
        }

        // creates a static vertex buffer prefilled with a triangle from the given points (clockwise)
        // the caller is responsible for deleting the returned vertex buffer
        static VertexBuffer *Triangle(Vector3 a, Vector3 b, Vector3 c)
        {
            VertexBuffer *buffer = new VertexBuffer(triangle_vertices, VertexBufferUsage::Static);
            const std::array<Vector3, triangle_vertices> vertices =
            {
                a, b, c,
            };

            buffer->SetVertices(0, vertices);
            return buffer;
        }

        // creates a static vertex buffer prefilled with a quad from the given points (clockwise)
        // the caller is responsible for deleting the returned vertex buffer
        static VertexBuffer *Quad(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
        {
            VertexBuffer *buffer = new VertexBuffer(quad_vertices, VertexBufferUsage::Static);
            const std::array<Vector3, quad_vertices> vertices =
            {
                a, b, d,
                b, c, d,
            };

            buffer->SetVertices(0, vertices);
            return buffer;
        }

        // bind this vertex buffer to GL_ARRAY_BUFFER for usage
        void Bind();
};
