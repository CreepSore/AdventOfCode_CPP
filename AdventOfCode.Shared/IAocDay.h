#pragma once
#include <string>
#include <vector>
#include <imgui.h>

#include "AocDayPartResult.h"
#include "../AdventOfCode.Visualization/BaseWindow.h"

class IAocDay : IRenderable
{
protected:
    std::string* data = nullptr;
    uint8_t day = -1;
    uint16_t year = -1;
    uint8_t parts = 2;
    BaseWindow* window = nullptr;
    bool isVisual = false;

public:
    uint32_t getId() const
    {
        return (static_cast<uint32_t>(this->year) << 8) | this->day;
    }

    uint8_t getDay() const
    {
        return this->day;
    }

    uint16_t getYear() const
    {
        return this->year;
    }

    uint8_t getPartCount() const
    {
        return this->parts;
    }

    bool getIsVisual() const
    {
        return this->isVisual;
    }

    void initialize(const std::string& data);
    void initialize(const std::string& data, BaseWindow* window);
    void deinit();
    void handleVisual() const;
    void render(BaseWindow* window) override;
    void renderData(bool* p_open = 0, ImGuiWindowFlags flags = 0) const;

    virtual ~IAocDay();
    virtual void run(std::vector<AocDayPartResult>& resultList, int part = -1) = 0;
};
