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
    bool nodesPosInitialized = false;

public:
    std::map<uint16_t, GridNode*> nodes;
    std::vector<std::vector<GridNode*>> nodesPos;

    ~Grid();

    void addNode(GridNode* node);
    GridNode* getNodeAt(const Vec2& offset) const;
    std::vector<GridNode*> getNeighbors(const GridNode& node) const;
    GridNode* getNeighbor(const GridNode& node, const Vec2& offset) const;

    uint8_t getWidth() const;
    uint8_t getHeight() const;

    void fillNodesPos();
    std::string toString() const;

    static Grid fromString(const std::string& data);
};
