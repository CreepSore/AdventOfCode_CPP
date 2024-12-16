#include "pch.h"
#include "IAocDay.h"

uint8_t IAocDay::getDay() const
{
    return this->day;
}

uint16_t IAocDay::getYear() const
{
    return this->year;
}

uint8_t IAocDay::getPartCount() const
{
    return this->parts;
}

uint32_t IAocDay::getId() const
{
    uint32_t result = (static_cast<uint32_t>(this->year) << 8) | this->day;

    return result;
}

void IAocDay::initialize(const std::string& data)
{
    this->data = new std::string(data);
}
