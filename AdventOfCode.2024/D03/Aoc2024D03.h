#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"

class Aoc2024D03 : public IAocDay
{
private:
    AocDayPartResult runPart1() const;
    AocDayPartResult runPart1Alt() const;
    AocDayPartResult runPart2() const;

public:
    Aoc2024D03();
    ~Aoc2024D03() override;
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
};
