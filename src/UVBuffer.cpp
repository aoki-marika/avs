#include "UVBuffer.hpp"

UVBuffer::UVBuffer(unsigned int num_uvs,
                   BufferUsage usage) : Buffer(num_uv_components, num_uvs, usage)
{
}
