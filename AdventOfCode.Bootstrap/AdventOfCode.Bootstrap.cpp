#include <iostream>
#include <imgui.h>

#include "MenuView.h"
#include "../AdventOfCode.2024/D01/Aoc2024D01.h"
#include "../AdventOfCode.2024/D02/Aoc2024D02.h"
#include "../AdventOfCode.2024/D03/Aoc2024D03.h"
#include "../AdventOfCode.2024/D04/Aoc2024D04.h"
#include "../AdventOfCode.2024/D05/Aoc2024D05.h"
#include "../AdventOfCode.2024/D06/Aoc2024D06.h"
#include "../AdventOfCode.2024/D07/Aoc2024D07.h"
#include "../AdventOfCode.2024/D08/Aoc2024D08.h"
#include "../AdventOfCode.Shared/AocRegistry.h"
#include "../AdventOfCode.Shared/MultiLogger.h"
#include "../AdventOfCode.Shared/utils.h"
#include "../AdventOfCode.Visualization/BaseWindow.h"
#include "../AdventOfCode.Shared/StdoutLogger.h"
#include "../AdventOfCode.Shared/VectorLogger.h"
#include "../AdventOfCode.Visualization/PseudoRenderable.h"

#define IMGUI_USER_CONFIG "imgui_config.h"

class AdventOfCode
{
public:
    struct StartParameters
    {
        std::string basedir;
        bool benchmark = false;
        bool interactive = false;
        bool visual = false;
        int day = -1;
        int year = -1;
    };

private:
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

    bool init = true;
    StartParameters startParameters;

public:
    AocRegistry registry;
    BaseWindow* baseWindow = nullptr;
    MenuView* menuView = nullptr;
    MultiLogger multiLogger;
    VectorLogger* vectorLogger = new VectorLogger();
    StdoutLogger* stdoutLogger = new StdoutLogger();

    explicit AdventOfCode(StartParameters& startParameters):
        registry(startParameters.basedir)
    {
        this->startParameters = startParameters;

        multiLogger.appendLogger(stdoutLogger);
        multiLogger.appendLogger(vectorLogger);
        registry.logger = &multiLogger;
    }

    ~AdventOfCode()
    {
        delete this->baseWindow;
    }

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
            catch (...)
            {
                year = static_cast<int>(
                    std::chrono::year_month_day{ time_point_cast<std::chrono::days>(std::chrono::system_clock::now()) }
                    .year()
                    );

                std::cout << std::to_string(year) << '\n';
            }
        } while (year == -1);

        std::cout << "\nHello plsplsplspls input Day:\n> ";
        try
        {
            std::getline(std::cin, result);
            day = std::stoi(result);
        }
        catch (...)
        {
            day = -1;
        }

        return { year, day };
    }

    void run()
    {
        if (startParameters.interactive)
        {
            auto menuData = showMenu();
            startParameters.year = menuData.year;
            startParameters.day = menuData.day;
        }

        const std::vector<IAocDay*> days = {
            new Aoc2024D01(),
            new Aoc2024D02(),
            new Aoc2024D03(),
            new Aoc2024D04(),
            new Aoc2024D05(),
            new Aoc2024D06(),
            new Aoc2024D07(),
            new Aoc2024D08(),
        };

        for (const auto aocDay : days)
        {
            bool inTimeRange = startParameters.year == -1
                || (aocDay->getYear() == startParameters.year
                    && (startParameters.day == -1 || aocDay->getDay() == startParameters.day)
                );

            if (inTimeRange)
            {
                registry.registerDay(aocDay);
            }
        }

        if (startParameters.benchmark)
        {
            registry.runBenchmark();
        }
        else if (startParameters.visual)
        {
            std::string title("AdventOfCode - C++");

            this->baseWindow = new BaseWindow(title);
            this->menuView = new MenuView(&this->registry, this->vectorLogger);

            while (this->baseWindow->window != nullptr)
            {
                this->baseWindow->addRenderable(menuView);
                this->baseWindow->tickOnce();
            }
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
};

int main(int argc, char* argv[])
{
    std::string title = std::string("AdventOfCode - C++");

    enum class ArgParseModes : uint8_t
    {
        NONE=0,
        BASEDIR=1,
        DAY=2,
        YEAR=3
    };

    AdventOfCode::StartParameters startParameters;
    ArgParseModes parseMode = ArgParseModes::NONE;

    for(int i = 1; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);

        switch (parseMode)
        {
        case ArgParseModes::NONE:
            if (arg == "-benchmark")
            {
                startParameters.benchmark = true;
            }
            else if (arg == "-int")
            {
                startParameters.interactive = true;
            }
            else if (arg == "-vis")
            {
                startParameters.visual = true;
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

            break;

        case ArgParseModes::BASEDIR:
            startParameters.basedir = arg;
            parseMode = ArgParseModes::NONE;
            break;

        case ArgParseModes::DAY:
            startParameters.day = std::stoi(arg);
            parseMode = ArgParseModes::NONE;
            break;

        case ArgParseModes::YEAR:
            startParameters.year = std::stoi(arg);
            parseMode = ArgParseModes::NONE;
            break;
        }
    }

    AdventOfCode adventOfCode(startParameters);
    adventOfCode.run();
}
