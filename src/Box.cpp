#include "Box.hpp"

#include "ShaderSource.hpp"

Box::Box() : Drawable(ShaderSource::BOX_FRAGMENT)
{
    attrib_vertex_position = GetProgram()->GetAttribute("vertexPosition");
    uniform_colour = GetProgram()->GetUniform("colour");

    // create the vertex buffer
    vertex_buffer = VertexBuffer::Quad(Vector3(1, 1, 1),
                                       Vector3(1, 2, 1),
                                       Vector3(2, 2, 1),
                                       Vector3(2, 1, 1));

    // apply defaults
    SetColour(Colour4::White());
}

Box::~Box()
{
    delete vertex_buffer;
}

void Box::SetColour(Colour4 colour)
{
    GetProgram()->Use();
    GetProgram()->UniformColour4(uniform_colour, colour);
    this->colour = colour;
}

void Box::ColourTo(Colour4 colour, double duration, Easing easing)
{
    Transformable::BeginGroup();
        ColourRTo(colour.R, duration, easing);
        ColourGTo(colour.G, duration, easing);
        ColourBTo(colour.B, duration, easing);
        ColourATo(colour.A, duration, easing);
    Transformable::EndGroup();
}

void Box::ColourRTo(float r, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->colour.R, r, easing, [this](float value)
    {
        this->SetColour(Colour4(value, this->colour.G, this->colour.B, this->colour.A));
    });
}

void Box::ColourGTo(float g, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->colour.G, g, easing, [this](float value)
    {
        this->SetColour(Colour4(this->colour.R, value, this->colour.B, this->colour.A));
    });
}

void Box::ColourBTo(float b, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->colour.B, b, easing, [this](float value)
    {
        this->SetColour(Colour4(this->colour.R, this->colour.G, value, this->colour.A));
    });
}

void Box::ColourATo(float a, double duration, Easing easing)
{
    Transformable::AddTransform(duration, &this->colour.A, a, easing, [this](float value)
    {
        this->SetColour(Colour4(this->colour.R, this->colour.G, this->colour.B, value));
    });
}

void Box::DrawVertices()
{
    // bind, draw, and unbind the vertices
    vertex_buffer->BindAttribute(attrib_vertex_position);
    vertex_buffer->DrawAll();
    vertex_buffer->UnbindAttribute(attrib_vertex_position);
}
