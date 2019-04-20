#include "Drawable.hpp"
#include "ShaderSource.hpp"

#include <GLES2/gl2.h>

Drawable::Drawable(std::string fragment_source)
{
    // drawables use a quad with position and size 1,1,1 then multiply by their actual position/size when drawing
    // this allows the vertex buffer to never have to be modified, which is a tedious task
    vertex_buffer = VertexBuffer::Quad(Vector3(1, 1, 1),
                                       Vector3(1, 2, 1),
                                       Vector3(2, 2, 1),
                                       Vector3(2, 1, 1));

    // create the shaders and program
    Shader *vertex = new Shader(ShaderType::Vertex, ShaderSource::DRAWABLE_VERTEX);
    Shader *fragment = new Shader(ShaderType::Fragment, fragment_source);
    program = new Program(vertex, fragment);

    // delete the fragment and vertex as theyre now compiled and linked
    delete fragment;
    delete vertex;

    // get attribute and uniform locations
    attrib_vertex_position = program->GetAttribute("vertexPosition");
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
    delete vertex_buffer;
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

    // draw the quad from the vertex buffer
    BeginDraw();
    vertex_buffer->DrawAll();
    EndDraw();
}

void Drawable::BeginDraw()
{
    vertex_buffer->BindAttribute(attrib_vertex_position);
}

void Drawable::EndDraw()
{
    vertex_buffer->UnbindAttribute(attrib_vertex_position);
}
