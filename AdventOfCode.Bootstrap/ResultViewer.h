#pragma once
#include <chrono>
#include <map>

#include "../AdventOfCode.Shared/utils.h"
#include "../AdventOfCode.Shared/AocDayPartResult.h"

class ResultViewer : public IRenderable
{
private:
    bool setFocus = true;
    std::map<uint32_t, std::vector<AocDayPartResult>>* results = nullptr;

public:
    bool open = true;

    void setResults(std::map<uint32_t, std::vector<AocDayPartResult>>* data)
    {
        results = data;
    }

    void renderPart(float partSizeX, int part, AocDayPartResult partResult) {
        std::string partLabel = std::format("Part {}", part);

        ImGui::BeginChild(partLabel.data(), ImVec2(partSizeX, 0));
        ImGui::Text(partLabel.data());
        ImGui::Text("Result: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), std::to_string(partResult.result).data());
        ImGui::SameLine();

        if (ImGui::Button("Copy"))
        {
            ImGui::SetClipboardText(std::to_string(partResult.result).data());
        }

        ImGui::Text(std::format("Runtime: {}", durationToString(partResult.duration)).data());

        ImGui::EndChild();
    }

    void renderDay(float availableX, std::vector<uint64_t>& plotting, std::map<uint32_t, std::vector<AocDayPartResult>>::value_type day) {
        std::string label = std::format("{} {}", day.first >> 8, day.first & 0xFF);

        ImGui::BeginChild(label.data(), ImVec2(availableX, 125), ImGuiChildFlags_Border);
        ImGui::Text(label.data());

        if (!day.second.empty())
        {
            float partSizeX = ImGui::GetContentRegionAvail().x / day.second.size();

            int part = 1;
            for (int i = 0; i < day.second.size(); i++)
            {
                auto partResult = day.second[i];
                renderPart(partSizeX, part, partResult);

                if (i != day.second.size() - 1)
                {
                    ImGui::SameLine();
                }

                part += 1;
                plotting.push_back(std::chrono::duration_cast<std::chrono::microseconds>(partResult.duration).count());
            }
        }

        ImGui::EndChild();
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

        if(results == nullptr)
        {
            return;
        }

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
            open = false;
        }

        int columns = 2;
        int currentColumn = 0;
        float availableX = ImGui::GetContentRegionAvail().x / columns;
        std::vector<uint64_t> plotting;

        for (auto day : *this->results)
        {
            renderDay(availableX, plotting, day);

            if (++currentColumn != columns)
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
        ImPlot::BeginPlot("Runtime us", ImVec2(ImGui::GetContentRegionAvail().x, 500));
        ImPlot::PlotBars("", plotting.data(), plotting.size());

        int offsetX = 0;
        for (auto& result : *this->results)
        {
            int partNo = 1;
            for(auto part : result.second)
            {
                ImPlot::Annotation(
                    offsetX,
                    0,
                    ImPlot::GetLastItemColor(),
                    ImVec2(0, -20),
                    false,
                    std::format(
                        "{}/D{:02}/P{:02}",
                        result.first >> 8,
                        result.first & 0xFF,
                        partNo
                    ).data()
                );
                ImPlot::Annotation(
                    offsetX,
                    0,
                    ImPlot::GetLastItemColor(),
                    ImVec2(0, -5),
                    false,
                    durationToString(part.duration).data()
                );

                partNo += 1;
                offsetX += 1;
            }
        }

        ImPlot::EndPlot();
        ImGui::PopID();
        ImGui::End();
    }
};
