#pragma once
#include "../../AdventOfCode.Shared/IAocDay.h"
#include "../Shared/Grid.h"

class Aoc2024D08 : public IAocDay
{
private:
    struct Part1RenderArgs
    {
        bool valid = false;
        Grid* grid;
        std::map<uint8_t, ImVec4> colorMapping;
    };

    AocDayPartResult runPart1();
    AocDayPartResult runPart2();

    Part1RenderArgs part1RenderArgs;

public:
    Aoc2024D08();
    ~Aoc2024D08() override;
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
    void render(BaseWindow* window) override;
};
