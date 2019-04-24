#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Camera2D.hpp>
#include <avs/FreeTypeFont.hpp>
#include <avs/Text.hpp>

// FreeTypeFont [path/to/font.ttf] [face index] [height] [string 1] [string 2] ..
int main(int argc, char **argv)
{
    // setup the window and camera
    float width = 1280, height = 720;
    Window *window = new Window("Text + FreeTypeFont", width, height);
    Camera2D *camera = new Camera2D(width, height, -1, 1);

    // setup the font
    FreeTypeFont *font = new FreeTypeFont(std::string(argv[1]), atoi(argv[2]), atoi(argv[3]));

    // setup the texts
    unsigned int num_texts = argc - 4;
    Text *texts[num_texts];
    for (int i = 0; i < num_texts; i++)
    {
        std::string string = std::string(argv[4 + i]);
        Text *text = new Text(string.size(), BufferUsage::Static);
        text->SetFont(font);
        text->SetString(string);
        text->SetPosition(Vector3(10, height - ((atoi(argv[3]) + 10) * (i + 1)), 0));

        // create a colour pattern of white, red, yellow, pink, yellow, red, white
        // white a descending alpha gradient from 1.0 to 0.1
        text->SetColour(Colour4((i % 1) == 0,
                                (i % 2) == 0,
                                (i % 3) == 0,
                                1.0 - ((float)i / (float)num_texts) * 0.9f));

        texts[i] = text;
    }

    // draw the texts for 100 frames
    for (int i = 0; i < 100; i++)
    {
        window->ClearBuffers();
        for (int i = 0; i < num_texts; i++)
            texts[i]->Draw(camera);
        window->SwapBuffers();
    }

    for (int i = 0; i < num_texts; i++)
        delete texts[i];
    delete font;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
