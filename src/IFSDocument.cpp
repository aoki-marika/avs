#include "IFSDocument.hpp"

#include "ByteBuffer.hpp"
#include "KMLDocument.hpp"
#include "IFSDirectory.hpp"

IFS::Document::Document(std::string path)
{
    // create the buffer for reading the header
    ByteBuffer *header_buffer = new ByteBuffer(path);

    // ensure the given source matches the ifs signature
    if (header_buffer->ReadU32() != signature)
        throw new std::runtime_error("Given source is not of an IFS file (invalid signature)");

    // read the version
    uint16_t version = header_buffer->ReadU16();

    // the next u16 should equal NOT(version)
    if ((header_buffer->ReadU16() ^ version) != 0xffff)
        throw std::runtime_error("Given source is not of a valid IFS file (NOT(version) incorrect)");

    // read the creation time and tree size and ignore them as theyre unused here
    header_buffer->ReadU32();
    header_buffer->ReadU32();

    // read the end offset of the manifest kml
    uint32_t manifest_end_offset = header_buffer->ReadU32();

    // ifs versions after 1 have an md5 of the manifest here, skip it
    if (version > 1)
        header_buffer->SetOffset(header_buffer->GetOffset() + 16);

    // read the manifest
    unsigned int manifest_size = manifest_end_offset - header_buffer->GetOffset();
    unsigned char manifest_source[manifest_size];
    header_buffer->ReadBytes(manifest_size, manifest_source);
    KML::Document *manifest = new KML::Document(manifest_source);

    // create the data buffer
    data_buffer = new ByteBuffer(path, 0, manifest_end_offset);

    // load the root directory
    // pass the actual root node, <imgfs>
    root = new IFS::Directory(manifest->GetRoot()->GetNode("imgfs"), data_buffer, "imgfs");

    // delete the manifest and header buffer as theyre now finished
    delete manifest;
    delete header_buffer;
}

IFS::Document::~Document()
{
    delete root;
    delete data_buffer;
}
