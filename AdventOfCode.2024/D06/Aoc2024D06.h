#pragma once
#include <set>

#include "../../AdventOfCode.Shared/IAocDay.h"
#include "../Shared/Grid.h"

class Aoc2024D06 : public IAocDay
{
private:
    struct TraverseResult
    {
        bool looping;
        int visited;
        std::vector<Vec2> blocks;
    };

    AocDayPartResult runPart1() const;
    AocDayPartResult runPart2() const;

    static TraverseResult traverseGrid(
        Grid& grid,
        const Vec2& startPosition,
        const Vec2& startDirection,
        std::set<uint32_t>* inVisited,
        int depth = 0
    );

public:
    Aoc2024D06();
    ~Aoc2024D06() override;
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
};
