#pragma once
#include <iostream>

#include "ILogger.h"

class StdoutLogger : public ILogger
{

public:
    void print(const char* message) override
    {
        std::cout << message << '\n';
    }
};
