#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Camera3D.hpp>
#include <avs/Box.hpp>

int main()
{
    // setup the window and camera
    float width = 1280, height = 720;
    Window *window = new Window("Transformable", width, height);
    Camera3D *camera = new Camera3D(90, width / height, 0.1f, 100.0f);
    camera->SetPosition(Vector3(10, 15, -15));

    // sequence box tests Then(), Delay(), and [Begin/End]Sequence()
    // swing box tests various transforms simultaneously

    // setup the sequence box
    Box *sequence_box = new Box();
    sequence_box->SetSize(Vector3(1, 2, 1));

    // colour to red
    sequence_box->ColourTo(Colour4(1, 0, 0, 1), 250, Easing::InQuad);

    // then position to 5,10,-10
    sequence_box->Then();
    sequence_box->PositionTo(Vector3(5, 10, -10), 500, Easing::OutQuad);

    // then size.x to 5
    sequence_box->Then();
    sequence_box->SizeXTo(5, 350, Easing::OutQuad);

    // then size.y to 0.1
    sequence_box->Then();
    sequence_box->SizeYTo(0.1f, 450, Easing::InQuad);

    // then alpha to 0.25
    sequence_box->Then();
    sequence_box->AlphaTo(0.25f, 500);

    // then delay 500 and alpha to 1, size to 1,2,1, colour to blue, and position to 0,0,0
    sequence_box->Then();
    sequence_box->Delay(500);
    sequence_box->BeginSequence();
        sequence_box->AlphaTo(1, 250, Easing::OutQuad);
        sequence_box->SizeTo(Vector3(2, 5, 1), 1000, Easing::OutQuad);
        sequence_box->ColourTo(Colour4(0, 0, 1, 1), 3000, Easing::OutQuad);
        sequence_box->PositionTo(Vector3(10, 10, -10), 1000, Easing::OutQuad);
    sequence_box->EndSequence();

    // setup the swing box
    Box *swing_box = new Box();
    swing_box->SetSize(Vector3(2, 1, 1));

    // delay the swing till after the sequence box has animated some
    swing_box->Delay(2500);
    swing_box->BeginSequence();
        // position.x to 5
        swing_box->PositionXTo(5, 750, Easing::OutQuad);

        // position.y to 10
        swing_box->PositionYTo(10, 1000, Easing::InQuad);

        // position.z to -10
        swing_box->PositionZTo(-10, 1000);

        // size to 2,4,1
        swing_box->SizeTo(Vector3(2, 4, 1), 1000);

        // alpha to 0.25
        swing_box->AlphaTo(0.25f, 1000, Easing::OutQuad);

        // colour to pink
        swing_box->ColourTo(Colour4(1, 0, 1, 1), 1000, Easing::InQuad);
    swing_box->EndSequence();

    // draw the boxes for 300 frames
    for (int i = 0; i < 300; i++)
    {
        window->ClearBuffers();
        sequence_box->Draw(camera);
        swing_box->Draw(camera);
        window->SwapBuffers();
    }

    delete swing_box;
    delete sequence_box;
    delete camera;
    delete window;
    return EXIT_SUCCESS;
}
