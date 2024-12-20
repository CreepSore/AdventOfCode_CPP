#include "pch.h"
#include "Aoc2024D06.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <thread>

#include "../Shared/Grid.h"

Aoc2024D06::Aoc2024D06()
{
    this->day = 6;
    this->year = 2024;
}

Aoc2024D06::~Aoc2024D06()
{
    delete this->data;
}

AocDayPartResult Aoc2024D06::runPart1() const
{
    static std::map<uint16_t, Vec2> rotation = {
        { Vec2::VEC_UP.getHash(), Vec2::VEC_RIGHT },
        { Vec2::VEC_RIGHT.getHash(), Vec2::VEC_DOWN },
        { Vec2::VEC_DOWN.getHash(), Vec2::VEC_LEFT },
        { Vec2::VEC_LEFT.getHash(), Vec2::VEC_UP }
    };

    int result = 0;

    Grid grid = Grid::fromString(*this->data);
    Vec2 startPosition;

    for (std::pair<uint8_t, GridNode*> node : grid.nodes)
    {
        if (node.second->value == '^')
        {
            startPosition = node.second->position;
            break;
        }
    }

    result = traverseGrid(grid, startPosition, Vec2::VEC_UP, nullptr, 0).visited;
    
    return { result };
}

AocDayPartResult Aoc2024D06::runPart2() const
{
    static std::map<uint16_t, Vec2> rotation = {
        { Vec2::VEC_UP.getHash(), Vec2::VEC_RIGHT },
        { Vec2::VEC_RIGHT.getHash(), Vec2::VEC_DOWN },
        { Vec2::VEC_DOWN.getHash(), Vec2::VEC_LEFT },
        { Vec2::VEC_LEFT.getHash(), Vec2::VEC_UP }
    };

    int result = 0;

    Grid visualGrid = Grid::fromString(*this->data);
    Grid grid = Grid::fromString(*this->data);
    Vec2 startPosition;

    for (std::pair<uint8_t, GridNode*> node : grid.nodes)
    {
        if (node.second->value == '^')
        {
            startPosition = node.second->position;
            break;
        }
    }

    auto traversed = traverseGrid(grid, startPosition, Vec2::VEC_UP, nullptr, 1);

    for (auto block : traversed.blocks)
    {
        if(!Vec2::equals(block, startPosition))
        {
            result += 1;
        }
    }

    return { result };
}

Aoc2024D06::TraverseResult Aoc2024D06::traverseGrid(
    Grid& grid, 
    const Vec2& startPosition, 
    const Vec2& startDirection,
    std::set<uint32_t>* inVisited,
    int depth
)
{
    TraverseResult result;
    result.blocks = std::vector<Vec2>();
    result.visited = 1;

    static std::map<uint16_t, Vec2> rotation = {
        { Vec2::VEC_UP.getHash(), Vec2::VEC_RIGHT },
        { Vec2::VEC_RIGHT.getHash(), Vec2::VEC_DOWN },
        { Vec2::VEC_DOWN.getHash(), Vec2::VEC_LEFT },
        { Vec2::VEC_LEFT.getHash(), Vec2::VEC_UP }
    };

    std::set<uint16_t> visitedPositions;
    std::set<uint32_t>* visited = new std::set<uint32_t>();

    if(inVisited != nullptr)
    {
        for (uint32_t value : *inVisited)
        {
            visited->insert(value);
        }
    }

    GridNode* currentNode = grid.getNodeAt(startPosition);
    Vec2 currentDirection = startDirection;

    if(currentNode == nullptr)
    {
        result.looping = false;
        return result;
    }

    for(;;)
    {
        const uint16_t currentPosHash = currentNode->position.getHash();
        const uint16_t currentDirHash = currentDirection.getHash();
        const uint32_t mergedHash = (currentPosHash << 16 | currentDirHash);

        if (visited->contains(mergedHash))
        {
            result.looping = true;
            break;
        }

        visitedPositions.insert(currentPosHash);
        visited->insert(mergedHash);

        GridNode* nextNode = grid.getNodeAt(Vec2::add(currentNode->position, currentDirection));

        if(nextNode == nullptr)
        {
            result.looping = false;
            break;
        }

        if(nextNode->value == '#')
        {
            currentDirection = rotation[currentDirHash];
            continue;
        }

        if(depth > 0 && !visitedPositions.contains(nextNode->position.getHash()))
        {
            const char oldValue = nextNode->value;
            nextNode->value = '#';
            auto traversed = traverseGrid(
                grid,
                Vec2::add(currentNode->position, rotation[currentDirHash]),
                rotation[currentDirHash],
                visited,
                depth - 1
            );
            nextNode->value = oldValue;

            if(traversed.looping)
            {
                result.blocks.push_back(nextNode->position);
            }
            else
            {
                for (auto block : traversed.blocks)
                {
                    result.blocks.push_back(block);
                }
            }
        }

        currentNode = nextNode;
    }

    result.visited = visitedPositions.size();
    delete visited;
    return result;
}


void Aoc2024D06::run(std::vector<AocDayPartResult>& resultList, int part)
{
    switch (part)
    {
    case -1:
        resultList.push_back(runPart1());
        resultList.push_back(runPart2());
        break;

    case 1:
        resultList.push_back(runPart1());
        break;

    case 2:
        resultList.push_back(runPart2());
        break;

    default:
        break;
    }
}
