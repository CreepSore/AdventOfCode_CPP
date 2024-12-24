#pragma once
#include <vector>

#include "ILogger.h"

class VectorLogger : public ILogger
{
public:
    std::vector<const char*> data;

    void print(const char* message) override
    {
        this->data.push_back(message);
    }
};
