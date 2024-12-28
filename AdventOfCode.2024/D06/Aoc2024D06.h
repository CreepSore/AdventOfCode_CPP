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
        size_t visited;
        std::vector<Vec2> blocks;
    };

    struct RenderData
    {
        Grid* inGrid;
        int inDepth;
        std::set<uint32_t>* inVisitedPositions;

        bool paused = true;
        bool skip = false;
        int step = 0;
        int stepConfig = 1;
        bool changed = false;

        RenderData(Grid* inGrid, int inDepth, std::set<uint32_t>* inVisitedPositions)
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
        std::set<uint64_t>* inVisited,
        int depth = 0
    );

    RenderData* renderArgs = nullptr;
public:
    Aoc2024D06();
    ~Aoc2024D06() override;
    
    void run(std::vector<AocDayPartResult>& resultList, int part) override;
    void render(BaseWindow* window) override;
};
