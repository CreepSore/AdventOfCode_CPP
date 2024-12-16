#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"

class Aoc2024D01 : public IAocDay
{
private:
    AocDayPartResult runPart1() const;
    AocDayPartResult runPart2() const;

public:
    Aoc2024D01();
    ~Aoc2024D01() override;

    void run(std::vector<AocDayPartResult>& resultList, int part = -1) override;
};
