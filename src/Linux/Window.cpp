#include "Window.hpp"

#include <assert.h>

// namespaced to avoid naming collisions
namespace X11
{
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/Xutil.h>
};

// data struct for linux window implementation
typedef struct
{
    X11::Display *Display;
    X11::Window Window;
} WindowData;

Window::Window(std::string title,
               unsigned int width,
               unsigned int height) : WindowBase::WindowBase(title, width, height)
{
    using namespace X11;

    // create the data struct
    Data = new WindowData();
    WindowData *data = (WindowData *)Data;

    // open the display
    data->Display = XOpenDisplay(NULL);
    assert(data->Display != NULL);

    // create the display window
    XSetWindowAttributes attributes;
    attributes.event_mask = StructureNotifyMask;
    data->Window = XCreateWindow(data->Display, //display
                                 XDefaultRootWindow(data->Display), //window
                                 0, //x
                                 0, //y
                                 width, //width
                                 height, //height
                                 0, //border width
                                 CopyFromParent, //depth
                                 InputOutput, //class
                                 CopyFromParent, //visual
                                 CWEventMask, //value mask
                                 &attributes); //attributes

    // override attribute redirect
    attributes = XSetWindowAttributes();
    attributes.override_redirect = false;
    XChangeWindowAttributes(data->Display, data->Window, CWOverrideRedirect, &attributes);

    // mark the window as non-composited
    int one = 1;
    XChangeProperty(data->Display, //display
                    data->Window, //window
                    XInternAtom(data->Display, "_HILDON_NON_COMPOSITED_WINDOW", false), //property
                    XA_INTEGER, //type
                    32, //format
                    PropModeReplace, //mode
                    (unsigned char *)&one, //data
                    1); //num elements

    // set the min and max window size to disallow resizing
    XSizeHints size_hints;
    size_hints.flags = PMinSize | PMaxSize;
    size_hints.min_width = size_hints.max_width = width;
    size_hints.min_height = size_hints.max_height = height;
    XSetWMNormalHints(data->Display, data->Window, &size_hints);

    // map the window
    XMapWindow(data->Display, data->Window);

    // set the windows title
    XStoreName(data->Display, data->Window, title.c_str());

    // wait for mapping to finish
    XEvent event;
    while (true)
    {
        XNextEvent(data->Display, &event);
        if (event.type == MapNotify)
            break;
    }

    // create the context
    CreateContext();
}

Window::~Window()
{
    WindowData *data = (WindowData *)Data;
    XDestroyWindow(data->Display, data->Window);
    XCloseDisplay(data->Display);
    delete data;
}

void *Window::GetDisplay()
{
    return ((WindowData *)Data)->Display;
}

void *Window::GetWindow()
{
    return &((WindowData *)Data)->Window;
}
