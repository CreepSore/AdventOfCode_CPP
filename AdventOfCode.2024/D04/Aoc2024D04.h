#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"

class Aoc2024D04 : public IAocDay
{
private:
    AocDayPartResult runPart1() const;
    AocDayPartResult runPart2() const;
    AocDayPartResult runPart2Multithreaded() const;

public:
    Aoc2024D04();
    ~Aoc2024D04() override;
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
};
