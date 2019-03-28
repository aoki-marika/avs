#include "KMLNode.hpp"

KML::Node::Node(std::string name) : name(name)
{
}

KML::Node::~Node()
{
    for (auto c: children)
        delete c;
}

void KML::Node::AddAttribute(std::string name, std::string value)
{
    attributes[name] = value;
}

void KML::Node::AddChild(KML::Node *child)
{
    children.push_back(child);
    child->parent = this;
}

std::string KML::Node::GetAttribute(std::string name)
{
    return attributes[name];
}

KML::Node *KML::Node::GetNode(std::string name)
{
    for (auto c: children)
        if (c->name == name)
            return c;

    return nullptr;
}

std::vector<KML::Node *> KML::Node::GetNodes(std::string name)
{
    std::vector<KML::Node *> nodes;
    for (auto c: children)
        if (c->name == name)
            nodes.push_back(c);

    return nodes;
}
