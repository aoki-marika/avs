#pragma once

#include <string>

#include "IFSEntry.hpp"
#include "ByteBuffer.hpp"

namespace IFS
{
    class Directory : public Entry
    {
        private:
            // the child entries of this directory
            std::vector<Entry *> children;

            // replace all instances of match in string with replacement
            std::string stringReplaceAll(std::string string, std::string match, std::string replacement);

        public:
            // create a directory filesystem entry from the given kml node with the given name
            // data_buffer is the ByteBuffer that is passed to child files to read data from
            Directory(KML::Node *node,
                      ByteBuffer *data_buffer,
                      std::string name);
            ~Directory();

            // get a reference to the first child entry of this directory matching the given name, if any
            // returns nullptr if no matches are found
            Entry *GetEntry(std::string name);

            // get a copy of the vector of pointers to this directories child entries
            std::vector<Entry *> GetEntries()
            {
                return children;
            }
    };
};
