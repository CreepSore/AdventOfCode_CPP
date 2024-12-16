#include "../AdventOfCode.2024/Aoc2024D01.h"
#include "../AdventOfCode.Shared/AocRegistry.h"

int main()
{
    Aoc2024D01 d01p01 = Aoc2024D01();

    AocRegistry registry = AocRegistry(std::string("E:\\_AOC"));
    registry.registerDay(&d01p01);

    registry.run();
}
