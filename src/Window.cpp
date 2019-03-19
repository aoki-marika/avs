#include "Window.hpp"

#include <assert.h>
#include <bcm_host.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

Window::Window(int width, int height)
{
    bcm_host_init();

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

    EGLBoolean result;

    // get the display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display != EGL_NO_DISPLAY);
    result = eglInitialize(display, NULL, NULL);
    assert(result != EGL_FALSE);

    // get a framebuffer configuration
    EGLConfig config;
    EGLint num_configs;
    result = eglChooseConfig(display, attributes, &config, 1, &num_configs);
    assert(result != EGL_FALSE);

    // get a gles api
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(result != EGL_FALSE);

    // create the rendering context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
    assert(context != EGL_NO_CONTEXT);

    // create the draw and window rects
    VC_RECT_T draw_rect = (VC_RECT_T)
    {
        .x = 0,
        .y = 0,
        .width = width << 16,
        .height = height << 16,
    };

    VC_RECT_T window_rect = (VC_RECT_T)
    {
        .x = 0,
        .y = 0,
        .width = width,
        .height = height,
    };

    // get the dispman handles

    // make the background of the window 100% opaque to fix issues with alpha blending
    VC_DISPMANX_ALPHA_T dispman_alpha =
    {
        .flags = DISPMANX_FLAGS_ALPHA_FIXED_NON_ZERO,
        .opacity = 255,
    };

    dispman_display = vc_dispmanx_display_open(0);
    DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start(0);
    dispman_element = vc_dispmanx_element_add(dispman_update,           //update
                                              dispman_display,          //display
                                              0,                        //layer
                                              &window_rect,             //dest_rect
                                              0,                        //src
                                              &draw_rect,               //src_rect
                                              DISPMANX_PROTECTION_NONE, //protection
                                              &dispman_alpha,           //alpha
                                              0,                        //clamp
                                              (DISPMANX_TRANSFORM_T)0); //transform

    // create the dispman window
    static EGL_DISPMANX_WINDOW_T window =
    {
        .element = dispman_element,
        .width = width,
        .height = height,
    };

    vc_dispmanx_update_submit_sync(dispman_update);

    // create the surface
    surface = eglCreateWindowSurface(display, config, &window, NULL);
    assert(surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(display, surface, surface, context);
    assert(result != EGL_FALSE);
}

Window::~Window()
{
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);

    // close the dispman display
    DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start(10);
    assert(dispman_update != DISPMANX_NO_HANDLE);

    int result;
    result = vc_dispmanx_element_remove(dispman_update, dispman_element);
    assert(result == 0);
    result = vc_dispmanx_update_submit_sync(dispman_update);
    assert(result == 0);
    result = vc_dispmanx_display_close(dispman_display);
    assert(result == 0);

    bcm_host_deinit();
}

void Window::SwapBuffers()
{
    eglSwapBuffers(display, surface);
}
