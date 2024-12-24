#pragma once
#include <set>

#include "../../AdventOfCode.Shared/IAocDay.h"
#include "../Shared/Grid.h"

class Aoc2024D06 : public IAocDay
{
private:
    bool dataVisible = false;

    struct TraverseResult
    {
        bool looping;
        int visited;
        std::vector<Vec2> blocks;
    };

    struct Part1RenderData
    {
        Grid* inGrid;
        int inDepth;
        std::set<uint16_t>* inVisitedPositions;

        bool paused = true;
        bool skip = false;
        int step = 0;
        int stepConfig = 1;
        bool changed = false;

        Part1RenderData(Grid* inGrid, int inDepth, std::set<uint16_t>* inVisitedPositions)
        {
            this->inGrid = inGrid;
            this->inDepth = inDepth;
            this->inVisitedPositions = inVisitedPositions;
        };
    };

    AocDayPartResult runPart1();
    AocDayPartResult runPart2();

    TraverseResult traverseGrid(
        Grid& grid,
        const Vec2& startPosition,
        const Vec2& startDirection,
        std::set<uint32_t>* inVisited,
        int depth = 0
    );

    Part1RenderData* part1RenderData = nullptr;

    void renderPart1();
public:
    Aoc2024D06();
    ~Aoc2024D06() override;
    
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
    void render(BaseWindow* window) override;
};
