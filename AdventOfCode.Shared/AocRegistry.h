#pragma once
#include <vector>
#include <chrono>
#include <map>

#include "IAocDay.h"
#include "MultiLogger.h"

class AocRegistry
{
    std::string dataPath;

public:
    MultiLogger* logger = nullptr;
    std::vector<IAocDay*> days;
    AocRegistry(const std::string& dataPath);

    void registerDay(IAocDay* day);
    std::string* loadData(uint16_t year, uint8_t day) const;

    std::map<uint32_t, std::vector<AocDayPartResult>> run() const;
    std::map<uint32_t, std::vector<AocDayPartResult>> run(BaseWindow* window) const;
    std::vector<AocDayPartResult> runDay(IAocDay* day, BaseWindow* window = nullptr) const;
    void runBenchmark() const;
};
