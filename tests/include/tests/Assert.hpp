#pragma once

#include <stdlib.h>
#include <math.h>
#include <array>
#include <sstream>
#include <iostream>
#include <avs/KMLNode.hpp>
#include <avs/Matrix4.hpp>

namespace Assert
{
    // the epsilon for float equals asserts
    static const float EPSILON = 0.001;

    template <class T>
    static void Failure(std::string name, T *value, T *expected)
    {
        std::cerr << " -- " << name << ": Value is not equal to expected." << std::endl;
        std::cerr << "    " << "Expected:" << std::endl;
        std::cerr << "    " << *expected << std::endl;
        std::cerr << "    " << "Got:" << std::endl;
        std::cerr << "    " << *value << std::endl;
        exit(EXIT_FAILURE);
    }

    template <class T>
    static void Equals(std::string name, T value, T expected)
    {
        if (value != expected)
            Failure(name, &value, &expected);
    }


    static void Matrix4Equals(std::string name, Matrix4 value, Matrix4 expected)
    {
        bool failure = false;

        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (fabs(value.Values[x][y] - expected.Values[x][y]) > EPSILON)
                {
                    failure = true;
                    break;
                }
            }
        }

        if (failure)
        {
            std::ostringstream value_stream, expected_stream;
            value_stream << std::fixed;
            expected_stream << std::fixed;

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    value_stream << value.Values[x][y];
                    expected_stream  << expected.Values[x][y];

                    if (x < 3)
                    {
                        value_stream << ", ";
                        expected_stream << ", ";
                    }
                }

                value_stream << std::endl << "    ";
                expected_stream << std::endl << "    ";
            }

            std::string value_string = value_stream.str(), expected_string = expected_stream.str();
            Failure(name, &value_string, &expected_string);
        }
    }

    static void NodeAttributeEquals(std::string name, KML::Node *node, std::string attribute_name, std::string expected)
    {
        std::string value = node->GetAttribute(attribute_name);
        if (value != expected)
            Failure(name, &value, &expected);
    }

    template <class T>
    static void NodeValueEquals(std::string name, KML::Node *node, T expected)
    {
        KML::NodeValue<T> *value_node = (KML::NodeValue<T> *)node;
        T value = value_node->GetValue();
        if (value != expected)
            Failure(name, &value, &expected);
    }

    template <class T, unsigned N>
    static void NodeArrayEquals(std::string name, KML::Node *node, std::array<T, N> expected)
    {
        KML::NodeArray<T> *array_node = (KML::NodeArray<T> *)node;
        bool failed = false;

        if (array_node->GetCount() != N)
        {
            failed = true;
        }
        else
        {
            for (int i = 0; i < N; i++)
            {
                if (array_node->GetValue(i) != expected[i])
                {
                    failed = true;
                    break;
                }
            }
        }

        if (failed)
        {
            // convert the node values and expected to strings so they can be displayed
            // theres all sorts of potential nonsense from just passing the vector/array to Failure
            // note: this prints hex as their u8 values, not sure it can be done properly as in most cases uint8_t is just typedef unsigned char
            // and id rather print hex as ints than ints as hex
            std::ostringstream value_stream, expected_stream;
            value_stream << "{ ";
            expected_stream << "{ ";

            for (int i = 0; i < array_node->GetCount(); i++)
            {
                value_stream << +array_node->GetValue(i);
                if (i < array_node->GetCount() - 1)
                    value_stream << ", ";
            }

            for (int i = 0; i < N; i++)
            {
                expected_stream << +expected[i];
                if (i < N - 1)
                    expected_stream << ", ";
            }

            value_stream << " }";
            expected_stream << " }";

            // get the strings and fail
            std::string expected_string = expected_stream.str(), value_string = value_stream.str();
            Failure(name, &value_string, &expected_string);
        }
    }
};
