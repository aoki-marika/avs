#include "Sprite.hpp"

#include "ShaderSource.hpp"

Sprite::Sprite() : Drawable(ShaderSource::SPRITE_FRAGMENT)
{
    attrib_vertex_uv = GetProgram()->GetAttribute("vertexUV");
    uniform_sampler = GetProgram()->GetUniform("sampler");

    // create the uv buffer
    // 6 uvs for 6 vertices on a quad
    uv_buffer = new UVBuffer(6, BufferUsage::Static);
}

Sprite::~Sprite()
{
    delete uv_buffer;
}

void Sprite::SetImage(Atlas *atlas, std::string name)
{
    // write the uv values to the uv buffer
    atlas->SetBufferData(uv_buffer, 0, name);

    // set this sprites size to match the images size
    unsigned int width, height;
    atlas->GetImageSize(name, &width, &height);
    this->SetSize(Vector3(width, height, 1));
}

void Sprite::BeginDraw()
{
    Drawable::BeginDraw();
    uv_buffer->BindAttribute(attrib_vertex_uv);
}

void Sprite::EndDraw()
{
    uv_buffer->UnbindAttribute(attrib_vertex_uv);
    Drawable::EndDraw();
}
