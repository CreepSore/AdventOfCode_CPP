#pragma once
#include <vector>

#include "ILogger.h"

class MultiLogger : public ILogger
{
private:
    std::vector<ILogger*> loggers;

public:
    MultiLogger* appendLogger(ILogger* logger)
    {
        this->loggers.push_back(logger);
        return this;
    }

    void print(const char* message) override
    {
        for(auto logger : loggers)
        {
            logger->print(message);
        }
    }
};
