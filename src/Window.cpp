#include "Window.hpp"

#include <assert.h>
#include <GLES2/gl2.h>

// x11 has conflicting typenames, e.g. Window, so wrap it in a namespace
namespace Linux
{
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>

    // needs to be in the same namespace as x11 as it references x11 types
    #include <EGL/egl.h>
};

Window::Window(std::string title, int width, int height)
{
    // setup the window and gles context
    setupWindow(title, width, height);
    setupContext();

    // setup gles
    glEnable(GL_BLEND);
    glClearColor(0, 0, 0, 1);
}

Window::~Window()
{
    Linux::eglDestroySurface(egl_display, egl_surface);
    Linux::eglDestroyContext(egl_display, egl_context);
    Linux::eglTerminate(egl_display);

    Linux::XDestroyWindow(x_display, x_window);
    Linux::XCloseDisplay(x_display);
}

void Window::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers()
{
    Linux::eglSwapBuffers(egl_display, egl_surface);
}

void Window::setupWindow(std::string title, int width, int height)
{
    // open the display
    x_display = Linux::XOpenDisplay(NULL);
    assert(x_display != NULL);

    // create the display window
    Linux::XSetWindowAttributes attributes;
    x_window = Linux::XCreateWindow(x_display,
                                    Linux::XDefaultRootWindow(x_display),
                                    0,
                                    0,
                                    width,
                                    height,
                                    0,
                                    CopyFromParent,
                                    InputOutput,
                                    CopyFromParent,
                                    CWEventMask,
                                    &attributes);

    // override attribute redirect
    attributes = Linux::XSetWindowAttributes();
    attributes.override_redirect = false;
    Linux::XChangeWindowAttributes(x_display, x_window, CWOverrideRedirect, &attributes);

    // needed for XA_INTEGER macro
    using Linux::Atom;

    // mark the window as non-composited
    int one = 1;
    Linux::XChangeProperty(x_display,
                         x_window,
                         Linux::XInternAtom(x_display, "_HILDON_NON_COMPOSITED_WINDOW", false),
                         XA_INTEGER,
                         32,
                         PropModeReplace,
                         (unsigned char *)&one,
                         1);

    // set the min and max window size to disallow resizing
    Linux::XSizeHints size_hints;
    size_hints.flags = PMinSize | PMaxSize;
    size_hints.min_width = size_hints.max_width = width;
    size_hints.min_height = size_hints.max_height = height;
    Linux::XSetWMNormalHints(x_display, x_window, &size_hints);

    // map the window
    Linux::XMapWindow(x_display, x_window);

    // set the windows title
    Linux::XStoreName(x_display, x_window, title.c_str());
}

void Window::setupContext()
{
    using namespace Linux;

    // egl and context attributes
    static const EGLint attributes[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_SAMPLE_BUFFERS, 1,
        EGL_SAMPLES, 4, //4x msaa
        EGL_NONE,
    };

    static const EGLint context_attributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
    };

    // the last egl call result
    EGLBoolean result;

    // get the display
    egl_display = eglGetDisplay((EGLNativeDisplayType)x_display);
    assert(egl_display != EGL_NO_DISPLAY);
    result = eglInitialize(egl_display, NULL, NULL);
    assert(result != EGL_FALSE);

    // get a framebuffer configuration
    EGLConfig config;
    EGLint num_configs;
    result = eglChooseConfig(egl_display, attributes, &config, 1, &num_configs);
    assert(result != EGL_FALSE);

    // get a gles api
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(result != EGL_FALSE);

    // create the rendering context
    egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, context_attributes);
    assert(egl_context != EGL_NO_CONTEXT);

    // create the surface
    egl_surface = eglCreateWindowSurface(egl_display, config, x_window, NULL);
    assert(egl_surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
    assert(result != EGL_FALSE);
}
