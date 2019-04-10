#include "Window.hpp"

#include <assert.h>
#include <bcm_host.h>
#include <EGL/egl.h>

// data struct for raspberry window implementation
typedef struct
{
    DISPMANX_DISPLAY_HANDLE_T Display;
    DISPMANX_ELEMENT_HANDLE_T Element;
    EGL_DISPMANX_WINDOW_T *Window;
    EGLNativeDisplayType EglDisplay;
} WindowData;

Window::Window(std::string title,
               unsigned int width,
               unsigned int height) : WindowBase::WindowBase(title, width, height)
{
    // create the data struct
    Data = new WindowData();
    WindowData *data = (WindowData *)Data;

    // init the broadcam host
    bcm_host_init();

    // create the draw and window rects
    // dispmanx complains about these being unsigned
    int swidth = width, sheight = height;

    VC_RECT_T draw_rect = (VC_RECT_T)
    {
        .x = 0,
        .y = 0,
        .width = swidth << 16,
        .height = sheight << 16,
    };

    VC_RECT_T window_rect = (VC_RECT_T)
    {
        .x = 0,
        .y = 0,
        .width = swidth,
        .height = sheight,
    };

    // open the default display
    data->Display = vc_dispmanx_display_open(0);

    // get the element
    // make the background of the window 100% opaque to fix issues with alpha blending
    VC_DISPMANX_ALPHA_T alpha =
    {
        .flags = DISPMANX_FLAGS_ALPHA_FIXED_NON_ZERO,
        .opacity = 255,
    };

    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);
    data->Element = vc_dispmanx_element_add(update, //update
                                            data->Display, //display
                                            0, //layer
                                            &window_rect, //dest_rect
                                            0, //src
                                            &draw_rect, //src_rect
                                            DISPMANX_PROTECTION_NONE, //protection
                                            &alpha, //alpha
                                            0, //clamp
                                            (DISPMANX_TRANSFORM_T)0); //transform

    // create the window
    data->Window = new EGL_DISPMANX_WINDOW_T();
    data->Window->element = data->Element;
    data->Window->width = swidth;
    data->Window->height = sheight;

    // set the egl display
    // cant just be passed from GetDisplay as it requires a pointer
    data->EglDisplay = EGL_DEFAULT_DISPLAY;

    // submit the init update
    vc_dispmanx_update_submit_sync(update);

    // create the context
    CreateContext();
}

Window::~Window()
{
    WindowData *data = (WindowData *)Data;
    DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start(10);
    assert(dispman_update != DISPMANX_NO_HANDLE);

    int result;
    result = vc_dispmanx_element_remove(dispman_update, data->Element);
    assert(result == 0);
    result = vc_dispmanx_update_submit_sync(dispman_update);
    assert(result == 0);
    result = vc_dispmanx_display_close(data->Display);
    assert(result == 0);

    bcm_host_deinit();
    delete data->Window;
    delete data;
}

void *Window::GetDisplay()
{
    return &((WindowData *)Data)->EglDisplay;
}

void *Window::GetWindow()
{
    return &((WindowData *)Data)->Window;
}
