#include "pch.h"
#include "Aoc2024D04.h"

#include <iostream>

#include "Grid.h"

Aoc2024D04::Aoc2024D04()
{
    this->day = 4;
    this->year = 2024;
}

Aoc2024D04::~Aoc2024D04()
{
    delete this->data;
}

AocDayPartResult Aoc2024D04::runPart1() const
{
    const std::string toFind = "XMAS";
    Grid grid = Grid::fromString(*this->data);
    const auto height = grid.getHeight();
    const auto width = grid.getWidth();

    int result = 0;

    const auto checkOffset = [](Grid& grid, const Vec2& position, const char c)->bool
    {
        const GridNode* currentNode = grid.getNodeAt(position);
        return currentNode != nullptr && currentNode->value == c;
    };

    const auto checkNode = [checkOffset](Grid& grid, GridNode* node, const std::string& pattern)->uint8_t
    {
        size_t patternLength = pattern.length();
        uint8_t result = 0;

        if(node->value != pattern[0])
        {
            return 0;
        }

        bool found = true;
        for(int x = 1; x < patternLength; x++)
        {
            if(!checkOffset(grid, Vec2(node->position.x + x, node->position.y), pattern[x]))
            {
                found = false;
                break;
            }
        }

        if(found)
        {
            result += 1;
        }

        found = true;
        for (int x = 1; x < patternLength; x++)
        {
            if (!checkOffset(grid, Vec2(node->position.x - x, node->position.y), pattern[x]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        found = true;
        for (int y = 1; y < patternLength; y++)
        {
            if (!checkOffset(grid, Vec2(node->position.x, node->position.y + y), pattern[y]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        found = true;
        for (int y = 1; y < patternLength; y++)
        {
            if (!checkOffset(grid, Vec2(node->position.x, node->position.y - y), pattern[y]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        found = true;
        for (int off = 1; off < patternLength; off++)
        {
            if (!checkOffset(grid, Vec2(node->position.x + off, node->position.y + off), pattern[off]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        found = true;
        for (int off = 1; off < patternLength; off++)
        {
            if (!checkOffset(grid, Vec2(node->position.x + off, node->position.y - off), pattern[off]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        found = true;
        for (int off = 1; off < patternLength; off++)
        {
            if (!checkOffset(grid, Vec2(node->position.x - off, node->position.y + off), pattern[off]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        found = true;
        for (int off = 1; off < patternLength; off++)
        {
            if (!checkOffset(grid, Vec2(node->position.x - off, node->position.y - off), pattern[off]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result += 1;
        }

        return result;
    };

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            GridNode* node = grid.getNodeAt(Vec2(x, y));
            if(node == nullptr)
            {
                std::cout << "No Node at " << Vec2(x, y) << "\n";
                continue;
            }

            result += checkNode(grid, node, toFind);
        }
    }

    return { result };
}

AocDayPartResult Aoc2024D04::runPart2() const
{
    int result = 0;

    return { result };
}

void Aoc2024D04::run(std::vector<AocDayPartResult>& resultList, int part)
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
