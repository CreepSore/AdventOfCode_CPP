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

class AdventOfCode : IRenderable
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

    std::map<uint16_t, std::vector<IAocDay*>>* groupedDays = nullptr;
    std::map<uint32_t, std::vector<AocDayPartResult>> results;

    bool init = true;
    bool logViewerOpen = false;
    bool resultViewOpen = false;
    bool demoOpen = false;
    bool demoWasOpen = false;
    bool allChecked = false;
    bool anyChecked = false;
    bool runVisual = false;
    std::map<uint32_t, bool> checkedDays;
    StartParameters startParameters;

    void updateChecked()
    {
        for(IAocDay* day : this->registry->days)
        {
            if(!this->checkedDays[day->getId()])
            {
                allChecked = false;
                break;
            }

            allChecked = true;
        }

        for (IAocDay* day : this->registry->days)
        {
            if (this->checkedDays[day->getId()])
            {
                anyChecked = true;
                break;
            }

            anyChecked = false;
        }
    }

public:
    AocRegistry* registry = nullptr;
    BaseWindow* baseWindow = nullptr;
    MultiLogger* multiLogger = nullptr;
    VectorLogger* vectorLogger = nullptr;

    explicit AdventOfCode(StartParameters& startParameters)
    {
        this->startParameters = startParameters;
        this->registry = new AocRegistry(startParameters.basedir);

        this->multiLogger = new MultiLogger();
        this->vectorLogger = new VectorLogger();
        StdoutLogger* stdoutLogger = new StdoutLogger();

        multiLogger->appendLogger(stdoutLogger);
        multiLogger->appendLogger(vectorLogger);
        registry->logger = multiLogger;
    }

    ~AdventOfCode() override
    {
        delete this->registry;
        delete this->multiLogger;
        delete this->groupedDays;
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
        };

        for (const auto aocDay : days)
        {
            bool inTimeRange = startParameters.year == -1
                || (aocDay->getYear() == startParameters.year
                    && (startParameters.day == -1 || aocDay->getDay() == startParameters.day)
                );

            if (inTimeRange)
            {
                registry->registerDay(aocDay);
                this->checkedDays[aocDay->getId()] = false;
            }
        }

        if (startParameters.benchmark)
        {
            registry->runBenchmark();
        }
        else if (startParameters.visual)
        {
            this->runVisual = true;
            std::string title("AdventOfCode - C++");

            delete this->baseWindow;
            this->baseWindow = new BaseWindow(title);

            delete this->groupedDays;
            this->groupedDays = new std::map<uint16_t, std::vector<IAocDay*>>();
            
            for (auto aocDay : registry->days)
            {
                (*this->groupedDays)[aocDay->getYear()].push_back(aocDay);
            }

            while (this->baseWindow->window != nullptr)
            {
                this->baseWindow->addRenderable(this);
                this->baseWindow->tickOnce();
            }
        }
        else
        {
            this->results = registry->run();
        }

        for (auto value : days)
        {
            delete value;
        }
    }

    void render(BaseWindow* window) override
    {
        updateChecked();

        bool init = this->init;
        this->init = false;

        if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
        {
            this->demoOpen = !this->demoOpen;
        }

        if(this->demoOpen)
        {
            if(!this->demoWasOpen)
            {
                ImGui::SetNextWindowFocus();
            }

            ImGui::ShowDemoWindow(&this->demoOpen);
        }

        this->demoWasOpen = this->demoOpen;


        auto viewport = ImGui::GetMainViewport();

        if (logViewerOpen)
        {
            ImGui::SetNextWindowFocus();
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowPos(viewport->Pos);

            ImGui::Begin(
                "Log",
                0,
                ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize
            );

            if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                logViewerOpen = false;
                this->vectorLogger->data.clear();
            }

            if(logViewerOpen)
            {
                for (auto line : this->vectorLogger->data)
                {
                    ImGui::Text(line.data());
                }
            }

            ImGui::End();
        }

        if (resultViewOpen && !logViewerOpen)
        {
            ImGui::SetNextWindowFocus();
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowPos(viewport->Pos);

            ImGui::Begin(
                "Results",
                0,
                ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize
            );

            if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                resultViewOpen = false;
            }

            int columns = 2;
            int currentColumn = 0;
            float availableX = ImGui::GetContentRegionAvail().x / columns;
            std::vector<float> plotting;

            for(auto day : this->results)
            {
                std::string label = std::format("{} {}", day.first >> 8, day.first & 0xFF);

                ImGui::BeginChild(label.data(), ImVec2(availableX, 125), ImGuiChildFlags_Border);
                ImGui::Text(label.data());

                if(day.second.size() > 0)
                {
                    float partSizeX = ImGui::GetContentRegionAvail().x / day.second.size();

                    int part = 1;
                    for(int i = 0; i < day.second.size(); i++)
                    {
                        auto partResult = day.second[i];
                        std::string partLabel = std::format("Part {}", part);

                        ImGui::BeginChild(partLabel.data(), ImVec2(partSizeX, 0));
                        ImGui::Text(partLabel.data());
                        ImGui::Text("Result: ");
                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), std::to_string(partResult.result).data());
                        ImGui::SameLine();

                        if(ImGui::Button("Copy"))
                        {
                            ImGui::SetClipboardText(std::to_string(partResult.result).data());
                        }

                        ImGui::Text(std::format("Runtime: {}", durationToString(partResult.duration)).data());

                        ImGui::EndChild();

                        if(i != day.second.size() - 1)
                        {
                            ImGui::SameLine();
                        }

                        part += 1;
                        plotting.push_back(static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(partResult.duration).count()));
                    }
                }

                ImGui::EndChild();

                if (currentColumn++ != columns)
                {
                    ImGui::SameLine();
                }
                else
                {
                    currentColumn = 0;
                }
            }

            ImGui::NewLine();

            ImGui::PushID("Histogram");
            ImGui::PlotHistogram(
                "",
                plotting.data(),
                plotting.size(),
                0,
                "Runtimes us",
                0,
                100000,
                ImVec2(ImGui::GetContentRegionAvail().x, 500)
            );
            ImGui::PopID();
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


        bool allChecked = this->allChecked;
        if (ImGui::Checkbox("All checked", &allChecked))
        {
            for (IAocDay* day : this->registry->days)
            {
                this->checkedDays[day->getId()] = allChecked;
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Visual", &this->runVisual);

        ImGui::SameLine();

        ImGui::BeginDisabled(!this->anyChecked);
        if (ImGui::Button("Run", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
        {
            // We have to end here because days also render stuff
            ImGui::EndDisabled();
            ImGui::End();
            ImGui::EndFrame();
            window->removeRenderable(this);

            this->results.clear();

            for(auto day : this->registry->days)
            {
                if(checkedDays[day->getId()])
                {
                    if(this->runVisual)
                    {
                        this->results[day->getId()] = this->registry->runDay(day, window);
                    }
                    else
                    {
                        this->results[day->getId()] = this->registry->runDay(day, nullptr);
                    }
                }
            }

            logViewerOpen = this->vectorLogger->data.size() > 0;
            resultViewOpen = true;

            return;
        }
        ImGui::EndDisabled();

        for (auto year : *this->groupedDays)
        {
            if (init)
            {
                if(year.first == static_cast<int>(
                    std::chrono::year_month_day{
                        time_point_cast<std::chrono::days>(std::chrono::system_clock::now())
                    }.year()
                ))
                {
                    ImGui::SetNextItemOpen(true);
                }
            }

            if (ImGui::TreeNode(std::to_string(year.first).data()))
            {
                bool allCheckedDay = true;
                for(IAocDay* day : year.second)
                {
                    if(!this->checkedDays[day->getId()])
                    {
                        allCheckedDay = false;
                        break;
                    }
                }

                if (ImGui::Checkbox("All", &allCheckedDay))
                {
                    for (IAocDay* day : year.second)
                    {
                        this->checkedDays[day->getId()] = allCheckedDay;
                    }
                }

                for (auto day : year.second)
                {
                    std::string label = std::format("{} {}", day->getYear(), day->getDay());

                    ImGui::PushID(std::format("{}{}Checked", day->getYear(), day->getDay()).data());
                    ImGui::Checkbox("", &this->checkedDays[day->getId()]);
                    ImGui::PopID();

                    ImGui::SameLine();

                    if (day->getIsVisual())
                    {
                        ImVec4 old = ImGui::GetStyleColorVec4(ImGuiCol_Button);
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, old.z, 0, old.w));
                    }

                    if (ImGui::Button(label.data(), ImVec2(ImGui::GetContentRegionAvail().x - 100, 0)))
                    {
                        if (day->getIsVisual())
                        {
                            window->removeRenderable(this);
                            ImGui::PopStyleColor();
                        }

                        // We have to end here because days also render stuff
                        ImGui::TreePop();
                        ImGui::End();
                        ImGui::EndFrame();

                        this->results.clear();
                        this->results[day->getId()] = registry->runDay(day, window);
                        logViewerOpen = this->vectorLogger->data.size() > 0;
                        resultViewOpen = true;

                        return;
                    }

                    if (day->getIsVisual())
                    {
                        ImGui::PopStyleColor();
                    }

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

                ImGui::TreePop();
            }
        }

        ImGui::End();
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
