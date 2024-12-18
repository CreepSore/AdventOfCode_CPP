#include <iostream>

#include "../AdventOfCode.2024/D01/Aoc2024D01.h"
#include "../AdventOfCode.2024/D02/Aoc2024D02.h"
#include "../AdventOfCode.2024/D03/Aoc2024D03.h"
#include "../AdventOfCode.2024/D04/Aoc2024D04.h"
#include "../AdventOfCode.Shared/AocRegistry.h"

struct MenuResult
{
    int year;
    int day;

    MenuResult(const int year, const int day)
    {
        this->year = year;
        this->day = day;
    }
};

MenuResult showMenu()
{
    int year = -1;
    int day = -1;
    std::string result;

    do
    {
        std::cout << "Hello plsplsplspls input Year:\n> ";
        try
        {
            std::getline(std::cin, result);
            year = std::stoi(result);
        }
        catch(...)
        {
            year = static_cast<int>(
                std::chrono::year_month_day{time_point_cast<std::chrono::days>(std::chrono::system_clock::now())}
                    .year()
            );

            std::cout << std::to_string(year) << '\n';
        }
    } while (year == -1);

    do
    {
        std::cout << "\nHello plsplsplspls input Day:\n> ";
        try
        {
            std::getline(std::cin, result);
            day = std::stoi(result);
        }
        catch (...)
        {
            std::cout << "PLS RETRY\n";
        }
    } while (day == -1);

    return {year, day};
}

int main(int argc, char* argv[])
{
    enum class ArgParseModes : uint8_t
    {
        NONE=0,
        BASEDIR=1,
        DAY=2,
        YEAR=3
    };

    std::string basedir;
    bool benchmark = false;
    bool interactive = false;
    int day = -1;
    int year = -1;
    ArgParseModes parseMode = ArgParseModes::NONE;

    for(int i = 1; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);

        switch (parseMode)
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
            else if (arg == "-day")
            {
                parseMode = ArgParseModes::DAY;
            }
            else if (arg == "-year")
            {
                parseMode = ArgParseModes::YEAR;
            }
            else if(arg == "-int")
            {
                interactive = true;
            }

            break;

        case ArgParseModes::BASEDIR:
            basedir = arg;
            parseMode = ArgParseModes::NONE;
            break;

        case ArgParseModes::DAY:
            day = std::stoi(arg);
            parseMode = ArgParseModes::NONE;
            break;

        case ArgParseModes::YEAR:
            year = std::stoi(arg);
            parseMode = ArgParseModes::NONE;
            break;
        }
    }

    if(interactive)
    {
        auto menuData = showMenu();
        year = menuData.year;
        day = menuData.day;
    }

    const std::vector<IAocDay*> days = {
        new Aoc2024D01(),
        new Aoc2024D02(),
        new Aoc2024D03(),
        new Aoc2024D04()
    };

    AocRegistry registry = AocRegistry(std::string(basedir));
    for (const auto aocDay : days)
    {
        if(year == -1 || (aocDay->getYear() == year && (day == -1 || aocDay->getDay() == day)))
        {
            registry.registerDay(aocDay);
        }
    }

    if(benchmark)
    {
        registry.runBenchmark();
    }
    else
    {
        registry.run();
    }

    for (auto value : days)
    {
        delete value;
    }
}
