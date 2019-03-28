#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>

namespace KML
{
    enum NodeFormat
    {
        // data types
        S8            = 1 << 0,
        U8            = 1 << 1,
        S16           = 1 << 2,
        U16           = 1 << 3,
        S32           = 1 << 4,
        U32           = 1 << 5,
        S64           = 1 << 6,
        U64           = 1 << 7,
        Binary        = 1 << 8,
        String        = 1 << 9,
        IPv4          = 1 << 10,
        Timestamp     = 1 << 11, //unix epoch
        Float         = 1 << 12,
        Double        = 1 << 13,
        Attribute     = 1 << 14,
        Bool          = 1 << 15,

        // special types
        NodeStart     = 1 << 16,
        NodeEnd       = 1 << 17,
        SectionEnd    = 1 << 18,

        // number of values
        OneValues     = 1 << 19,
        TwoValues     = 1 << 20,
        ThreeValues   = 1 << 21,
        FourValues    = 1 << 22,
        EightValues   = 1 << 23,
        SixteenValues = 1 << 24,
    };

    template <class T> class NodeValue;
    template <class T> class NodeArray;

    class Node
    {
        private:
            // the name of the tag of this node
            std::string name;

            // the parent node of this node
            Node *parent = nullptr;

            // the attributes of this node, mapped name:value
            std::map<std::string, std::string> attributes;

            // the children nodes of this node
            std::vector<Node *> children;

        public:
            // create a node with the given tag name
            Node(std::string name);
            ~Node();

            // create a proper node subclass for the given type and bytes, with the given name
            // num_items should be the total amount of items (num_array * num_values)
            // item_size should be the size in bytes of each item of the given type
            // create_item should be a method to convert from bytes to the final data model
            // note: the caller is responsible for freeing the node
            template <class T>
            static Node *FromBytes(std::string name,
                                   const unsigned char *bytes,
                                   int num_items,
                                   size_t item_size,
                                   T (* create_item)(const unsigned char *, int))
            {
                // create the items
                std::vector<T> items(num_items);
                for (int i = 0; i < num_items; i++)
                    items[i] = create_item(bytes, i * item_size);

                // create a value/array node depending on how many items there are
                if (num_items == 1)
                    return new NodeValue<T>(name, items[0]);
                else
                    return new NodeArray<T>(name, items);
            }

            // add an attribute to this node of the given name with the given value
            void AddAttribute(std::string name, std::string value);

            // add a child node to this node
            // note: once a node is added to a parent, the lifetime of the node is managed by its parent
            // so once the parent is deleted, all of its children are also deleted
            void AddChild(Node *child);

            // get the value of the attribute of the given name from this node
            std::string GetAttribute(std::string name);

            // get the first/all child node of this node that has the given tag name
            // returns nullptr if no match(es) is/are found
            Node *GetNode(std::string name);
            std::vector<Node *> GetNodes(std::string name);

            // get the parent node of this node
            // returns nullptr if this node does not have a parent
            Node *GetParent()
            {
                return parent;
            }
    };

    // a node that holds a single value of type T
    template <class T>
    class NodeValue : public Node
    {
        private:
            T value;

        public:
            // create a value node with the given tag name and value
            NodeValue(std::string name, T value) : Node(name), value(value)
            {
            }

            // get the value of this node
            T GetValue()
            {
                return value;
            }
    };

    // a node that holds an array of values of type T
    template <class T>
    class NodeArray : public Node
    {
        private:
            std::vector<T> values;

        public:
            // create an array node with the given tag name and values
            NodeArray(std::string name, std::vector<T> values) : Node(name), values(values)
            {
            }

            // get the count of this nodes values
            size_t GetCount()
            {
                return values.size();
            }

            // get the values in this nodes values at the given index
            T GetValue(size_t i)
            {
                return values[i];
            }
    };

    // some useful aliases of nodes for each NodeFormat
    // note: all of these are pointers as theyre meant to be cast to from GetNode()
    typedef NodeValue<int8_t>        *NodeS8;
    typedef NodeValue<uint8_t>       *NodeU8;
    typedef NodeValue<int16_t>       *NodeS16;
    typedef NodeValue<uint16_t>      *NodeU16;
    typedef NodeValue<int32_t>       *NodeS32;
    typedef NodeValue<uint32_t>      *NodeU32;
    typedef NodeValue<int64_t>       *NodeS64;
    typedef NodeValue<uint64_t>      *NodeU64;
    typedef NodeArray<unsigned char> *NodeBinary;
    typedef NodeValue<std::string>   *NodeString;
    typedef NodeArray<unsigned char> *NodeIPv4;
    typedef NodeValue<uint32_t>      *NodeTimestamp;
    typedef NodeValue<float>         *NodeFloat;
    typedef NodeValue<double>        *NodeDouble;
    typedef NodeValue<bool>          *NodeBool;

    typedef NodeArray<int8_t>        *NodeS8Array;
    typedef NodeArray<uint8_t>       *NodeU8Array;
    typedef NodeArray<int16_t>       *NodeS16Array;
    typedef NodeArray<uint16_t>      *NodeU16Array;
    typedef NodeArray<int32_t>       *NodeS32Array;
    typedef NodeArray<uint32_t>      *NodeU32Array;
    typedef NodeArray<int64_t>       *NodeS64Array;
    typedef NodeArray<uint64_t>      *NodeU64Array;
    typedef NodeArray<unsigned char> *NodeBinaryArray;
    typedef NodeArray<std::string>   *NodeStringArray;
    typedef NodeArray<unsigned char> *NodeIPv4Array;
    typedef NodeArray<uint32_t>      *NodeTimestampArray;
    typedef NodeArray<float>         *NodeFloatArray;
    typedef NodeArray<double>        *NodeDoubleArray;
    typedef NodeArray<bool>          *NodeBoolArray;
};
