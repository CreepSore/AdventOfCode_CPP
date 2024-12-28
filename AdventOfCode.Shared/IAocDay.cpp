#include "IAocDay.h"

IAocDay::~IAocDay()
{
    this->deinit();
}

void IAocDay::initialize(const std::string& data)
{
    this->data = new std::string(data);
    this->window = nullptr;
}

void IAocDay::initialize(const std::string& data, BaseWindow* window)
{
    this->initialize(data);
    this->window = window;

    if (this->isVisual)
    {
        window->addRenderable(this);
    }
}

void IAocDay::deinit()
{
    if(this->isVisual && this->window != nullptr)
    {
        this->window->removeRenderable(this);
    }
}

void IAocDay::handleVisual() const
{
    if(this->window == nullptr)
    {
        return;
    }

    this->window->tickOnce();
}

void IAocDay::render(BaseWindow* window)
{
    // Do nothing by default
}

void IAocDay::renderData(bool* p_open, ImGuiWindowFlags flags) const
{
    if(!ImGui::Begin("Data", p_open, flags))
    {
        ImGui::End();
        return;
    }

    std::string line;

    for(const char c : *this->data)
    {
        if(c == '\n')
        {
            ImGui::Text(line.data());
            line.clear();
            continue;
        }

        line.push_back(c);
    }

    if(line.empty())
    {
        ImGui::Text(line.data());
    }

    ImGui::End();
}
