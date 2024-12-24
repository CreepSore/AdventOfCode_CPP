#include "Aoc2024D04.h"

#include <iostream>
#include <thread>

#include "../Shared/Grid.h"

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
    const std::string toFind = "MAS";
    Grid grid = Grid::fromString(*this->data);
    const auto height = grid.getHeight();
    const auto width = grid.getWidth();

    int result = 0;

    const auto checkNode = [](Grid& grid, GridNode* node, const std::string& pattern)->uint8_t
    {
        size_t patternLength = pattern.length();

        if (node->value != pattern[1])
        {
            return 0;
        }

        const auto upLeft = grid.getNodeAt(Vec2::add(node->position, Vec2::VEC_UP_LEFT));
        const auto upRight = grid.getNodeAt(Vec2::add(node->position, Vec2::VEC_UP_RIGHT));
        const auto downLeft = grid.getNodeAt(Vec2::add(node->position, Vec2::VEC_DOWN_LEFT));
        const auto downRight = grid.getNodeAt(Vec2::add(node->position, Vec2::VEC_DOWN_RIGHT));

        if (
            upLeft == nullptr
            || upRight == nullptr
            || downLeft == nullptr
            || downRight == nullptr
        )
        {
            return 0;
        }

        const bool valid = (
            (upLeft->value == pattern[0] && downRight->value == pattern[2])
            || (upLeft->value == pattern[2] && downRight->value == pattern[0])
        ) && (
            (upRight->value == pattern[0] && downLeft->value == pattern[2])
            || (upRight->value == pattern[2] && downLeft->value == pattern[0])
        );

        return valid;
    };

    // Offset because it's impossible to construct a cross
    // at the edges
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            GridNode* node = grid.getNodeAt(Vec2(x, y));
            if (node == nullptr)
            {
                std::cout << "No Node at " << Vec2(x, y) << "\n";
                continue;
            }

            result += checkNode(grid, node, toFind);
        }
    }

    return { result };
}

AocDayPartResult Aoc2024D04::runPart2Multithreaded() const
{
    Grid grid = Grid::fromString(*this->data);
    const auto height = grid.getHeight();
    const auto width = grid.getWidth();

    std::map<std::thread::id, int*> threadResults;
    std::vector<std::thread> threads;

    auto threadFn = [](int* result, Grid* grid, std::vector<GridNode*>* nodes)->void
        {
            const std::string pattern = "MAS";

            for (const auto node : *nodes)
            {
                if (node->value != pattern[1])
                {
                    continue;
                }

                const auto upLeft = grid->getNodeAt(Vec2::add(node->position, Vec2::VEC_UP_LEFT));
                const auto upRight = grid->getNodeAt(Vec2::add(node->position, Vec2::VEC_UP_RIGHT));
                const auto downLeft = grid->getNodeAt(Vec2::add(node->position, Vec2::VEC_DOWN_LEFT));
                const auto downRight = grid->getNodeAt(Vec2::add(node->position, Vec2::VEC_DOWN_RIGHT));

                if (
                    upLeft == nullptr
                    || upRight == nullptr
                    || downLeft == nullptr
                    || downRight == nullptr
                    )
                {
                    continue;
                }

                const bool valid = (
                    (upLeft->value == pattern[0] && downRight->value == pattern[2])
                    || (upLeft->value == pattern[2] && downRight->value == pattern[0])
                ) && (
                    (upRight->value == pattern[0] && downLeft->value == pattern[2])
                    || (upRight->value == pattern[2] && downLeft->value == pattern[0])
                );

                *result += valid;
            }

            delete nodes;
        };

    int totalNodes = width * height;
    // Offset because it's impossible to construct a cross
    // at the edges
    std::vector<GridNode*>* nodes = new std::vector<GridNode*>();
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            GridNode* node = grid.getNodeAt(Vec2(x, y));
            if (node == nullptr)
            {
                std::cout << "No Node at " << Vec2(x, y) << "\n";
                continue;
            }

            nodes->push_back(node);

            if (nodes->size() > totalNodes / 16)
            {
                int* threadResult = new int(0);
                auto thread = std::thread(threadFn, threadResult, &grid, nodes);
                threadResults[thread.get_id()] = threadResult;
                threads.push_back(std::move(thread));
                nodes = new std::vector<GridNode*>();
            }
        }
    }

    if (nodes->size() > 0)
    {
        int* threadResult = new int(0);
        auto thread = std::thread(threadFn, threadResult, &grid, nodes);
        threadResults[thread.get_id()] = threadResult;
        threads.push_back(std::move(thread));
    }
    else
    {
        delete nodes;
    }

    int result = 0;

    for (std::thread& thread : threads)
    {
        auto threadId = thread.get_id();
        if(thread.joinable())
        {
            thread.join();
        }

        const int* threadResult = threadResults[threadId];

        result += *threadResult;
        delete threadResult;
    }

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
