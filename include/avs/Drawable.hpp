#pragma once

#include "Camera.hpp"
#include "Program.hpp"
#include "Vector3.hpp"
#include "VertexBuffer.hpp"

enum class BlendMode
{
    Normal,
    Additive,
    Multiply,
};

// used as a base class, use Box for a functional implementation
class Drawable
{
    private:
        VertexBuffer *vertex_buffer;
        Program *program;

        unsigned int attrib_vertex_position;
        unsigned int uniform_pv, uniform_size, uniform_position, uniform_alpha;

        // the current blend mode of this drawable
        BlendMode blend_mode;

    protected:
        // get the program of this drawable
        Program *GetProgram()
        {
            return program;
        }

    public:
        // note: all drawable fragment shaders should have a float uniform called "alpha"
        // that should be used to multiply the output alpha
        Drawable(std::string fragment_source);
        ~Drawable();

        // set the draw size/position of this drawable to the given value
        // note: these values are used according to the conventions of the given camera when drawing
        void SetSize(Vector3 size);
        void SetPosition(Vector3 position);

        // set the draw alpha of this drawable to the given value on a scale of 0 to 1
        // defaults to 1
        void SetAlpha(float alpha);

        // set the alpha blend mode of this drawable to the given mode
        // defaults to normal
        void SetBlendMode(BlendMode mode);

        // draw this drawables current state using the given camera
        void Draw(Camera *camera);
};
