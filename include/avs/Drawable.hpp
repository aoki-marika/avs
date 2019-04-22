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

        // methods called before and after the vertices of this drawable are drawn
        // Drawable implements these to bind the vertex position to the fragmemt shader
        // so subclasses should call the base implementation when overriding
        // this drawables program is used before these methods are called
        virtual void BeginDraw();
        virtual void EndDraw();

        // the method called to draw the vertices of this drawable
        // Drawable implements this to draw the default quad vertex buffer
        // so subclasses should typically call the base implementation when overriding
        // todo: Drawable needs to be a bit more generic and not have a VertexBuffer
        // and have subclasses implement that instead
        virtual void DrawVertices();

    public:
        // create a new drawable with the given fragment shader source
        // note: all drawable fragment shaders should have a float uniform called "alpha"
        // that should be used to multiply the output alpha
        Drawable(std::string fragment_source);
        ~Drawable();

        // set the draw size/position of this drawable to the given value
        // defaults to 0,0,0
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
