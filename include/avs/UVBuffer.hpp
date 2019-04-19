#pragma once

#include <array>

#include "Buffer.hpp"
#include "UV.hpp"

class UVBuffer : public Buffer
{
    private:
        const unsigned int num_uv_components = 2;

    public:
        UVBuffer(unsigned int num_uvs,
                 BufferUsage usage);

        // set the uvs starting at the given index to the given uvs
        // index is in uvs, not components
        template <std::size_t size>
        void SetUVs(unsigned int index, const std::array<UV, size> *uvs)
        {
            // convert the given uv array to a float array
            float data[size * num_uv_components];
            for (int i = 0; i < size * num_uv_components; i += num_uv_components)
            {
                UV uv = (* uvs)[i / num_uv_components];
                data[i + 0] = uv.U;
                data[i + 1] = uv.V;
            }

            // set the values in the buffer
            SetData(index * num_uv_components, size * num_uv_components, data);
        }
};
