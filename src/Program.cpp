#include "Program.hpp"

#include <stdexcept>
#include <GLES2/gl2.h>

Program::Program(const Shader *vertex, const Shader *fragment)
{
    // create, attach, and link the program
    id = glCreateProgram();
    glAttachShader(id, vertex->GetID());
    glAttachShader(id, fragment->GetID());
    glLinkProgram(id);

    // check for link errors
    GLint success = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    // get the log if linking failed
    if (success != GL_TRUE)
    {
        int max_length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

        char log[max_length];
        glGetProgramInfoLog(id, max_length, &max_length, log);

        if (max_length > 0)
            throw std::runtime_error(log);
        else
            throw std::runtime_error("Unknown error (did you initialize GLES?)\n");
    }
}

Program::~Program()
{
    glDeleteProgram(id);
}
