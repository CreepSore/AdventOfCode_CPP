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

AocDayPartResult Aoc2024D07::runPart1() const
{
    int64_t result = 0;
    std::vector<int64_t> currentNumbers;
    std::string buffer;

    auto getPossibilities = [](int maxBits)
    {
        std::vector<int> possibilities;
        int until = 0;

        for(int i = 0; i < maxBits; i++)
        {
            until |= 1 << i;
        }

        for(int i = 0; i <= until; i++)
        {
            possibilities.push_back(i);
        }

        return possibilities;
    };

    std::map<const char, int64_t(*)(int64_t, int64_t)> operatorFnMapping = {
        {0, [](int64_t num0, int64_t num1) -> int64_t {
            return num0 + num1;
        }},
        {1, [](int64_t num0, int64_t num1) -> int64_t {
            return num0 * num1;
        }}
    };

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
            int64_t expected = currentNumbers[0];
            auto possibilities = getPossibilities(currentNumbers.size() - 1);

            for(auto possibility : possibilities)
            {
                int64_t calcResult = 0;
                int64_t previousNum = currentNumbers[1];

                for(int j = 2; j < currentNumbers.size(); j++)
                {
                    auto toRun = operatorFnMapping[possibility >> (j - 2) & 0x1];
                    int64_t curr = currentNumbers[j];

                    auto currentCalcResult = toRun(previousNum, curr);

                    calcResult = currentCalcResult;
                    previousNum = currentCalcResult;
                }

                if(!valid)
                {
                    valid = calcResult == expected;

                    if(valid)
                    {
                        break;
                    }
                }
            }

            if(valid)
            {
                result += expected;
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
