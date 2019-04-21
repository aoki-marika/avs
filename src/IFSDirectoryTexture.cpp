#include "IFSDirectoryTexture.hpp"

#include <openssl/md5.h>

#include "IFSFile.hpp"
#include "KMLDocument.hpp"

IFS::DirectoryTexture::DirectoryTexture(KML::Node *node,
                                        ByteBuffer *data_buffer,
                                        std::string name) : IFS::Directory(node, data_buffer, name)
{
    // get the texture list source
    IFS::File *texture_list_file = (IFS::File *)this->GetEntry("texturelist.xml");
    unsigned char texture_list_source[texture_list_file->GetSize()];
    texture_list_file->GetData(texture_list_source);

    // get the texture list kml
    KML::Document *texture_list = new KML::Document(texture_list_source);
    KML::Node *root = texture_list->GetRoot()->GetNode("texturelist");

    // get the compression format of this texture
    IFS::TextureCompression compression;
    if (root->GetAttribute("compress") == "avslz")
        compression = IFS::TextureCompression::LZ77;
    else
        compression = IFS::TextureCompression::Uncompressed;

    // read all the textures
    for (auto t: root->GetNodes("texture"))
    {
        // update all the md5 hashed files to use the plaintext names
        for (auto i: t->GetNodes("image"))
        {
            // encode the name of the current image
            std::string name = i->GetAttribute("name");
            const int num_encoded_name_bytes = texture_list->GetConverter()->GetBufferSize(name);
            unsigned char encoded_name_bytes[num_encoded_name_bytes];
            unsigned int encoded_name_length = texture_list->GetConverter()->Encode(name, encoded_name_bytes);

            // get the md5 hash of the encoded name
            unsigned char md5[MD5_DIGEST_LENGTH];
            MD5(encoded_name_bytes, encoded_name_length, md5);

            // get the string of the md5 hash
            char md5_characters[MD5_DIGEST_LENGTH * 2 + 1];
            for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
                sprintf(md5_characters + i * 2, "%02x", md5[i]);
            md5_characters[MD5_DIGEST_LENGTH * 2] = '\0';
            std::string md5_string = std::string(md5_characters);

            // update the hashed file to use the plaintext name
            IFS::File *file = (IFS::File *)this->GetEntry(md5_string);
            if (file != nullptr)
                file->Name = name;
        }

        // create the texture
        textures.push_back(new IFS::Texture(this, t, compression));
    }

    // remove all the child entries as theyve now been mapped to textures
    RemoveEntries();

    // free the texture list kml as its now processed
    delete texture_list;
}

IFS::DirectoryTexture::~DirectoryTexture()
{
    for (auto t: textures)
        delete t;
}

Atlas *IFS::DirectoryTexture::GetAtlas(std::string name)
{
    for (auto t: textures)
        if (t->GetName() == name)
            return t->GetAtlas();

    return nullptr;
}
