#pragma once

#include <string>

namespace Linux
{
    struct _XDisplay;
    typedef _XDisplay Display;

    typedef unsigned long CARD32;
    typedef CARD32 XID;
    typedef XID Window;

    typedef void *EGLDisplay;
    typedef void *EGLSurface;
    typedef void *EGLContext;
};

class Window
{
    private:
        Linux::EGLDisplay egl_display;
        Linux::EGLSurface egl_surface;
        Linux::EGLContext egl_context;

        Linux::Display *x_display;
        Linux::Window x_window;

        void setupWindow(std::string title, int width, int height);
        void setupContext();

    public:
        Window(std::string title, int width, int height);
        ~Window();

        // clear the current frame buffer for this window
        // should be called at the start of each frame
        void ClearBuffers();

        // swap frame buffers for this window
        // should be called at the end of each frame
        void SwapBuffers();
};
