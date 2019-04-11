#pragma once

#include <string>

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;

// partially implemented for cross platform and platform specific
class Window
{
    private:
        EGLDisplay egl_display;
        EGLSurface egl_surface;
        EGLContext egl_context;

    protected:
        // internal struct for implementation specific data
        void *Data;

        // get the egl display/window for this window
        // implemented per platform
        void *GetDisplay();
        void *GetWindow();

        // create the egl context for this window
        // should be called after window creation in implementation ctors
        void CreateContext();

        // tear down the egl context for this window
        // should be called in implementation dtors
        void DeleteContext();

    public:
        // create a new window with the given title, width, and height
        // then create and setup an egl context for it
        // implemented per platform
        Window(std::string title,
               unsigned int width,
               unsigned int height);
        ~Window();

        // clear the current frame buffer for this window
        // should be called at the start of each frame
        void ClearBuffers();

        // swap frame buffers for this window
        // should be called at the end of each frame
        void SwapBuffers();
};
