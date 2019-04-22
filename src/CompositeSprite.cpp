#include "CompositeSprite.hpp"

#include "ShaderSource.hpp"
#include "VertexConstants.hpp"

CompositeSprite::CompositeSprite(unsigned int max_sprites,
                                 BufferUsage usage) : Drawable(ShaderSource::SPRITE_FRAGMENT)
{
    attrib_vertex_position = GetProgram()->GetAttribute("vertexPosition");
    attrib_vertex_uv = GetProgram()->GetAttribute("vertexUV");
    uniform_sampler = GetProgram()->GetUniform("sampler");

    // create the vertex/uv buffers
    unsigned int num_vertices = VertexConstants::QUAD_VERTICES * max_sprites;
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

void CompositeSprite::DrawVertices()
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
