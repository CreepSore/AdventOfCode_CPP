#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"

class Aoc2024D05 : public IAocDay
{
private:
    AocDayPartResult runPart1() const;
    AocDayPartResult runPart2() const;
    AocDayPartResult runPart2Multithreaded() const;

public:
    Aoc2024D05();
    ~Aoc2024D05() override;
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
};
