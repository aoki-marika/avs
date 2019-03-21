#pragma once

#include "Shader.hpp"

class Program
{
    private:
        unsigned int id;

    public:
        Program(const Shader *vertex, const Shader *fragment);
        ~Program();
};
