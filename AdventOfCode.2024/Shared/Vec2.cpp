#include "Vec2.h"

#include <string>

Vec2::Vec2()
{
    this->x = 0;
    this->y = 0;
    this->hash = 0;
}

Vec2::Vec2(const Vec2& obj)
{
    this->x = obj.x;
    this->y = obj.y;
    this->hash = (static_cast<uint16_t>(this->x) << 16) | this->y;
}

Vec2::Vec2(int16_t x, int16_t y)
{
    this->x = x;
    this->y = y;
    this->hash = (static_cast<uint16_t>(this->x) << 16) | this->y;
}

void Vec2::add(const Vec2& obj)
{
    this->x += obj.x;
    this->y += obj.y;
}

Vec2 Vec2::add(const Vec2& obj0, const Vec2& obj1)
{
    return {static_cast<int16_t>(obj0.x + obj1.x), static_cast<int16_t>(obj0.y + obj1.y)};
}

Vec2 Vec2::subtract(const Vec2& obj0, const Vec2& obj1)
{
    return { static_cast<int16_t>(obj0.x - obj1.x), static_cast<int16_t>(obj0.y - obj1.y) };
}

bool Vec2::equals(const Vec2& obj0, const Vec2& obj1)
{
    return obj0.x == obj1.x
        && obj0.y == obj1.y;
}

std::ostream& operator<<(std::ostream& stream, const Vec2& vec)
{
    stream << "Vec2[" << std::to_string(vec.x) << "," << std::to_string(vec.y) << "]";
    return stream;
}

/// <summary>
/// Vec2[0, -1]
/// </summary>
Vec2 Vec2::VEC_UP = Vec2(0, -1);

/// <summary>
/// Vec2[1, -1]
/// </summary>
Vec2 Vec2::VEC_UP_RIGHT = Vec2(1, -1);

/// <summary>
/// Vec2[1, 0]
/// </summary>
Vec2 Vec2::VEC_RIGHT = Vec2(1, 0);

/// <summary>
/// Vec2[1, 1]
/// </summary>
Vec2 Vec2::VEC_DOWN_RIGHT = Vec2(1, 1);

/// <summary>
/// Vec2[0, 1]
/// </summary>
Vec2 Vec2::VEC_DOWN = Vec2(0, 1);

/// <summary>
/// Vec2[-1, 1]
/// </summary>
Vec2 Vec2::VEC_DOWN_LEFT = Vec2(-1, 1);

/// <summary>
/// Vec2[-1, 0]
/// </summary>
Vec2 Vec2::VEC_LEFT = Vec2(-1, 0);

/// <summary>
/// Vec2[-1, -1]
/// </summary>
Vec2 Vec2::VEC_UP_LEFT = Vec2(-1, -1);
