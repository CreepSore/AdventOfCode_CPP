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

bool IAocDay::getIsVisual() const
{
    return this->isVisual;
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

void IAocDay::initialize(const std::string& data, BaseWindow* window)
{
    this->initialize(data);
    this->window = window;
}

void IAocDay::handleVisual()
{
    if(!this->getIsVisual() || this->window == nullptr)
    {
        return;
    }

    this->window->tickOnce<IAocDay>([](IAocDay* t) {
        t->render();
    }, this);
}

void IAocDay::render()
{
    // Do nothing by default
}
