#include <iostream>
#include <fstream>
#include "club.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        return 1;
    }

    InputParser parser;
    if (!parser.parse(file))
    {
        std::cout << parser.getErrorLine() << "\n";
        return 1;
    }

    Club club(parser.getConfig(), parser.getEvents());
    club.process();
    club.printResults();
}