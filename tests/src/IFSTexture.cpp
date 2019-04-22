#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Camera3D.hpp>
#include <avs/IFSDocument.hpp>
#include <avs/Sprite.hpp>

using namespace IFS;

// IFSTexture [path/to/document.ifs] [texture name] [image name 1] [image name 2] ..
int main(int argc, char **argv)
{
    // setup the window and camera
    float width = 1280, height = 720;
    Window *window = new Window("IFS::Texture", width, height);
    Camera3D *camera = new Camera3D(90, width / height, 0.1f, 4096.0f);

    // load the document
    Document *document = new Document(std::string(argv[1]));
    Atlas *atlas = document->GetAtlas(std::string(argv[2]));

    // create the sprites
    unsigned int num_sprites = argc - 3;
    Sprite *sprites[num_sprites];
    for (int i = 0; i < num_sprites; i++)
    {
        sprites[i] = new Sprite();
        sprites[i]->SetImage(atlas, std::string(argv[3 + i]));
        sprites[i]->SetPosition(Vector3(sinf(i) * 100, cosf(i) * -200, -i * 5));
    }

    // draw each sprite for 1024 frames, panning the camera each frame
    for (int i = 0; i < 1024; i++)
    {
        window->ClearBuffers();
        camera->SetPosition(Vector3(-200 + i, 0, -i * 4));

        for (int i = 0; i < num_sprites; i++)
            sprites[i]->Draw(camera);

        window->SwapBuffers();
    }

    for (int i = 0; i < num_sprites; i++)
        delete sprites[i];

    delete document;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
