#include "Aoc2024D05.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <numeric>
#include <set>

Aoc2024D05::Aoc2024D05()
{
    this->day = 5;
    this->year = 2024;
}

Aoc2024D05::~Aoc2024D05()
{
    delete this->data;
}

AocDayPartResult Aoc2024D05::runPart1() const
{
    int result = 0;
    std::map<uint8_t, std::vector<uint8_t>*> after;
    std::vector<std::vector<int>*> pages;
    pages.push_back(new std::vector<int>());

    uint8_t num0 = 0;
    std::string buffer;
    uint8_t readStep = 0;
    for(int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if(c == '\r')
        {
            continue;
        }

        if(readStep == 0 && c == '\n')
        {
            if (i >= 2 && this->data->at(i - 2) == '\n')
            {
                readStep = 1;
            }

            if(readStep == 0)
            {
                uint8_t num1 = std::stoi(buffer);
                buffer.clear();

                if(!after.contains(num0))
                {
                    after[num0] = new std::vector<uint8_t>();
                }

                after[num0]->push_back(num1);
            }

            continue;
        }

        if(readStep == 0 && c == '|')
        {
            num0 = std::stoi(buffer);
            buffer.clear();
            continue;
        }

        if(readStep == 1 && (c == ',' || c == '\n'))
        {
            uint8_t num1 = std::stoi(buffer);
            buffer.clear();
            
            std::vector<int>* currentPage = pages.at(pages.size() - 1);
            currentPage->push_back(num1);

            if (c == '\n')
            {
                pages.push_back(new std::vector<int>());
            }

            continue;
        }

        buffer.push_back(c);
    }


    for(int i = 0; i < pages.size(); i++)
    {
        auto page = pages.at(i);
        bool valid = page->size() > 0;
        std::set<int> scannedNumbers;

        for(int j = 0; j < page->size(); j++)
        {
            int num = page->at(j);
            if(after.contains(num))
            {
                for(const auto numBefore : *after[num])
                {
                    if(scannedNumbers.contains(numBefore))
                    {
                        valid = false;
                        break;
                    }
                }

                if (!valid)
                {
                    break;
                }
            }

            scannedNumbers.insert(num);
        }

        if(valid)
        {
            result += page->at(std::ceil(page->size() / 2));
        }
    }


    for (const std::pair<const unsigned char, std::vector<unsigned char>*> pair : after)
    {
        delete pair.second;
    }

    for(const auto page : pages)
    {
        delete page;
    }

    return { result };
}

AocDayPartResult Aoc2024D05::runPart2() const
{
    int result = 0;
    std::map<uint8_t, std::set<uint8_t>*> after;
    std::vector<std::vector<int>*> pages;
    pages.push_back(new std::vector<int>());

    uint8_t num0 = 0;
    std::string buffer;
    uint8_t readStep = 0;
    for (int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if (c == '\r')
        {
            continue;
        }

        if (readStep == 0 && c == '\n')
        {
            if (i >= 2 && this->data->at(i - 2) == '\n')
            {
                readStep = 1;
            }

            if (readStep == 0)
            {
                uint8_t num1 = std::stoi(buffer);
                buffer.clear();

                if (!after.contains(num0))
                {
                    after[num0] = new std::set<uint8_t>();
                }

                after[num0]->insert(num1);
            }

            continue;
        }

        if (readStep == 0 && c == '|')
        {
            num0 = std::stoi(buffer);
            buffer.clear();
            continue;
        }

        if (readStep == 1 && (c == ',' || c == '\n'))
        {
            uint8_t num1 = std::stoi(buffer);
            buffer.clear();

            std::vector<int>* currentPage = pages.at(pages.size() - 1);
            currentPage->push_back(num1);

            if (c == '\n')
            {
                pages.push_back(new std::vector<int>());
            }

            continue;
        }

        buffer.push_back(c);
    }


    for (int i = 0; i < pages.size(); i++)
    {
        auto page = pages.at(i);
        bool valid = page->size() > 0;
        struct ScannedNumber {
            uint8_t number;
            std::set<uint8_t>* after;

            ScannedNumber(int number, std::set<uint8_t>* after)
            {
                this->number = number;
                this->after = after;
            }
        };

        std::set<uint8_t> scannedNumbers;
        std::vector<ScannedNumber> nums;

        for (int j = 0; j < page->size(); j++)
        {
            int num = page->at(j);
            if (after.contains(num))
            {
                for (const auto numBefore : *after[num])
                {
                    if (scannedNumbers.contains(numBefore))
                    {
                        valid = false;
                    }
                }
            }

            scannedNumbers.insert(num);

            if(!after.contains(num))
            {
                after[num] = new std::set<uint8_t>();
            }

            nums.push_back(ScannedNumber(num, after[num]));
        }

        if (!valid && page->size() > 0)
        {
            std::sort(nums.begin(), nums.end(), [](ScannedNumber a, ScannedNumber b){
                return a.after->contains(b.number) || !b.after->contains(a.number);
            });
            result += nums.at(std::ceil(nums.size() / 2)).number;
        }
    }


    for (const std::pair<const unsigned char, std::set<unsigned char>*> pair : after)
    {
        delete pair.second;
    }

    for (const auto page : pages)
    {
        delete page;
    }

    return { result };
}


void Aoc2024D05::run(std::vector<AocDayPartResult>& resultList, int part)
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
