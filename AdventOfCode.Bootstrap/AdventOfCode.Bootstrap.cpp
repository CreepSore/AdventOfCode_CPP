#include <iostream>

#include "imgui_internal.h"
#include "../AdventOfCode.2024/D01/Aoc2024D01.h"
#include "../AdventOfCode.2024/D02/Aoc2024D02.h"
#include "../AdventOfCode.2024/D03/Aoc2024D03.h"
#include "../AdventOfCode.2024/D04/Aoc2024D04.h"
#include "../AdventOfCode.2024/D05/Aoc2024D05.h"
#include "../AdventOfCode.2024/D06/Aoc2024D06.h"
#include "../AdventOfCode.2024/D07/Aoc2024D07.h"
#include "../AdventOfCode.Shared/AocRegistry.h"
#include "../AdventOfCode.Shared/MultiLogger.h"
#include "../AdventOfCode.Shared/utils.h"
#include "../AdventOfCode.Visualization/BaseWindow.h"
#include "../lib/imgui/imgui.h"
#include "../AdventOfCode.Shared/StdoutLogger.h"
#include "../AdventOfCode.Shared/VectorLogger.h"

#define IMGUI_USER_CONFIG "imgui_config.h"

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

    std::cout << "\nHello plsplsplspls input Day:\n> ";
    try
    {
        std::getline(std::cin, result);
        day = std::stoi(result);
    }
    catch(...)
    {
        day = -1;
    }

    return {year, day};
}

int main(int argc, char* argv[])
{
    std::string title = std::string("OIDA");

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
    bool visual = false;
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
            else if (arg == "-int")
            {
                interactive = true;
            }
            else if (arg == "-vis")
            {
                visual = true;
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
        new Aoc2024D04(),
        new Aoc2024D05(),
        new Aoc2024D06(),
        new Aoc2024D07(),
    };

    AocRegistry registry = AocRegistry(std::string(basedir));
    MultiLogger multiLogger;
    StdoutLogger stdoutLogger;
    VectorLogger vectorLogger;

    multiLogger.appendLogger(&stdoutLogger);
    multiLogger.appendLogger(&vectorLogger);
    registry.logger = &multiLogger;

    for (const auto aocDay : days)
    {
        bool inTimeRange = year == -1 || (aocDay->getYear() == year && (day == -1 || aocDay->getDay() == day));
        //bool isVisual = visual == aocDay->getIsVisual();

        if(inTimeRange /* && isVisual */)
        {
            registry.registerDay(aocDay);
        }
    }

    if(benchmark)
    {
        registry.runBenchmark();
    }
    else if(visual)
    {
        struct VisualArgs
        {
            bool ticking;
            AocRegistry* registry;
            BaseWindow* window;
        };


        auto window = BaseWindow(title);
        VisualArgs args{true, &registry, &window};
        bool tick = true;

        auto fn = [](VisualArgs* args) {
            auto viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowPos(viewport->Pos);

            if(!ImGui::Begin("Run Day", 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
            {
                ImGui::End();
                return;
            }

            if(ImGui::Button("All", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
            {
                // We have to end here because days also render stuff
                ImGui::End();
                args->registry->run(args->window);
                args->ticking = false;
                return;
            }

            for (auto day : args->registry->days)
            {
                std::string label;
                label.append(std::to_string(day->getYear()));
                label.append(" ");
                label.append(std::to_string(day->getDay()));

                ImGui::BeginDisabled(!day->getIsVisual());
                if(ImGui::Button(label.data(), ImVec2(ImGui::GetContentRegionAvail().x - 100, 0)))
                {
                    // We have to end here because days also render stuff
                    ImGui::EndDisabled();
                    ImGui::End();

                    args->registry->runDay(day, args->window);
                    args->ticking = false;
                    return;
                }
                ImGui::EndDisabled();

                ImGui::SameLine();
                ImGui::PushID(std::format("?{}", day->getId()).data());
                if(ImGui::Button("?", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
                {
                    openBrowser(std::format("https://adventofcode.com/{}/day/{}", day->getYear(), day->getDay()).data());
                }
                ImGui::PopID();

                ImGui::SameLine();
                ImGui::PushID(std::format("I{}", day->getId()).data());
                if (ImGui::Button("I", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    openBrowser(std::format("https://adventofcode.com/{}/day/{}/input", day->getYear(), day->getDay()).data());
                }
                ImGui::PopID();
            }

            ImGui::End();
        };

        while(args.ticking && window.window != nullptr)
        {
            window.tickOnce<VisualArgs>(+fn, &args);
        }

        if(window.window == nullptr)
        {
            window.tickOnce();
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
