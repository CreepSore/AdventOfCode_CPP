#pragma once
#include <vector>

#include "IAocDay.h"

class AocRegistry
{
    std::vector<IAocDay*> days;
    std::string dataPath;

public:
    AocRegistry(const std::string& dataPath);
    void registerDay(IAocDay* day);
    std::string* loadData(uint16_t year, uint8_t day) const;
    void run() const;
};
