#include <stdlib.h>
#include <avs/Window.hpp>

int main()
{
    // render a 720x1280 window for 100 frames
    Window *window = new Window(720, 1280);

    for (int i = 0; i < 100; i++)
        window->SwapBuffers();

    delete window;
    return EXIT_SUCCESS;
}
