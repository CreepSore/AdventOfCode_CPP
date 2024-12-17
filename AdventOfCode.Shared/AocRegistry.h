#pragma once
#include <vector>
#include <chrono>

#include "IAocDay.h"

class AocRegistry
{
    std::vector<IAocDay*> days;
    std::string dataPath;

    static std::string durationToString(std::chrono::steady_clock::duration duration);

public:
    AocRegistry(const std::string& dataPath);
    void registerDay(IAocDay* day);
    std::string* loadData(uint16_t year, uint8_t day) const;
    void run() const;
    void runBenchmark() const;
};
