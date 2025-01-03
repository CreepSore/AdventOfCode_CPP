#include "AocRegistry.h"

#include <iomanip>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "utils.h"

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

std::map<uint32_t, std::vector<AocDayPartResult>> AocRegistry::run() const
{
    std::map<uint32_t, std::vector<AocDayPartResult>> result;

    for (auto day : this->days)
    {
        result[day->getId()] = this->runDay(day);
    }

    return result;
}

std::map<uint32_t, std::vector<AocDayPartResult>> AocRegistry::run(BaseWindow* window) const
{
    std::map<uint32_t, std::vector<AocDayPartResult>> result;

    for (auto day : this->days)
    {
        result[day->getId()] = this->runDay(day, window);
    }

    return result;
}

std::vector<AocDayPartResult> AocRegistry::runDay(IAocDay* day, BaseWindow* window) const
{
    std::vector<AocDayPartResult> allResults;
    std::vector<AocDayPartResult> results;

    auto sDay = std::to_string(day->getDay());

    if (sDay.length() == 1)
    {
        sDay.insert(0, "0");
    }

    this->logger->print(std::format("Running [{}/{}]:", day->getYear(), sDay).data());

    const std::string* data = this->loadData(day->getYear(), day->getDay());

    if (data == nullptr)
    {
        this->logger->print("  No datafile found");
        this->logger->print("");
        return allResults;
    }

    if(window == nullptr)
    {
        day->initialize(*data);
    }
    else
    {
        day->initialize(*data, window);
    }

    for (int i = 1; i <= day->getPartCount(); i++)
    {
        results.clear();

        this->logger->print(std::format("  Part {}:", i).data());

        auto begin = std::chrono::steady_clock::now();
        day->run(results, i);

        std::string runtime;
        auto time = std::chrono::steady_clock::now() - begin;

        if (results.size() > 0)
        {
            for(size_t i = 0; i < results.size(); i++)
            {
                results[i].duration = time;
                allResults.push_back(results[i]);
            }

            this->logger->print(std::format("    Result: {}", results[0].result).data());
        }


        this->logger->print(std::format("    Runtime: {}", durationToString(time)).data());
        this->logger->print("");
    }

    this->logger->print("");
    delete data;

    return allResults;
}

void AocRegistry::runBenchmark() const
{
    std::vector<int> benchmarks;
    benchmarks.push_back(10);
    benchmarks.push_back(100);
    benchmarks.push_back(1000);

    size_t spacing = 0;

    for (const int value : benchmarks)
    {
        size_t size = std::to_string(value).length();
        if(size > spacing)
        {
            spacing = size;
        }
    }

    for (const auto day : this->days)
    {
        auto sDay = std::to_string(day->getDay());

        if (sDay.length() == 1)
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

        if (data == nullptr)
        {
            std::cout << "    No datafile found" << "\n\n";
            continue;
        }

        day->initialize(*data);

        for (int i = 1; i <= day->getPartCount(); i++)
        {
            std::cout
                << "    Part "
                << std::to_string(i)
                << ":\n";


            for (int benchmark : benchmarks)
            {
                std::chrono::steady_clock::duration min {-1};
                std::chrono::steady_clock::duration max {-1};
                std::chrono::steady_clock::duration avg {-1};

                for(int j = 0; j < benchmark; j++)
                {
                    results.clear();
                    auto begin = std::chrono::steady_clock::now();

                    day->run(results, i);

                    auto timeDelta = std::chrono::steady_clock::now() - begin;

                    if(timeDelta < min || min.count() == -1)
                    {
                        min = timeDelta;
                    }

                    if(timeDelta > max || max.count() == -1)
                    {
                        max = timeDelta;
                    }

                    avg += timeDelta;
                }

                avg /= benchmark;

                

                std::cout
                    << std::setw(spacing + 8)
                    << std::to_string(benchmark)
                    << "x = "
                    << std::setw(8)
                    << durationToString(min)
                    << " MIN   "
                    << std::setw(8)
                    << durationToString(max)
                    << " MAX   "
                    << std::setw(8)
                    << durationToString(avg)
                    << " AVG\n";
            }
        }

        std::cout << '\n';

        delete data;
    }
}
