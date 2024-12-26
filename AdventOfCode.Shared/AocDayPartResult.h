#pragma once
#include <chrono>

struct AocDayPartResult
{
    int64_t result;
    std::chrono::steady_clock::duration duration;

    AocDayPartResult(int64_t result)
    {
        this->result = result;
    }
};
