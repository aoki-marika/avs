#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Camera3D.hpp>
#include <avs/Box.hpp>

int main()
{
    // setup the window and camera
    float width = 1280, height = 720;
    Window *window = new Window(width, height);
    Camera3D *camera = new Camera3D(90, width / height, 0.1f, 100.0f);
    camera->SetPosition(Vector3(5, 5, -5));

    // setup the box
    Box *box = new Box();
    box->SetSize(Vector3(1, 2, 1));
    box->SetPosition(Vector3(-0.5f, -1, 0));
    box->SetAlpha(0.5);
    box->SetBlendMode(BlendMode::Additive);
    box->SetColour(Colour4(1, 0, 1, 0.5));

    // draw the box for 100 frames
    for (int i = 0; i < 100; i++)
    {
        window->ClearBuffers();
        box->Draw(camera);
        window->SwapBuffers();
    }

    delete box;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
