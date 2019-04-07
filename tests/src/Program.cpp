#include <stdlib.h>
#include <avs/Window.hpp>
#include <avs/Shader.hpp>
#include <avs/Program.hpp>

int main()
{
    // need a window to get a gles context
    Window *window = new Window("Context", 1, 1);

    std::string vertex_source = R"VERTEX(
void main()
{
    gl_Position = vec4(0, 0, 0, 0);
}
)VERTEX";

    std::string fragment_source = R"FRAGMENT(
void main()
{
    gl_FragColor = vec4(1, 0, 0, 1);
}
)FRAGMENT";

    Shader *vertex = new Shader(ShaderType::Vertex, vertex_source);
    Shader *fragment = new Shader(ShaderType::Fragment, fragment_source);
    Program *program = new Program(vertex, fragment);

    delete program;
    delete fragment;
    delete vertex;
    delete window;
    return EXIT_SUCCESS;
}
