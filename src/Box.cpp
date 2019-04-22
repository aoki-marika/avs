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
}

void Box::DrawVertices()
{
    // bind, draw, and unbind the vertices
    vertex_buffer->BindAttribute(attrib_vertex_position);
    vertex_buffer->DrawAll();
    vertex_buffer->UnbindAttribute(attrib_vertex_position);
}
