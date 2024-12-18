#include "pch.h";
#include "Vec2.h";


Vec2::Vec2()
{
    this->x = 0;
    this->y = 0;
}

Vec2::Vec2(const Vec2& obj)
{
    this->x = obj.x;
    this->y = obj.y;
}

Vec2::Vec2(uint8_t x, uint8_t y)
{
    this->x = x;
    this->y = y;
}

uint16_t Vec2::getHash() const
{
    return (this->x << 8) | this->y;
}

Vec2 Vec2::add(const Vec2& obj0, const Vec2& obj1)
{
    return {static_cast<uint8_t>(obj0.x + obj1.x), static_cast<uint8_t>(obj0.y + obj1.y)};
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
