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

void Sprite::BeginDraw()
{
    if (atlas != nullptr) atlas->Bind();
    Drawable::BeginDraw();
    uv_buffer->BindAttribute(attrib_vertex_uv);
}

void Sprite::EndDraw()
{
    uv_buffer->UnbindAttribute(attrib_vertex_uv);
    Drawable::EndDraw();
    if (atlas != nullptr) atlas->Unbind();
}
