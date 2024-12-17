#include <iostream>

#include "../AdventOfCode.2024/D01/Aoc2024D01.h"
#include "../AdventOfCode.2024/D02/Aoc2024D02.h"
#include "../AdventOfCode.2024/D03/Aoc2024D03.h"
#include "../AdventOfCode.Shared/AocRegistry.h"

int main(int argc, char* argv[])
{
    enum class ArgParseModes
    {
        NONE=0,
        BASEDIR=1
    };

    std::string basedir;
    bool benchmark = false;
    ArgParseModes parseMode = ArgParseModes::NONE;

    for(int i = 1; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);

        switch(parseMode)
        {
        case ArgParseModes::NONE:
            if (arg == "-benchmark")
            {
                benchmark = true;
            }
            else if (arg == "-basedir")
            {
                parseMode = ArgParseModes::BASEDIR;
            }

            break;

        case ArgParseModes::BASEDIR:
            basedir = arg;
            parseMode = ArgParseModes::NONE;
            break;
        }
    }

    Aoc2024D01* d01 = new Aoc2024D01();
    Aoc2024D02* d02 = new Aoc2024D02();
    Aoc2024D03* d03 = new Aoc2024D03();

    AocRegistry registry = AocRegistry(std::string(basedir));
    registry.registerDay(d01);
    registry.registerDay(d02);
    registry.registerDay(d03);

    if(benchmark)
    {
        registry.runBenchmark();
    }
    else
    {
        registry.run();
    }

    delete d01;
    delete d02;
    delete d03;
}
