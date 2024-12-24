#pragma once
#include <vector>
#include <chrono>

#include "IAocDay.h"
#include "MultiLogger.h"

class AocRegistry
{
    std::string dataPath;
    static std::string durationToString(std::chrono::steady_clock::duration duration);

public:
    MultiLogger* logger = nullptr;
    std::vector<IAocDay*> days;
    AocRegistry(const std::string& dataPath);

    void registerDay(IAocDay* day);
    std::string* loadData(uint16_t year, uint8_t day) const;

    void run() const;
    void run(BaseWindow* window) const;
    void runDay(IAocDay* day, BaseWindow* window = nullptr) const;
    void runBenchmark() const;
};
