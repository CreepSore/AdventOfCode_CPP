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
            if(!buffer.empty())
            {
                currentNumbers.emplace_back(std::stoll(buffer));
                buffer.clear();
            }
            continue;
        }

        if (c == '\n')
        {
            const size_t lineSize = currentNumbers.size();
            const int64_t expected = currentNumbers[0];
            const int until = 0xFFFFFFFF >> (32 - lineSize - 1);

            for(int possibility = 0; possibility < until; possibility++)
            {
                int64_t calcResult = 0;

                for(int j = 2; j < lineSize; j++)
                {
                    int64_t previousNum = currentNumbers[j - 1];
                    const int64_t curr = currentNumbers[j];
                    const int operation = possibility >> (j - 2) & 0x1;
                    int64_t currentCalcResult;

                    switch (operation)
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
    int64_t result = 0;

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
