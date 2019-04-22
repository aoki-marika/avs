#pragma once

#include "Shader.hpp"
#include "Matrix4.hpp"
#include "Colour4.hpp"
#include "Atlas.hpp"

class Program
{
    private:
        unsigned int id;

    public:
        Program(const Shader *vertex, const Shader *fragment);
        ~Program();

        // get the id of the attribute/uniform by the given name from this program
        int GetAttribute(std::string name);
        int GetUniform(std::string name);

        // use this program for drawing vertices/fragments or assigning attributes/uniforms
        void Use();

        // set uniform values to various data types
        // Use() must be called before uniforms can be set
        void UniformFloat(int uniform, float value);
        void UniformVector3(int uniform, Vector3 value);
        void UniformColour4(int uniform, Colour4 value);
        void UniformMatrix4(int uniform, Matrix4 value);
        void UniformAtlas(int uniform, Atlas *atlas);
};
