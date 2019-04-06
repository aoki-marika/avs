#pragma once

#include "IFSDirectory.hpp"

namespace IFS
{
    // directory of md5 hashed files
    // used as a base class for various specialized directories
    class DirectoryMD5 : public Directory
    {
        public:
            DirectoryMD5(KML::Node *node,
                         ByteBuffer *data_buffer,
                         std::string name,
                         std::string mapping_file_name, //the name of the mapping kml file in this directory
                         std::string mapping_nodes_name, //the tag name of mapping nodes in the mapping kml
                         std::string extension = ""); //the extension to append to the plain text filenames, if any
    };
};
