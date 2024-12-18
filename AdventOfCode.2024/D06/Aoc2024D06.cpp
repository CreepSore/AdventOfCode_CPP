#include "pch.h"
#include "Aoc2024D06.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>

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
    int result = 0;

    Grid grid = Grid::fromString(*this->data);
    Vec2 currentPosition;
    Vec2 movementVector = Vec2::VEC_UP;

    for (std::pair<uint8_t, GridNode*> node : grid.nodes)
    {
        if(node.second->value == '^')
        {
            currentPosition = node.second->position;
            break;
        }
    }

    std::set<uint16_t> visited;
    while(grid.getNodeAt(currentPosition) != nullptr)
    {
        visited.insert(currentPosition.getHash());

        auto nextPosition = Vec2::add(currentPosition, movementVector);
        auto nextNode = grid.getNodeAt(nextPosition);

        if(nextNode == nullptr)
        {
            break;
        }

        if(nextNode->value == '#')
        {
            if(Vec2::equals(movementVector, Vec2::VEC_UP))
            {
                movementVector = Vec2::VEC_RIGHT;
            }
            else if(Vec2::equals(movementVector, Vec2::VEC_RIGHT))
            {
                movementVector = Vec2::VEC_DOWN;
            }
            else if (Vec2::equals(movementVector, Vec2::VEC_DOWN))
            {
                movementVector = Vec2::VEC_LEFT;
            }
            else if (Vec2::equals(movementVector, Vec2::VEC_LEFT))
            {
                movementVector = Vec2::VEC_UP;
            }

            continue;
        }

        currentPosition = nextPosition;
    }

    result = visited.size();

    return { result };
}

AocDayPartResult Aoc2024D06::runPart2() const
{
    int result = 0;

    return { result };
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
