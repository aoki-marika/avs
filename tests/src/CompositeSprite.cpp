#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Camera2D.hpp>
#include <avs/CompositeSprite.hpp>
#include <avs/IFSDocument.hpp>
#include <avs/Atlas.hpp>

using namespace IFS;

// CompositeSprite [path/to/document.ifs] [texture name] [image name 1] [image name 2] ..
int main(int argc, char **argv)
{
    // setup the window and camera
    float width = 1280, height = 720;
    const unsigned int num_max_sprites = 100;
    Window *window = new Window("CompositeSprite", width, height);
    Camera2D *camera = new Camera2D(width, height, -100, 100);

    // setup the composite sprite
    CompositeSprite *sprite = new CompositeSprite(num_max_sprites, BufferUsage::Static);
    Document *document = new Document(std::string(argv[1]));
    Atlas *atlas = document->GetAtlas(std::string(argv[2]));
    sprite->SetAtlas(atlas);

    // create a sine wave of the given images
    std::vector<CompositeSprite::Sprite> sprites(num_max_sprites);
    unsigned int size = 100;
    for (int i = 0; i < argc - 3 && i < num_max_sprites; i++)
    {
        float x = -size + (i * size);
        x -= size + ((int)x / (int)width) * width;
        sprites[i] = CompositeSprite::Sprite(x,
                                             (height / 2 - size / 2) + sin(i) * size,
                                             std::string(argv[3 + i]));
    }

    sprite->SetSprites(&sprites);

    // draw the composite sprite for 100 frames
    for (int i = 0; i < 100; i++)
    {
        window->ClearBuffers();
        sprite->Draw(camera);
        window->SwapBuffers();
    }

    delete document;
    delete sprite;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
