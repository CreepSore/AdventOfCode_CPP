#pragma once
#include <format>
#include <string>
#include <imgui.h>

#include "../AdventOfCode.Visualization/IRenderable.h"
#include "../AdventOfCode.Shared/VectorLogger.h"

class LogViewer : public IRenderable
{
private:
    bool selfManaged = false;
    bool setFocus = true;
    bool shouldClear = false;
    bool autoClear = true;

public:
    bool open = true;
    std::vector<std::string>* data = nullptr;

    LogViewer() = default;

    LogViewer(std::vector<std::string>* data, bool copy = false) {
        this->setData(data, copy);
    }
    
    ~LogViewer() override {
        if (!selfManaged) {
            return;
        }

        delete this->data;
    }

    void setData(std::vector<std::string>* data, bool copy = false)
    {
        if (copy) {
            if(selfManaged)
            {
                delete this->data;
            }

            selfManaged = true;

            this->data = new std::vector<std::string>();

            for (std::string str : *data) {
                this->data->emplace_back(str);
            }

            return;
        }

        this->data = data;
    }

    void render(BaseWindow* window) override {
        if(!open)
        {
            setFocus = true;
            return;
        }

        if(setFocus)
        {
            ImGui::SetNextWindowFocus();
            setFocus = false;
        }

        if(!ImGui::Begin(
            "Log",
            &open,
            ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_MenuBar
        ))
        {
            ImGui::End();
            return;
        }

        if(ImGui::BeginMenuBar())
        {
            if(ImGui::MenuItem("Clear Log"))
            {
                this->data->clear();
            }

            ImGui::PushID("AUTOCLEAR");
            if (ImGui::MenuItem(std::format("Auto-Clear is {}", autoClear ? "On" : "Off").data()))
            {
                autoClear = !autoClear;
            }
            ImGui::PopID();

            ImGui::EndMenuBar();
        }

        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            open = false;

            if(autoClear)
            {
                this->data->clear();
            }
        }

        if (open && data != nullptr)
        {
            for (auto line : *data)
            {
                ImGui::Text(line.data());
            }
        }

        ImGui::End();
    }
};
