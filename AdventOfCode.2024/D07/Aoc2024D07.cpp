#include "pch.h"
#include "Aoc2024D07.h"

#include <format>
#include <iostream>

#include "../Shared/Grid.h"

Aoc2024D07::Aoc2024D07()
{
    this->day = 7;
    this->year = 2024;
}

Aoc2024D07::~Aoc2024D07()
{
    delete this->data;
}

std::vector<int> Aoc2024D07::getPossibilities(int maxBits, std::map<int, std::vector<int>>* cache)
{
    if (cache->contains(maxBits))
    {
        return cache->at(maxBits);
    }

    std::vector<int> possibilities;
    int until = 0xFFFFFFFF >> (32 - maxBits);

    for (int i = 0; i <= until; i++)
    {
        possibilities.emplace_back(i);
    }

    cache->emplace(maxBits, possibilities);
    return possibilities;
}


AocDayPartResult Aoc2024D07::runPart1() const
{
    int64_t result = 0;
    std::vector<int64_t> currentNumbers;
    std::map<int, std::vector<int>> possibilityCache;
    std::string buffer;

    for(const char c : *this->data)
    {
        if(c == ':')
        {
            continue;
        }

        if (c == ' ' || c == '\r')
        {
            if(buffer.length() > 0)
            {
                currentNumbers.emplace_back(std::stoll(buffer));
                buffer.clear();
            }
            continue;
        }

        if (c == '\n')
        {
            bool valid = false;
            const int64_t expected = currentNumbers[0];
            const auto possibilities = getPossibilities(currentNumbers.size() - 1, &possibilityCache);

            for(const auto possibility : possibilities)
            {
                int64_t calcResult = 0;
                int64_t previousNum = currentNumbers[1];

                for(int j = 2; j < currentNumbers.size(); j++)
                {
                    const int64_t curr = currentNumbers[j];
                    int64_t currentCalcResult;

                    switch (possibility >> (j - 2) & 0x1)
                    {
                    case 0:
                        currentCalcResult = previousNum + curr;
                        break;

                    case 1:
                        currentCalcResult = previousNum * curr;
                        break;

                    default:
                        currentCalcResult = 0;
                        break;
                    }

                    calcResult = currentCalcResult;
                    previousNum = currentCalcResult;
                }

                if(calcResult == expected)
                {
                    result += expected;
                    break;
                }
            }

            currentNumbers.clear();
            continue;
        }

        buffer.push_back(c);
    }

    return { result };
}

AocDayPartResult Aoc2024D07::runPart2() const
{
    int result = 0;

    return { result };
}

void Aoc2024D07::run(std::vector<AocDayPartResult>& resultList, int part)
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
