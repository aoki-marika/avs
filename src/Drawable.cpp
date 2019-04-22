#include "Drawable.hpp"
#include "ShaderSource.hpp"

#include <GLES2/gl2.h>

Drawable::Drawable(std::string fragment_source)
{
    // create the shaders and program
    Shader *vertex = new Shader(ShaderType::Vertex, ShaderSource::DRAWABLE_VERTEX);
    Shader *fragment = new Shader(ShaderType::Fragment, fragment_source);
    program = new Program(vertex, fragment);

    // delete the fragment and vertex as theyre now compiled and linked
    delete fragment;
    delete vertex;

    // get uniform locations
    uniform_pv = program->GetUniform("pv");
    uniform_size = program->GetUniform("size");
    uniform_position = program->GetUniform("position");
    uniform_alpha = program->GetUniform("alpha");

    // apply defaults
    SetAlpha(1);
    SetBlendMode(BlendMode::Normal);
}

Drawable::~Drawable()
{
    delete program;
}

void Drawable::SetSize(Vector3 size)
{
    program->Use();
    program->UniformVector3(uniform_size, size);
}

void Drawable::SetPosition(Vector3 position)
{
    program->Use();
    program->UniformVector3(uniform_position, position);
}

void Drawable::SetAlpha(float alpha)
{
    program->Use();
    program->UniformFloat(uniform_alpha, alpha);
}

void Drawable::SetBlendMode(BlendMode mode)
{
    this->blend_mode = mode;
}

void Drawable::Draw(Camera *camera)
{
    // apply blend mode
    switch (blend_mode)
    {
        case BlendMode::Normal:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case BlendMode::Additive:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case BlendMode::Multiply:
            glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
            break;
    }

    // set the pv matrix
    program->Use();
    program->UniformMatrix4(uniform_pv, camera->GetMatrix());

    // draw the vertices
    DrawVertices();
}
