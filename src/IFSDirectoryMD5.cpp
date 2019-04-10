#include "IFSDirectoryMD5.hpp"

#include <stdexcept>
#include <openssl/md5.h>

#include "KMLDocument.hpp"
#include "IFSFile.hpp"

IFS::DirectoryMD5::DirectoryMD5(KML::Node *node,
                                ByteBuffer *data_buffer,
                                std::string name,
                                std::string mapping_file_name,
                                std::string mapping_directory_nodes_name,
                                std::string mapping_nodes_name,
                                std::string extension) : IFS::Directory(node, data_buffer, name)
{
    // get the mapping file
    IFS::File *mapping_file = (IFS::File *)this->GetEntry(mapping_file_name);

    // throw an exception if no mapping file was found
    if (mapping_file == nullptr)
        throw new std::runtime_error("Unable to find MD5 mapping file");

    // load the mapping kml
    unsigned char mapping_source[mapping_file->GetSize()];
    mapping_file->GetData(mapping_source);
    KML::Document *mapping = new KML::Document(mapping_source);

    // get the true root node
    KML::Node *root = mapping->GetRoot()->GetChild(0);

    // map the md5 hashed names to the plain text names
    for (auto d: root->GetNodes(mapping_directory_nodes_name))
    {
        for (auto m: d->GetNodes(mapping_nodes_name))
        {
            // encode the name attribute to get the md5 hash of it
            std::string name = m->GetAttribute("name");
            const int num_encoded_name_bytes = mapping->GetConverter()->GetBufferSize(name);
            unsigned char encoded_name_bytes[num_encoded_name_bytes];
            unsigned int encoded_name_length = mapping->GetConverter()->Encode(name, encoded_name_bytes);

            // get the md5 hash of the encoded name attribute
            unsigned char md5[MD5_DIGEST_LENGTH];
            MD5(encoded_name_bytes, encoded_name_length, md5);

            // get the string of the md5 hash
            char md5_characters[MD5_DIGEST_LENGTH * 2 + 1];
            for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
                sprintf(md5_characters + i * 2, "%02x", md5[i]);
            md5_characters[MD5_DIGEST_LENGTH * 2] = '\0';
            std::string md5_string = std::string(md5_characters);

            // find the matching entry for the md5 hash and set its name to the plain text name
            for (auto e: this->GetEntries())
            {
                IFS::File *f = (IFS::File *)e;

                // skip if the current entry is not a file
                // only files are md5 hashed
                if (f == nullptr)
                    continue;

                if (f->Name == md5_string)
                {
                    f->Name = name;
                    if (extension != "")
                        f->Name += "." + extension;
                    break;
                }
            }
        }
    }

    // free the mapping kml now that its been applied
    delete mapping;
}
