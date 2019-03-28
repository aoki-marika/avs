#include <stdlib.h>
#include <avs/VertexBuffer.hpp>
#include <avs/Vector3.hpp>

int main()
{
    // dynamic buffer
    VertexBuffer *buffer = new VertexBuffer(3, VertexBufferUsage::Dynamic);
    const std::array<Vector3, 3> vertices =
    {
        Vector3(-1, -1, 0),
        Vector3( 0,  1, 0),
        Vector3( 1, -1, 0),
    };

    buffer->SetVertices(0, vertices);
    delete buffer;

    // triangle
    VertexBuffer *triangle = VertexBuffer::Triangle(Vector3(-1, -1, 0),
                                                    Vector3( 0,  1, 0),
                                                    Vector3( 1, -1, 0));
    delete triangle;

    // quad
    VertexBuffer *quad = VertexBuffer::Quad(Vector3(-1, -1, 0),
                                            Vector3(-1,  1, 0),
                                            Vector3( 1,  1, 0),
                                            Vector3( 1, -1, 0));
    delete quad;

    return EXIT_SUCCESS;
}
