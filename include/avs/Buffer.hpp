#pragma once

enum BufferUsage
{
    // the buffer is meant to have its data assigned once
    Static,

    // the buffer is meant to have its data assigned many times
    Dynamic,
};

// note: buffer is hardcoded to only accept float data
// this is all thats needed for now so its fine, but in the future this may need to change
class Buffer
{
    private:
        // the id of the gl buffer of this buffer
        unsigned int id;

        // the number of components per value in this buffer
        unsigned int num_value_components;

        // the number of values this buffer is allocated to hold
        unsigned int num_values;

    protected:
        // set the data at the given index in this buffer to the given data
        // index/size are in values
        // size should be the length of data
        void SetData(unsigned int index,
                     unsigned int size,
                     const float *data);

    public:
        // create a new buffer of the given size with the given usage
        // num_value_components is the number of components in each value of this buffer
        // num_values is the number of values with num_value_components components that this buffer is allocated to hold
        Buffer(unsigned int num_value_components,
               unsigned int num_values,
               BufferUsage usage);
        ~Buffer();

        // bind/unbind this buffer to the given vertex attribute
        // attribute should be the id of the attribute from Program::GetAttribute();
        // unbind should always be called after bind is called
        void BindAttribute(int attribute);
        void UnbindAttribute(int attribute);
};
