#include "Box.hpp"

#include "ShaderSource.hpp"

Box::Box() : Drawable(ShaderSource::BOX_FRAGMENT)
{
    uniform_colour = GetProgram()->GetUniform("colour");

    // apply defaults
    SetColour(Colour4::White());
}

Box::~Box()
{
}

void Box::SetColour(Colour4 colour)
{
    GetProgram()->Use();
    GetProgram()->UniformColour4(uniform_colour, colour);
}
