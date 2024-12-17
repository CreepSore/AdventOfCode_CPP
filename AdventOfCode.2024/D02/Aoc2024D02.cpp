#include "pch.h"
#include "Aoc2024D02.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>


Aoc2024D02::Aoc2024D02()
{
    this->day = 2;
    this->year = 2024;
}

Aoc2024D02::~Aoc2024D02()
{
    delete this->data;
}

AocDayPartResult Aoc2024D02::runPart1() const
{
    std::string buffer;
    int lastNum = -1;
    int currentNum = -1;
    int result = 0;
    bool skip = false;
    Directions direction = Directions::NOT_SET;

    for(unsigned int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);
        if(skip)
        {
            if(c == '\n')
            {
                skip = false;
                currentNum = -1;
                direction = Directions::NOT_SET;
            }

            continue;
        }

        lastNum = currentNum;

        if(c == '\n')
        {
            if(direction != Directions::INVALID)
            {
                result = result + 1;
            }

            currentNum = -1;
            direction = Directions::NOT_SET;

            continue;
        }

        if(c == '\r' || c == ' ')
        {
            currentNum = atoi(buffer.data());
            buffer.clear();

            if(lastNum == -1)
            {
                continue;
            }

            Directions actualDirection;

            if (lastNum < currentNum)
            {
                actualDirection = Directions::ASCENDING;
            }
            else if (lastNum > currentNum)
            {
                actualDirection = Directions::DESCENDING;
            }
            else
            {
                actualDirection = Directions::INVALID;
            }

            if(direction == Directions::NOT_SET)
            {
                direction = actualDirection;
            }

            if(direction != actualDirection || actualDirection == Directions::INVALID)
            {
                skip = true;
                continue;
            }

            int delta = abs(currentNum - lastNum);
            if(delta > 3)
            {
                skip = true;
            }

            continue;
        }

        if(c >= '0' && c <= '9')
        {
            buffer.push_back(c);
        }
    }
        
    return {result};
}

AocDayPartResult Aoc2024D02::runPart2() const
{
    // 459
    std::string buffer;
    Directions lastDirection = Directions::NOT_SET;
    int lastLastNum = -1;
    int lastNum = -1;
    int currentNum = -1;
    int result = 0;
    bool retry = true;
    std::map<Directions, int> dirCount;
    dirCount[Directions::ASCENDING] = 0;
    dirCount[Directions::DESCENDING] = 0;
    dirCount[Directions::INVALID] = 0;
    dirCount[Directions::SKIP] = 0;

    for (unsigned int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if (c == '\n')
        {
            currentNum = -1;

            int min = std::min(dirCount[Directions::ASCENDING], dirCount[Directions::DESCENDING]);

            if(
                (dirCount[Directions::INVALID] == 0 && min == 1)
                || (dirCount[Directions::INVALID] == 0 && min == 0))
            {
                result += 1;
            }

            dirCount[Directions::ASCENDING] = 0;
            dirCount[Directions::DESCENDING] = 0;
            dirCount[Directions::INVALID] = 0;
            dirCount[Directions::SKIP] = 0;
            retry = true;

            continue;
        }

        if (c == '\r' || c == ' ')
        {
            lastLastNum = lastNum;
            lastNum = currentNum;
            currentNum = atoi(buffer.data());
            buffer.clear();

            if (lastNum == -1)
            {
                continue;
            }

            Directions actualDirection;

            if (lastNum < currentNum)
            {
                actualDirection = Directions::ASCENDING;
            }
            else if (lastNum > currentNum)
            {
                actualDirection = Directions::DESCENDING;
            }
            else
            {
                actualDirection = Directions::INVALID;
            }

            if(abs(lastNum - currentNum) > 3)
            {
                actualDirection = Directions::INVALID;
            }

            if(actualDirection == Directions::INVALID && retry)
            {
                if(lastLastNum != -1)
                {
                    retry = false;

                    if (lastLastNum < currentNum)
                    {
                        actualDirection = Directions::ASCENDING;
                    }
                    else if (lastLastNum > currentNum)
                    {
                        actualDirection = Directions::DESCENDING;
                    }
                    else
                    {
                        actualDirection = Directions::INVALID;
                    }

                    if (abs(lastLastNum - currentNum) > 3)
                    {
                        actualDirection = Directions::INVALID;
                    }
                }
                else
                {
                    actualDirection = Directions::SKIP;
                }
            }

            dirCount[actualDirection] += 1;

            continue;
        }

        if (c >= '0' && c <= '9')
        {
            buffer.push_back(c);
        }
    }

    return { result };
}

AocDayPartResult Aoc2024D02::runPart2Alt() const
{
    struct Row
    {
        std::vector<int> nums;
    };

    std::string buffer;
    Row currentRow = Row();
    std::vector<Row> rows;

    for (unsigned int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if (c == '\n')
        {
            rows.push_back(currentRow);
            currentRow = Row();
            continue;
        }

        if (c == '\r' || c == ' ')
        {
            int currentNum = atoi(buffer.data());
            buffer.clear();

            currentRow.nums.push_back(currentNum);

            continue;
        }

        if (c >= '0' && c <= '9')
        {
            buffer.push_back(c);
        }
    }

    int result = 0;

    auto check = [](const std::vector<int>& nums) -> bool
    {
        Directions direction = Directions::NOT_SET;

        for(int i = 1; i < nums.size(); i++)
        {
            int prev = nums[i - 1];
            int curr = nums[i];

            if(std::abs(prev - curr) > 3)
            {
                return false;
            }

            Directions actualDirection;

            if(curr > prev)
            {
                actualDirection = Directions::ASCENDING;
            }
            else if(curr < prev)
            {
                actualDirection = Directions::DESCENDING;
            }
            else
            {
                return false;
            }

            if(direction == Directions::NOT_SET)
            {
                direction = actualDirection;
            }
            else if(actualDirection != direction)
            {
                return false;
            }
        }

        return true;
    };

    for (auto& [nums] : rows)
    {
        bool correct = false;

        for(int i = -1; i < nums.size(); i++)
        {
            nums = std::vector(nums);
            if(i == -1)
            {
                if(check(nums))
                {
                    correct = true;
                    break;
                }

                continue;
            }

            nums.erase(nums.begin() + i);
            if(check(nums))
            {
                correct = true;
                break;
            }
        }

        result += correct;
    }

    return {result};
}

void Aoc2024D02::run(std::vector<AocDayPartResult>& resultList, const int part)
{
    switch(part)
    {
    case -1:
        resultList.push_back(runPart1());
        resultList.push_back(runPart2Alt());
        break;

    case 1:
        resultList.push_back(runPart1());
        break;

    case 2:
        resultList.push_back(runPart2Alt());
        break;

    default:
        break;
    }
}
