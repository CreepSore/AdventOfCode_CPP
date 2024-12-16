#include "pch.h"
#include "AocRegistry.h"

#include <iomanip>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>

AocRegistry::AocRegistry(const std::string& dataPath)
{
    this->dataPath = dataPath;
}

void AocRegistry::registerDay(IAocDay* day)
{
    this->days.push_back(day);
}

std::string* AocRegistry::loadData(const uint16_t year, const uint8_t day) const
{
    const std::filesystem::path path = this->dataPath + "/" + std::to_string(year) + "." + std::to_string(day) + ".txt";

    if(!std::filesystem::exists(path))
    {
        return nullptr;
    }

    std::ifstream ifstream = std::ifstream(path, std::ios::in | std::ios::binary);
    const auto size = std::filesystem::file_size(path);
    std::string* result = new std::string(size, '\x00');

    ifstream.read(result->data(), size);

    return result;
}

void AocRegistry::run() const
{
    for (auto day : this->days)
    {
        auto sDay = std::to_string(day->getDay());

        if(sDay.length() == 1)
        {
            sDay.insert(0, "0");
        }  

        std::cout
            << "Running ["
            << std::to_string(day->getYear())
            << "/"
            << sDay
            << "]:\n";

        std::vector<AocDayPartResult> results;
        const std::string* data = this->loadData(day->getYear(), day->getDay());

        if(data == nullptr)
        {
            std::cout << "    No datafile found" << "\n\n";
            continue;
        }

        day->initialize(*data);

        for(int i = 1; i <= day->getPartCount(); i++)
        {
            results.clear();

            std::cout
                << "    Part "
                << std::to_string(i)
                << ": [";

            auto begin = std::chrono::steady_clock::now();
            day->run(results, i);

            if(results.size() == 1)
            {
                std::cout << std::to_string(results[0].result);
            }

            std::cout
                << "] ";

            std::string runtime;
            int type = 1;
            auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - begin).count();

            if(time > 1000)
            {
                time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count();
                type = 2;

                if(time > 1000)
                {
                    time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
                    type = 3;
                }
            }

            runtime = std::to_string(time);

            switch(type)
            {
            case 1:
                runtime.append(" ns");
                break;

            case 2:
                runtime.append(" us");
                break;

            case 3:
                runtime.append(" ms");
                break;

            default:
                break;
            }

            std::cout
                << runtime
                << '\n';
        }

        std::cout << '\n';

        delete data;
    }
}


