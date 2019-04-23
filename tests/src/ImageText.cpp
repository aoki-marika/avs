#include <stdlib.h>
#include <string.h>
#include <avs/Window.hpp>
#include <avs/Camera3D.hpp>
#include <avs/IFSDocument.hpp>
#include <avs/ImageFont.hpp>
#include <avs/Text.hpp>

using namespace IFS;

// CompositeSprite [path/to/document.ifs] [texture name] [spacing] [char 1] [image for char 1] [char 2] [image for char 2]..
int main(int argc, char **argv)
{
    // setup the window and camera
    const unsigned int max_length = 10;
    float width = 1280, height = 720;
    Window *window = new Window("Text + ImageFont", width, height);
    Camera3D *camera = new Camera3D(90, width / height, 0.1f, 2048.0f);

    // setup the font
    Document *document = new Document(std::string(argv[1]));
    ImageFont *font = new ImageFont(document->GetAtlas(std::string(argv[2])), atof(argv[3]));
    for (int i = 4; i < argc; i += 2)
        font->SetGlyph(argv[i][0], std::string(argv[i + 1]));

    // setup the text
    Text *text = new Text(max_length, BufferUsage::Dynamic);
    text->SetFont(font);

    // draw the text for n frames
    int n = 1024;
    for (int i = 0; i < n; i++)
    {
        window->ClearBuffers();

        // set the texts string so it goes from n / 2 to 0 to n / 2
        // this highlights ensuring that only the set characters are drawn
        int value = abs(-n / 2 + i);
        char characters[max_length + 1];
        sprintf(characters, "%i", value);
        text->SetString(std::string(characters));

        // move the camera with the text count
        camera->SetPosition(Vector3(-n / 2 + i, 0, -value - 50));

        text->Draw(camera);
        window->SwapBuffers();
    }

    delete text;
    delete font;
    delete document;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
