#pragma once

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void print(const char* message) = 0;
};
