#include <stdlib.h>
#include <avs/Window.hpp>

int main()
{
    // render a 720x1280 window for 100 frames
    Window *window = new Window(720, 1280);

    int i = 0;
    while (i < 100)
    {
        window->SwapBuffers();
        i++;
    }

    delete window;
    return EXIT_SUCCESS;
}
