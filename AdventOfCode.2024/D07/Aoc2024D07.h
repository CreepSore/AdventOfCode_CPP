#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"
#include "../Shared/Grid.h"

class Aoc2024D07 : public IAocDay
{
private:
    AocDayPartResult runPart1() const;
    AocDayPartResult runPart2() const;
    static std::vector<int> getPossibilities(int maxBits, std::map<int, std::vector<int>>* cache);

public:
    Aoc2024D07();
    ~Aoc2024D07() override;
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
};
