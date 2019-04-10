#pragma once

#include <string>
#include <fstream>

#include "Reader.hpp"

class FileReader : public Reader
{
    private:
        // the file stream for this reader to read from
        std::ifstream stream;

    public:
        // create a file reader for a file at the given path
        FileReader(std::string path);
        ~FileReader();

        void ReadBytes(unsigned int offset,
                       unsigned int num_bytes,
                       unsigned char *out);
};
