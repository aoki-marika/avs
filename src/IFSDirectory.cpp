#include "IFSDirectory.hpp"

#include "IFSDirectoryTexture.hpp"
#include "IFSFile.hpp"

IFS::Directory::Directory(KML::Node *node,
                          ByteBuffer *data_buffer,
                          std::string name) : IFS::Entry(node, name)
{
    for (auto c: node->GetChildren())
    {
        std::string name = c->GetName();

        // special characters
        name = stringReplaceAll(name, "_E", ".");
        name = stringReplaceAll(name, "__", "_");
        if (name.at(0) == '_' && isdigit(name.at(1)))
            name.erase(0, 1);

        // ignore various unused files/directories
        if (name == "_info_" || //metadata
            name == "_super_")  //sub-reference (todo: are sub-referenced ifs needed?)
            continue;

        // get the entries type
        // directory
        if (c->GetChildrenCount() > 0 && c->GetChild(0)->GetName() != "i")
        {
            // handle the various directory types
            if (name == "tex")
                children.push_back(new IFS::DirectoryTexture(c, data_buffer, name));
            else
                children.push_back(new IFS::Directory(c, data_buffer, name));
        }
        // file
        else
            children.push_back(new IFS::File(c, data_buffer, name));
    }
}

IFS::Directory::~Directory()
{
    RemoveEntries();
}

IFS::Entry *IFS::Directory::GetEntry(std::string name)
{
    for (auto c: children)
        if (c->Name == name)
            return c;

    return nullptr;
}

void IFS::Directory::RemoveEntries()
{
    for (auto c: children)
        delete c;
    children.clear();
}

std::string IFS::Directory::stringReplaceAll(std::string string, std::string match, std::string replacement)
{
    while (true)
    {
        size_t position = string.find(match);
        if (position != std::string::npos)
            string.replace(position, match.length(), replacement);
        else
            break;
    }

    return string;
}
