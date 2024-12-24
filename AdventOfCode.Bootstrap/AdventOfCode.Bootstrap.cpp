#include <iostream>
#include <imgui.h>

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
#include "../AdventOfCode.Shared/StdoutLogger.h"
#include "../AdventOfCode.Shared/VectorLogger.h"
#include "../AdventOfCode.Visualization/PseudoRenderable.h"

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

PseudoRenderable<std::tuple<AocRegistry*, BaseWindow*, VectorLogger*>> constructRenderable(AocRegistry& registry, BaseWindow& window, VectorLogger& logger)
{
    auto args = std::make_tuple(
        &registry,
        &window,
        &logger
    );

    return PseudoRenderable<std::tuple<AocRegistry*, BaseWindow*, VectorLogger*>>([](auto thisRenderable, auto& args) {
        AocRegistry* registry = std::get<0>(args);
        BaseWindow* window = std::get<1>(args);
        VectorLogger* logger = std::get<2>(args);

        auto viewport = ImGui::GetMainViewport();

        if(logger->data.size() > 0)
        {
            ImGui::SetNextWindowFocus();
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowPos(viewport->Pos);

            bool open = true;
            ImGui::Begin(
                "Log",
                &open,
                ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize
            );

            if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                logger->data.clear();
            }

            if(!open)
            {
                logger->data.clear();
            }
            else
            {
                for (auto line : logger->data)
                {
                    ImGui::Text(line.data());
                }
            }

            ImGui::End();
        }

        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowPos(viewport->Pos);

        if (!ImGui::Begin(
            "Run Day",
            0,
            ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
        ))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Button("All", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
        {
            // We have to end here because days also render stuff
            ImGui::End();
            ImGui::EndFrame();
            window->removeRenderable(thisRenderable);

            registry->run(window);
            return;
        }

        for (auto day : registry->days)
        {
            std::string label;
            label.append(std::to_string(day->getYear()));
            label.append(" ");
            label.append(std::to_string(day->getDay()));

            if(day->getIsVisual())
            {
                ImVec4 old = ImGui::GetStyleColorVec4(ImGuiCol_Button);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, old.z, 0, old.w));
            }

            if (ImGui::Button(label.data(), ImVec2(ImGui::GetContentRegionAvail().x - 100, 0)))
            {
                if (day->getIsVisual())
                {
                    window->removeRenderable(thisRenderable);
                    ImGui::PopStyleColor();
                }

                // We have to end here because days also render stuff
                ImGui::End();
                ImGui::EndFrame();

                registry->runDay(day, window);

                return;
            }

            if (day->getIsVisual())
            {
                ImGui::PopStyleColor();
            }

            //ImGui::EndDisabled();

            ImGui::SameLine();
            ImGui::PushID(std::format("?{}", day->getId()).data());
            if (ImGui::Button("?", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
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
    }, args);
}

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

        if(inTimeRange)
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
        auto window = BaseWindow(title);
        ImGui::GetIO().IniFilename = nullptr;

        auto renderable = constructRenderable(registry, window, vectorLogger);

        while(window.window != nullptr)
        {
            window.addRenderable(&renderable);
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
