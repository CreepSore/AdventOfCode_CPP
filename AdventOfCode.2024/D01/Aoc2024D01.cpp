#include "pch.h"
#include "Aoc2024D01.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>


Aoc2024D01::Aoc2024D01()
{
    this->day = 1;
    this->year = 2024;
}

Aoc2024D01::~Aoc2024D01()
{
    delete this->data;
}

AocDayPartResult Aoc2024D01::runPart1() const
{
    std::vector<int> list1;
    std::vector<int> list2;

    std::string buffer;

    for(unsigned int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if(c == '\n')
        {
            continue;
        }

        if(c == '\r')
        {
            list2.push_back(atoi(buffer.data()));
            buffer.clear();
            continue;
        }

        if(c == ' ' && !buffer.empty())
        {
            list1.push_back(atoi(buffer.data()));
            buffer.clear();
            continue;
        }

        if(c >= '0' && c <= '9')
        {
            buffer.push_back(c);
        }
    }

    std::sort(
        list1.begin(), 
        list1.end(),
        [](int a, int b) -> bool {
            return a < b;
        }
    );

    std::sort(
        list2.begin(), 
        list2.end(), 
        [](int a, int b) -> bool {
            return a < b;
        }
    );

    int result = 0;
    int size = list1.size();

    for(unsigned int i = 0; i < size; i++)
    {
        result += abs(list1[i] - list2[i]);
    }
        
    return {result};
}

AocDayPartResult Aoc2024D01::runPart2() const
{
    std::vector<int> list;
    std::map<int, int> map;

    std::string buffer;

    for (unsigned int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if(c == '\n')
        {
            continue;
        }

        if (c == '\r')
        {
            int num = atoi(buffer.data());
            buffer.clear();

            if(!map.contains(num))
            {
                map[num] = 0;
            }

            map[num] = map[num] + 1;
            continue;
        }

        if (c == ' ' && !buffer.empty())
        {
            list.push_back(atoi(buffer.data()));
            buffer.clear();
            continue;
        }

        if (c >= '0' && c <= '9')
        {
            buffer.push_back(c);
        }
    }

    int result = 0;

    for (unsigned int i = 0; i < list.size(); i++)
    {
        if(map.contains(list[i]))
        {
            result += list[i] * map[list[i]];
        }
    }

    return {result};
}

void Aoc2024D01::run(std::vector<AocDayPartResult>& resultList, int part)
{

    switch(part)
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
