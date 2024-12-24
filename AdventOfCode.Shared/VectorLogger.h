#pragma once
#include <vector>

#include "ILogger.h"

class VectorLogger : public ILogger
{
public:
    std::vector<std::string> data;

    void print(const char* message) override
    {
        this->data.push_back(std::string(message));
    }
};
