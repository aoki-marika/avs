#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Camera3D.hpp>
#include <avs/Atlas.hpp>
#include <avs/Sprite.hpp>

int main()
{
    // setup the window and camera
    float width = 1280, height = 720;
    Window *window = new Window("Sprite", width, height);
    Camera3D *camera = new Camera3D(90, width / height, 0.1f, 100.0f);
    camera->SetPosition(Vector3(-3, -2, -5));

    // setup the atlas
    // cant use gles constants as headers arent included, so just hardcode them
    Atlas *atlas = new Atlas(100,
                             100,
                             0x1908, //GL_RGBA
                             0x1401, //GL_UNSIGNED_BYTE
                             0x2600, 0x2600, //GL_NEAREST
                             0x812f, 0x812f); //GL_CLAMP_TO_EDGE

    // image one
    const unsigned char image_one_data[] =
    {
        255, 0,   0,   255,
        0,   255, 0,   255,

        0,   0,   255, 255,
        255, 0,   0,   155,

        0,   255, 0,   155,
        0,   0,   255, 155,

        255, 0,   0,   55,
        0,   255, 0,   55,
    };

    // add to atlas
    std::string image_one = "image_one";
    unsigned int image_one_width = 2, image_one_height = 4;
    atlas->AddImage(image_one, 30, 10, image_one_width, image_one_height, image_one_data);

    // create sprite
    Sprite *image_one_sprite = new Sprite();
    image_one_sprite->SetImage(atlas, "image_one");
    image_one_sprite->SetPosition(Vector3(-(float)image_one_width / 2, -(float)image_one_height / 2, 0));

    // image two
    const unsigned char image_two_data[] =
    {
        255, 0,   0,   250,
        0,   255, 0,   200,
        0,   0,   255, 150,

        255, 0,   0,   150,
        0,   255, 0,   100,
        0,   0,   255, 50,
    };

    // add to atlas
    std::string image_two = "image_two";
    unsigned int image_two_width = 3, image_two_height = 2;
    atlas->AddImage(image_two, 50, 0, image_two_width, image_two_height, image_two_data);

    // create sprite
    Sprite *image_two_sprite = new Sprite();
    image_two_sprite->SetImage(atlas, "image_two");
    image_two_sprite->SetPosition(Vector3(-(float)image_two_width / 2, -(float)image_two_height / 2, -2));
    image_two_sprite->SetAlpha(0.25f);

    // draw the sprites for 100 frames
    for (int i = 0; i < 100; i++)
    {
        window->ClearBuffers();
        image_one_sprite->Draw(camera);
        image_two_sprite->Draw(camera);
        window->SwapBuffers();
    }

    delete image_two_sprite;
    delete image_one_sprite;
    delete atlas;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
