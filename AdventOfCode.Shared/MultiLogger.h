#pragma once
#include <vector>

#include "ILogger.h"

class MultiLogger : public ILogger
{
private:
    std::set<ILogger*> loggers;

public:
    ~MultiLogger()
    {
        for(auto logger : this->loggers)
        {
            delete logger;
        }
    }

    MultiLogger* appendLogger(ILogger* logger)
    {
        this->loggers.insert(logger);
        return this;
    }

    MultiLogger* removeLogger(ILogger* logger)
    {
        this->loggers.erase(logger);
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
