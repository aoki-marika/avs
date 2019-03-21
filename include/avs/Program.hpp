#pragma once

#include "Shader.hpp"
#include "Matrix4.hpp"

class Program
{
    private:
        unsigned int id;

    public:
        Program(const Shader *vertex, const Shader *fragment);
        ~Program();

        // get the id of the uniform/attribute by the given name from this program
        unsigned int GetUniform(std::string name);
        unsigned int GetAttribute(std::string name);

        // use this program for drawing vertices/fragments
        void Use();

        // set uniform values to various data types
        // Use must be called before uniforms can be set
        void UniformVector3(unsigned int uniform, Vector3 value);
        void UniformMatrix4(unsigned int uniform, Matrix4 value);
};
