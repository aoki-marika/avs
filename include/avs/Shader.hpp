#pragma once

#include <string>

enum class ShaderType
{
    Vertex,
    Fragment,
};

class Shader
{
    private:
        unsigned int id;

    public:
        Shader(ShaderType type, std::string source);
        ~Shader();

        unsigned int GetID() const
        {
            return id;
        }
};
