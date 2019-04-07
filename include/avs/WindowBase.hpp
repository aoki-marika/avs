#pragma once

#include <string>

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;

// a base class that implements cross platform window functionality
class WindowBase
{
    private:
        EGLDisplay egl_display;
        EGLSurface egl_surface;
        EGLContext egl_context;

    protected:
        // get the egl display/window for this window
        virtual void *GetDisplay() = 0;
        virtual void *GetWindow() = 0;

        // create the egl context for this window
        // should be called after window creation in subclass ctors
        void CreateContext();

    public:
        // create a new window with the given title, width, and height
        // then create and setup an egl context for it
        WindowBase(std::string title,
                   unsigned int width,
                   unsigned int height);
        ~WindowBase();

        // clear the current frame buffer for this window
        // should be called at the start of each frame
        void ClearBuffers();

        // swap frame buffers for this window
        // should be called at the end of each frame
        void SwapBuffers();
};
