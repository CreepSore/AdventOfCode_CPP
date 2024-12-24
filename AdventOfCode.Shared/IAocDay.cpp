#include "pch.h"
#include "IAocDay.h"

IAocDay::~IAocDay()
{
    this->deinit();
}

uint8_t IAocDay::getDay() const
{
    return this->day;
}

uint16_t IAocDay::getYear() const
{
    return this->year;
}

uint8_t IAocDay::getPartCount() const
{
    return this->parts;
}

bool IAocDay::getIsVisual() const
{
    return this->isVisual;
}

uint32_t IAocDay::getId() const
{
    uint32_t result = (static_cast<uint32_t>(this->year) << 8) | this->day;

    return result;
}

void IAocDay::initialize(const std::string& data)
{
    this->data = new std::string(data);
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
    if(this->isVisual)
    {
        this->window->removeRenderable(this);
    }
}

void IAocDay::handleVisual()
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
