#include "Shader.hpp"

#include <stdexcept>
#include <GLES2/gl2.h>

Shader::Shader(ShaderType type, std::string source)
{
    GLenum gl_type;
    switch (type)
    {
        case ShaderType::Vertex:
            gl_type = GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            gl_type = GL_FRAGMENT_SHADER;
            break;
    }

    // create and compile the shader
    id = glCreateShader(gl_type);
    const GLchar *gl_source = (GLchar *)source.c_str();
    glShaderSource(id, 1, &gl_source, NULL);
    glCompileShader(id);

    // check for compilation errors
    GLint success = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    // get the log if compilation failed
    if (success != GL_TRUE)
    {
        int max_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

        char log[max_length];
        glGetShaderInfoLog(id, max_length, &max_length, log);

        if (max_length > 0)
            throw std::runtime_error(log);
        else
            throw std::runtime_error("Unknown error (did you initialize GLES?)\n");
    }
}

Shader::~Shader()
{
    glDeleteShader(id);
}
