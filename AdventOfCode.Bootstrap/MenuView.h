#pragma once

#include "../AdventOfCode.Shared/AocRegistry.h"
#include "../AdventOfCode.Shared/IAocDay.h"

class MenuView : public IRenderable
{
private:
    bool demoOpen = false;
    bool demoWasOpen = false;
    bool allChecked = false;
    bool anyChecked = false;
    bool runVisual = false;
    std::map<uint16_t, std::vector<IAocDay*>>* groupedDays = nullptr;
    std::map<uint32_t, std::vector<AocDayPartResult>> results;
    std::map<uint32_t, bool> checkedDays;

    bool setFocus = true;
    bool isInitializing = true;
    LogViewer logViewer;
    ResultViewer resultViewer;
    AocRegistry* registry = nullptr;
    VectorLogger* vectorLogger = nullptr;

    void updateChecked()
    {
        for (IAocDay* day : this->registry->days)
        {
            if (!this->checkedDays[day->getId()])
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
    bool open = true;

    MenuView(AocRegistry* registry, VectorLogger* vectorLogger)
    {
        this->runVisual = true;
        this->registry = registry;
        this->vectorLogger = vectorLogger;
        this->groupedDays = new std::map<uint16_t, std::vector<IAocDay*>>();

        for (auto aocDay : registry->days)
        {
            (*this->groupedDays)[aocDay->getYear()].push_back(aocDay);
        }

        this->logViewer.setData(&vectorLogger->data, false);
        logViewer.open = false;
        resultViewer.open = false;

        for(IAocDay* day : this->registry->days)
        {
            this->checkedDays[day->getId()] = false;
        }
    }

    ~MenuView() override
    {
        delete this->groupedDays;
    }

    void handleDemoWindow() {
        if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
        {
            this->demoOpen = !this->demoOpen;
        }

        if (this->demoOpen)
        {
            if (!this->demoWasOpen)
            {
                ImGui::SetNextWindowFocus();
            }

            ImGui::ShowDemoWindow(&this->demoOpen);
        }

        this->demoWasOpen = this->demoOpen;
    }

    void onRunButtonClicked(BaseWindow* window) {
        window->removeRenderable(this);

        this->results.clear();

        for (auto day : this->registry->days)
        {
            if (checkedDays[day->getId()])
            {
                if (this->runVisual)
                {
                    this->results[day->getId()] = this->registry->runDay(day, window);
                }
                else
                {
                    this->results[day->getId()] = this->registry->runDay(day, nullptr);
                }
            }
        }

        this->logViewer.open = this->logViewer.data->size() > 0;
        resultViewer.setResults(&this->results);
        resultViewer.open = true;
    }

    void onCheckAllClicked(bool newChecked)
    {
        for (IAocDay* day : this->registry->days)
        {
            this->checkedDays[day->getId()] = newChecked;
        }
    }

    void onCheckFullYearClicked(std::vector<IAocDay*>& days, bool newChecked)
    {
        for (IAocDay* day : days)
        {
            this->checkedDays[day->getId()] = newChecked;
        }
    }

    void render(BaseWindow* window) override
    {
        if (!open)
        {
            setFocus = true;
            return;
        }

        if (setFocus)
        {
            ImGui::SetNextWindowFocus();
            setFocus = false;
        }

        updateChecked();

        bool init = this->isInitializing;
        this->isInitializing = false;

        handleDemoWindow();

        auto viewport = ImGui::GetMainViewport();

        if (logViewer.open)
        {
            ImGui::SetNextWindowFocus();
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowPos(viewport->Pos);

            logViewer.render(window);
        }

        if (resultViewer.open && !logViewer.open)
        {
            ImGui::SetNextWindowFocus();
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowPos(viewport->Pos);

            resultViewer.render(window);
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
            onCheckAllClicked(allChecked);
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
            onRunButtonClicked(window);

            return;
        }
        ImGui::EndDisabled();

        for (auto year : *this->groupedDays)
        {
            if (init)
            {
                if (year.first == static_cast<int>(
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
                for (IAocDay* day : year.second)
                {
                    if (!this->checkedDays[day->getId()])
                    {
                        allCheckedDay = false;
                        break;
                    }
                }

                if (ImGui::Checkbox("All", &allCheckedDay))
                {
                    onCheckFullYearClicked(year.second, allCheckedDay);
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
                        logViewer.open = this->vectorLogger->data.size() > 0;
                        resultViewer.setResults(&this->results);
                        resultViewer.open = true;

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
