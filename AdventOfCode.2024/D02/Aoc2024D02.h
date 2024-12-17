#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"

class Aoc2024D02 : public IAocDay
{
private:
    AocDayPartResult runPart1() const;
    AocDayPartResult runPart2() const;
    AocDayPartResult runPart2Alt() const;

    enum class Directions : uint8_t
    {
        NOT_SET = 0,
        ASCENDING = 1,
        DESCENDING = 2,
        SKIP = 3,
        INVALID = 99
    };
public:
    Aoc2024D02();
    ~Aoc2024D02() override;

    void run(std::vector<AocDayPartResult>& resultList, int part = -1) override;
};
