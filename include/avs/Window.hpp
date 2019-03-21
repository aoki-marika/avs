#pragma once

#include <stdint.h>

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;
typedef uint32_t DISPMANX_DISPLAY_HANDLE_T;
typedef uint32_t DISPMANX_ELEMENT_HANDLE_T;

class Window
{
    private:
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;

        DISPMANX_DISPLAY_HANDLE_T dispman_display;
        DISPMANX_ELEMENT_HANDLE_T dispman_element;

    public:
        Window(int width, int height);
        ~Window();

        // clear the current frame buffer for this window
        // should be called at the start of each frame
        void ClearBuffers();

        // swap frame buffers for this window
        // should be called at the end of each frame
        void SwapBuffers();
};
