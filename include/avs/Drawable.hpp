#pragma once

#include "Camera.hpp"
#include "Program.hpp"
#include "Vector3.hpp"

enum class BlendMode
{
    Normal,
    Additive,
    Multiply,
};

// used as a base class for anything that can be drawn on screen
// note: when subclasses create a quad vertex buffer with below values
// 1, 1, 1
// 1, 2, 1
// 2, 2, 1
// 2, 1, 1
// then it is intended to use the exact size and position passed to SetSize/SetPosition
// these values are used so the size/position can just be multipliers to the vertex position
// and the vertex buffer never has to be modified, which is nice as changing vertex buffer data is annoying
class Drawable
{
    private:
        int uniform_pv, uniform_size, uniform_position, uniform_alpha;

        // the program of this drawable
        Program *program;

        // the current blend mode of this drawable
        BlendMode blend_mode;

    protected:
        // get the program of this drawable
        Program *GetProgram()
        {
            return program;
        }

        // the method called to draw the vertices of the drawable
        // this drawables program is used and passed the pv matrix before this is called
        virtual void DrawVertices() = 0;

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
