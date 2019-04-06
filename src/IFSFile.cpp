#include "IFSFile.hpp"

IFS::File::File(KML::Node *node,
                ByteBuffer *data_buffer,
                std::string name) : IFS::Entry(node, name), data_buffer(data_buffer)
{
    // cast the node to its proper data node type
    KML::NodeS32Array data_node = (KML::NodeS32Array)node;

    // load the data from the node
    // if there are 3 values then theres a timestamp, but this program doesnt use it so ignore it
    data_offset = data_node->GetValue(0);
    data_size = data_node->GetValue(1);
}

void IFS::File::GetData(unsigned char *out)
{
    data_buffer->SetOffset(data_offset);
    data_buffer->ReadBytes(data_size, out);
}
