#include <stdlib.h>
#include <avs/VertexBuffer.hpp>

int main()
{
    // quad
    VertexBuffer *quad = VertexBuffer::Quad(Vector3(-1, -1, 0),
                                            Vector3(-1,  1, 0),
                                            Vector3( 1,  1, 0),
                                            Vector3( 1, -1, 0));
    delete quad;

    return EXIT_SUCCESS;
}
