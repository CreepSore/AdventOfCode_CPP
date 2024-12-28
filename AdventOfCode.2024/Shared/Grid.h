#pragma once
#include <map>
#include <string>
#include <vector>

#include "Vec2.h"
#include <imgui.h>

struct GridNode
{
    Vec2 position;
    uint8_t value;

    GridNode(const Vec2& position, uint8_t value);
};

class Grid
{
private:
    int16_t width = 0;
    int16_t height = 0;
    bool nodesPosInitialized = false;

public:
    std::map<uint32_t, GridNode*> nodes;
    std::vector<std::vector<GridNode*>> nodesPos;

    Grid() = default;
    ~Grid();

    void addNode(GridNode* node);
    GridNode* getNodeAt(const Vec2& offset) const;
    std::vector<GridNode*> getNodesWithValue(uint8_t value) const;
    std::vector<GridNode*> getNeighbors(const GridNode& node) const;
    GridNode* getNeighbor(const GridNode& node, const Vec2& offset) const;

    int16_t getWidth() const;
    int16_t getHeight() const;

    void fillNodesPos();
    std::string toString() const;

    void render() const;
    void render(std::map<uint8_t, ImVec4>& valueColorMapping) const;

    bool isInBounds(const Vec2& pos) const;

    static Grid fromString(const std::string& data);
};
