#include "IFSDirectoryTexture.hpp"

#include "IFSFile.hpp"
#include "KMLDocument.hpp"

IFS::DirectoryTexture::DirectoryTexture(KML::Node *node,
                                        ByteBuffer *data_buffer,
                                        std::string name) : IFS::DirectoryMD5(node, data_buffer, name, "texturelist.xml", "texture", "image", "png")
{
    // get the texture list source
    IFS::File *texture_list_file = (IFS::File *)this->GetEntry("texturelist.xml");
    unsigned char texture_list_source[texture_list_file->GetSize()];
    texture_list_file->GetData(texture_list_source);

    // get the texture list kml
    KML::Document *texture_list = new KML::Document(texture_list_source);
    KML::Node *root = texture_list->GetRoot()->GetNode("texturelist");

    // get whether or not the textures of this directory are compressed
    bool compressed = root->GetAttribute("compress") == "avslz";
    printf("compressed:%i\n", compressed);

    // read all the textures
    for (auto t: root->GetNodes("texture"))
    {
        // get the texture attributes
        std::string directory = t->GetAttribute("name");
        std::string format = t->GetAttribute("format"); //argb8888rev, argb4444, dxt5

        // get the texture size
        KML::NodeU16Array size_node = (KML::NodeU16Array)t->GetNode("size");
        uint16_t texture_width = size_node->GetValue(0);
        uint16_t texture_height = size_node->GetValue(1);

        printf("texture directory:\"%s\" format:\"%s\" size:%u,%u\n", directory.c_str(), format.c_str(), texture_width, texture_height);

        // get the files for each image
        for (auto i: t->GetNodes("image"))
        {
            // get the current images filename
            std::string filename = i->GetAttribute("name") + ".png";

            // get the file for the current image
            IFS::File *file = (IFS::File *)this->GetEntry(filename);
            if (file == nullptr)
                continue;

            printf("%s: %p\n", filename.c_str(), file);
        }
    }

    // free the texture list kml as its now processed
    delete texture_list;
}
