#pragma once
#include <string>
#include <vector>

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
    uint32_t getId() const;
    uint8_t getDay() const;
    uint16_t getYear() const;
    uint8_t getPartCount() const;
    bool getIsVisual() const;

    void initialize(const std::string& data);
    void initialize(const std::string& data, BaseWindow* window);
    void deinit();
    void handleVisual();
    virtual void render(BaseWindow* window) override;

    virtual ~IAocDay();
    virtual void run(std::vector<AocDayPartResult>& resultList, int part = -1) = 0;
};
