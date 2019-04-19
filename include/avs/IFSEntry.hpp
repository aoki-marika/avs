#pragma once

#include "KMLNode.hpp"

namespace IFS
{
    class Entry
    {
        public:
            // the name of this node
            std::string Name;

            // create an ifs filesystem entry from the given kml node with the given name
            Entry(KML::Node *node, std::string name);
            ~Entry();
    };
};
