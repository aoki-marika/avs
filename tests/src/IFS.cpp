#include <stdlib.h>
#include <avs/IFSDocument.hpp>

using namespace IFS;

int main(int argv, char **argc)
{
    // try to load every given argument as an ifs file
    for (int i = 1; i < argv; i++)
    {
        Document *document = new Document(std::string(argc[i]));
        delete document;
    }

    return EXIT_SUCCESS;
}
