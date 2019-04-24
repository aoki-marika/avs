#include "Sprite.hpp"

#include "ShaderSource.hpp"
#include "VertexConstants.hpp"

Sprite::Sprite() : Drawable(ShaderSource::SPRITE_FRAGMENT)
{
    attrib_vertex_position = GetProgram()->GetAttribute("vertexPosition");
    attrib_vertex_uv = GetProgram()->GetAttribute("vertexUV");
    uniform_sampler = GetProgram()->GetUniform("sampler");
    uniform_colour = GetProgram()->GetUniform("colour");

    // create the vertex/uv buffers
    vertex_buffer = VertexBuffer::Quad(Vector3(1, 1, 1),
                                       Vector3(1, 2, 1),
                                       Vector3(2, 2, 1),
                                       Vector3(2, 1, 1));
    uv_buffer = new UVBuffer(VertexConstants::QUAD_VERTICES, BufferUsage::Static);

    // apply defaults
    SetColour(Colour4::White());
}

Sprite::~Sprite()
{
    delete vertex_buffer;
    delete uv_buffer;
}

void Sprite::SetImage(Atlas *atlas, std::string name)
{
    // pass the texture to the shader
    GetProgram()->Use();
    GetProgram()->UniformAtlas(uniform_sampler, atlas);

    // write the uv values to the uv buffer
    atlas->SetBufferData(uv_buffer, 0, name);

    // set this sprites size to match the images size
    AtlasImage *image = atlas->GetImage(name);
    if (image != nullptr)
        this->SetSize(Vector3(image->Width, image->Height, 1));

    // store the atlas to bind when drawing
    this->atlas = atlas;
}

void Sprite::SetColour(Colour4 colour)
{
    GetProgram()->Use();
    GetProgram()->UniformColour4(uniform_colour, colour);
}

void Sprite::DrawVertices()
{
    // dont bother drawing if no atlas was set
    if (atlas == nullptr)
        return;

    // bind, draw, and unbind the atlas/vertices/uvs
    atlas->Bind();
    vertex_buffer->BindAttribute(attrib_vertex_position);
    uv_buffer->BindAttribute(attrib_vertex_uv);
    vertex_buffer->DrawAll();
    uv_buffer->UnbindAttribute(attrib_vertex_uv);
    vertex_buffer->UnbindAttribute(attrib_vertex_position);
    atlas->Unbind();
}
