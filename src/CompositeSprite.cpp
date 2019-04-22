#include "CompositeSprite.hpp"

#include "ShaderSource.hpp"

CompositeSprite::CompositeSprite(unsigned int max_sprites,
                                 BufferUsage usage) : Drawable(ShaderSource::SPRITE_FRAGMENT)
{
    attrib_vertex_position = GetProgram()->GetAttribute("vertexPosition");
    attrib_vertex_uv = GetProgram()->GetAttribute("vertexUV");
    uniform_sampler = GetProgram()->GetUniform("sampler");

    // create the vertex/uv buffers
    // 6 uvs for 6 vertices on a quad
    unsigned int num_vertices = 6 * max_sprites;
    vertex_buffer = new VertexBuffer(num_vertices, usage);
    uv_buffer = new UVBuffer(num_vertices, usage);

    // set the default size so sprite vertices arent affected
    this->SetSize(Vector3(1, 1, 1));
}

CompositeSprite::~CompositeSprite()
{
    delete vertex_buffer;
    delete uv_buffer;
}

void CompositeSprite::SetAtlas(Atlas *atlas)
{
    // pass the texture to the shader
    GetProgram()->Use();
    GetProgram()->UniformAtlas(uniform_sampler, atlas);

    // store the atlas to bind when drawing
    this->atlas = atlas;
}

// note: CompositeSprite does not call any of the base Drawable draw methods as it draws its own vertices
// todo: remove this note when Drawable removes its VertexBuffer

void CompositeSprite::BeginDraw()
{
    if (atlas != nullptr) atlas->Bind();
    vertex_buffer->BindAttribute(attrib_vertex_position);
    uv_buffer->BindAttribute(attrib_vertex_uv);
}

void CompositeSprite::EndDraw()
{
    uv_buffer->UnbindAttribute(attrib_vertex_uv);
    vertex_buffer->UnbindAttribute(attrib_vertex_position);
    if (atlas != nullptr) atlas->Unbind();
}

void CompositeSprite::DrawVertices()
{
    vertex_buffer->DrawAll();
}
