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
    this->size = size;
}

void Drawable::SetPosition(Vector3 position)
{
    program->Use();
    program->UniformVector3(uniform_position, position);
    this->position = position;
}

void Drawable::SetAlpha(float alpha)
{
    program->Use();
    program->UniformFloat(uniform_alpha, alpha);
    this->alpha = alpha;
}

void Drawable::SetBlendMode(BlendMode mode)
{
    this->blend_mode = mode;
}

void Drawable::SizeTo(Vector3 size, double duration, Easing easing)
{
    Transformable::BeginGroup();
        SizeXTo(size.X, duration, easing);
        SizeYTo(size.Y, duration, easing);
        SizeZTo(size.Z, duration, easing);
    Transformable::EndGroup();
}

void Drawable::SizeXTo(float x, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->size.X, x, easing, [this](float value)
    {
        this->SetSize(Vector3(value, this->size.Y, this->size.Z));
    });
}

void Drawable::SizeYTo(float y, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->size.Y, y, easing, [this](float value)
    {
        this->SetSize(Vector3(this->size.X, value, this->size.Z));
    });
}

void Drawable::SizeZTo(float z, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->size.Z, z, easing, [this](float value)
    {
        this->SetSize(Vector3(this->size.X, this->size.Y, value));
    });
}

void Drawable::PositionTo(Vector3 position, double duration, Easing easing)
{
    Transformable::BeginGroup();
        PositionXTo(position.X, duration, easing);
        PositionYTo(position.Y, duration, easing);
        PositionZTo(position.Z, duration, easing);
    Transformable::EndGroup();
}

void Drawable::PositionXTo(float x, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->position.X, x, easing, [this](float value)
    {
        this->SetPosition(Vector3(value, this->position.Y, this->position.Z));
    });
}

void Drawable::PositionYTo(float y, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->position.Y, y, easing, [this](float value)
    {
        this->SetPosition(Vector3(this->position.X, value, this->position.Z));
    });
}

void Drawable::PositionZTo(float z, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->position.Z, z, easing, [this](float value)
    {
        this->SetPosition(Vector3(this->position.X, this->position.Y, value));
    });
}

void Drawable::AlphaTo(float alpha, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->alpha, alpha, easing, [this](float value)
    {
        this->SetAlpha(value);
    });
}

void Drawable::Draw(Camera *camera)
{
    // update transforms
    Transformable::UpdateTransforms();

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
