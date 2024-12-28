#include "Grid.h"

#include <format>

#include "imgui.h"
#include "imgui_internal.h"

GridNode::GridNode(const Vec2& position, const uint8_t value)
{
    this->position = Vec2(position);
    this->value = value;
}

Grid::~Grid()
{
    for (const auto pair : this->nodes)
    {
        delete pair.second;
    }
}

void Grid::addNode(GridNode* node)
{
    this->nodes[node->position.hash] = node;

    this->height = std::max(this->height, static_cast<int16_t>(node->position.y + 1));
    this->width = std::max(this->width, static_cast<int16_t>(node->position.x + 1));
}

GridNode* Grid::getNodeAt(const Vec2& offset) const
{
    if(!this->isInBounds(offset))
    {
        return nullptr;
    }

    if(nodesPosInitialized)
    {
        return nodesPos[offset.x][offset.y];
    }

    if(!this->nodes.contains(offset.hash))
    {
        return nullptr;
    }

    return this->nodes.at(offset.hash);
}

std::vector<GridNode*> Grid::getNodesWithValue(const uint8_t value) const
{
    std::vector<GridNode*> result;

    for (auto node : this->nodes)
    {
        if(node.second->value == value)
        {
            result.push_back(node.second);
        }
    }

    return result;
}

std::vector<GridNode*> Grid::getNeighbors(const GridNode& node) const
{
    std::vector<GridNode*> result;

    GridNode* up = getNodeAt(Vec2::add(node.position, Vec2::VEC_UP));
    GridNode* upRight = getNodeAt(Vec2::add(node.position, Vec2::VEC_UP_RIGHT));
    GridNode* right = getNodeAt(Vec2::add(node.position, Vec2::VEC_RIGHT));
    GridNode* downRight = getNodeAt(Vec2::add(node.position, Vec2::VEC_DOWN_RIGHT));
    GridNode* down = getNodeAt(Vec2::add(node.position, Vec2::VEC_DOWN));
    GridNode* downLeft = getNodeAt(Vec2::add(node.position, Vec2::VEC_DOWN_LEFT));
    GridNode* left = getNodeAt(Vec2::add(node.position, Vec2::VEC_LEFT));
    GridNode* upLeft = getNodeAt(Vec2::add(node.position, Vec2::VEC_UP_LEFT));

    if (up != nullptr) result.push_back(up);
    if (upRight != nullptr) result.push_back(upRight);
    if (right != nullptr) result.push_back(right);
    if (downRight != nullptr) result.push_back(downRight);
    if (down != nullptr) result.push_back(down);
    if (downLeft != nullptr) result.push_back(downLeft);
    if (left != nullptr) result.push_back(left);
    if (upLeft != nullptr) result.push_back(upLeft);

    return result;
}

GridNode* Grid::getNeighbor(const GridNode& node, const Vec2& offset) const
{
    return this->getNodeAt(Vec2::add(node.position, offset));
}

int16_t Grid::getWidth() const
{
    return this->height;
}

int16_t Grid::getHeight() const
{
    return this->height;
}

void Grid::fillNodesPos()
{
    if(!nodesPos.empty())
    {
        return;
    }

    for (uint8_t x = 0; x < this->width; x++)
    {
        std::vector<GridNode*> list;
        for (uint8_t y = 0; y < this->height; y++)
        {
            list.push_back(this->getNodeAt(Vec2(x, y)));
        }
        nodesPos.push_back(list);
    }

    nodesPosInitialized = true;
}

std::string Grid::toString() const
{
    std::string result;

    for (int y = 0; y < this->height; y++)
    {
        if (y != 0)
        {
            result.push_back('\n');
        }

        for (int x = 0; x < this->width; x++)
        {
            GridNode* node = this->getNodeAt(Vec2(x, y));
            if (node != nullptr)
            {
                result.push_back(node->value);
            }
        }
    }

    return result;
}

void Grid::render() const
{
    auto dummy = std::map<uint8_t, ImVec4>();
    this->render(dummy);
}

void Grid::render(std::map<uint8_t, ImVec4>& valueColorMapping) const
{
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    ImGui::BeginChild("View", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    if (ImGui::BeginTable("grid", this->getWidth(), ImGuiTableFlags_SizingFixedFit))
    {
        for (uint8_t y = 0; y < this->getHeight(); y++)
        {
            for (uint8_t x = 0; x < this->getWidth(); x++)
            {
                ImGui::TableNextColumn();
                const auto node = this->getNodeAt(Vec2(x, y));

                std::string text;
                text.push_back(static_cast<const char>(node->value));

                ImVec4 color = ImVec4(1, 1, 1, 1);

                if(valueColorMapping.contains(node->value))
                {
                    color = valueColorMapping.at(node->value);
                }

                ImGui::TextColored(color, text.data());

                if(ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text(std::format("@ {}X {}Y", node->position.x, node->position.y).data());
                    ImGui::Text(std::format("@ {}W {}H", this->width, this->height).data());
                    ImGui::EndTooltip();
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}

bool Grid::isInBounds(const Vec2& pos) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < this->width && pos.y < this->height;
}

Grid Grid::fromString(const std::string& data)
{
    Grid grid;
    int32_t currX = 0;
    int32_t currY = 0;

    for(size_t i = 0; i < data.length(); i++)
    {
        const char c = data[i];

        if(c == '\r')
        {
            continue;
        }

        if(c == '\n')
        {
            currX = 0;
            currY += 1;
            continue;
        }

        grid.addNode(new GridNode(Vec2(currX, currY), c));

        currX += 1;
    }

    return grid;
}
