#pragma once
#include <cstdint>
#include <ostream>

struct Vec2
{
    uint32_t hash;
    int16_t x;
    int16_t y;

    Vec2();
    Vec2(const Vec2& obj);
    Vec2(int16_t x, int16_t y);

    uint16_t getHash() const {
        return this->hash;
    }

    void add(const Vec2& obj);

    /// <summary>
    /// Adds both Vectors together and returns a new Vec2 object
    /// </summary>
    /// <returns></returns>
    static Vec2 add(const Vec2& obj0, const Vec2& obj1);
    static Vec2 subtract(const Vec2& obj0, const Vec2& obj1);
    static bool equals(const Vec2& obj0, const Vec2& obj1);

    static Vec2 VEC_UP;
    static Vec2 VEC_UP_RIGHT;
    static Vec2 VEC_RIGHT;
    static Vec2 VEC_DOWN_RIGHT;
    static Vec2 VEC_DOWN;
    static Vec2 VEC_DOWN_LEFT;
    static Vec2 VEC_LEFT;
    static Vec2 VEC_UP_LEFT;
};

std::ostream& operator<<(std::ostream& stream, const Vec2& vec);
