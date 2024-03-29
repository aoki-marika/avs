#include "Window.hpp"

#include <assert.h>
#include <GLES2/gl2.h>

// namespaced to avoid naming collisions from egl includes
namespace EGL
{
    #include <EGL/egl.h>
};

#include "FreeType.hpp"

void Window::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers()
{
    EGL::eglSwapBuffers(egl_display, egl_surface);
}

void Window::CreateContext()
{
    using namespace EGL;

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
    egl_display = eglGetDisplay(*((EGLNativeDisplayType *)this->GetDisplay()));
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
    egl_surface = eglCreateWindowSurface(egl_display, config, *((EGLNativeWindowType *)this->GetWindow()), NULL);
    assert(egl_surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
    assert(result != EGL_FALSE);

    // setup gles
    glEnable(GL_BLEND);
    glClearColor(0, 0, 0, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // setup freetype
    FreeType::Init();
}

void Window::DeleteContext()
{
    FreeType::Deinit();
    EGL::eglDestroySurface(egl_display, egl_surface);
    EGL::eglDestroyContext(egl_display, egl_context);
    EGL::eglTerminate(egl_display);
}
