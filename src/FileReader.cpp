#include "FileReader.hpp"

FileReader::FileReader(std::string path)
{
    stream.open(path, std::ios::binary);
}

FileReader::~FileReader()
{
    stream.close();
}

void FileReader::ReadBytes(unsigned int offset,
                           unsigned int num_bytes,
                           unsigned char *out)
{
    stream.seekg(offset, std::ios::beg);
    stream.read((char *)out, num_bytes);
}
