#pragma once
#include <map>
#include <string>
#include <vector>

#include "Vec2.h"

struct GridNode
{
    Vec2 position;
    uint8_t value;

    GridNode(const Vec2& position, uint8_t value);
};

class Grid
{
private:
    uint8_t width = 0;
    uint8_t height = 0;

public:
    std::map<uint16_t, GridNode*> nodes;
    ~Grid();

    void addNode(GridNode* node);
    GridNode* getNodeAt(const Vec2& offset);
    std::vector<GridNode*> getNeighbors(const GridNode& node);
    GridNode* getNeighbor(const GridNode& node, const Vec2& offset);

    uint8_t getWidth() const;
    uint8_t getHeight() const;

    std::string toString();

    static Grid fromString(const std::string& data);
};
