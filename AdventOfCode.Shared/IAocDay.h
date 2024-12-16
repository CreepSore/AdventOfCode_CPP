#pragma once
#include <string>
#include <vector>

#include "AocDayPartResult.h"

class IAocDay
{
protected:
    std::string* data;
    uint8_t day = -1;
    uint16_t year = -1;
    uint8_t parts = 2;
public:
    uint32_t getId() const;
    uint8_t getDay() const;
    uint16_t getYear() const;
    uint8_t getPartCount() const;
    void initialize(const std::string& data);

    virtual ~IAocDay() = default;
    virtual void run(std::vector<AocDayPartResult>& resultList, int part = -1) = 0;
};
