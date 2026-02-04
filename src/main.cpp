#include "InputProcessing.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "The file isn't specified\n";
        return 0;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open())
    {
        std::cout << "The file can't be opened\n";
        return 0;
    }

    runFile(file);

    return 0;
}