#include "pch.h"
#include "Aoc2024D06.h"

#include <iostream>
#include <map>
#include <numeric>
#include <set>

#include "../Shared/Grid.h"

Aoc2024D06::Aoc2024D06()
{
    this->day = 6;
    this->year = 2024;
    this->isVisual = true;
}

Aoc2024D06::~Aoc2024D06()
{
    delete this->data;
}

void Aoc2024D06::renderPart1()
{
    std::string title = std::format("Grid (Depth {})", this->part1RenderData->inDepth);

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::PushID("Grid");
    ImGui::Begin(title.data(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

    {
        ImGui::BeginChild("Controls", ImVec2(ImGui::GetContentRegionAvail().x, 20));
        ImGui::PushID("PauseButton");
        if (ImGui::Button(this->part1RenderData->paused ? "Resume" : "Pause", ImVec2(100, 20)))
        {
            this->part1RenderData->paused = !this->part1RenderData->paused;
            this->part1RenderData->changed = true;
        }
        ImGui::PopID();

        ImGui::SameLine();
        if (ImGui::Button("Skip", ImVec2(200, 20)))
        {
            this->part1RenderData->skip = !this->part1RenderData->skip;
            this->part1RenderData->changed = true;

        }

        ImGui::SameLine();
        if (ImGui::SliderInt("Steps", &this->part1RenderData->stepConfig, 1, 1000))
        {
            this->part1RenderData->stepConfig = std::max(this->part1RenderData->stepConfig - (this->part1RenderData->stepConfig % 10), 1);
        }

        ImGui::SameLine();
        if (ImGui::Button("Step", ImVec2(100, 20)))
        {
            this->part1RenderData->step = this->part1RenderData->stepConfig;
            this->part1RenderData->changed = true;
        }

        ImGui::EndChild();
    }

    {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
        ImGui::BeginChild("View", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
        if (ImGui::BeginTable("grid", this->part1RenderData->inGrid->getWidth(), ImGuiTableFlags_SizingFixedFit))
        {
            for (int y = 0; y < this->part1RenderData->inGrid->getHeight(); y++)
            {
                for (int x = 0; x < this->part1RenderData->inGrid->getWidth(); x++)
                {
                    auto node = this->part1RenderData->inGrid->getNodeAt(Vec2(x, y));

                    std::string text;
                    text.push_back(node->value);

                    ImVec4 color;
                    bool doScroll = false;
                    switch (node->value)
                    {
                    case '#':
                        color = ImVec4(1, 0, 0, 1);
                        break;

                    case '.':
                        color = ImVec4(0.6, 0.6, 0.6, 1);

                        if (this->part1RenderData->inVisitedPositions->contains(node->position.getHash()))
                        {
                            color = ImVec4(0, 1, 0, 1);
                        }

                        break;

                    case 'X':
                        color = ImVec4(0, 1, 0, 1);
                        doScroll = true;
                        break;

                    default:
                        color = ImVec4(1, 1, 1, 1);
                    }

                    ImGui::TextColored(color, text.data());

                    if (doScroll)
                    {
                        ImGui::ScrollToBringRectIntoView(ImGui::GetCurrentWindow(), ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
                    }
                    ImGui::TableNextColumn();
                }
            }

            ImGui::EndTable();
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();
    }

    ImGui::End();
    ImGui::PopID();
}

void Aoc2024D06::render(BaseWindow* window)
{
    if(this->part1RenderData != nullptr)
    {
        this->renderPart1();
    }
}

AocDayPartResult Aoc2024D06::runPart1()
{
    static std::map<uint16_t, Vec2> rotation = {
        { Vec2::VEC_UP.getHash(), Vec2::VEC_RIGHT },
        { Vec2::VEC_RIGHT.getHash(), Vec2::VEC_DOWN },
        { Vec2::VEC_DOWN.getHash(), Vec2::VEC_LEFT },
        { Vec2::VEC_LEFT.getHash(), Vec2::VEC_UP }
    };

    int result = 0;

    Grid grid = Grid::fromString(*this->data);
    Vec2 startPosition;

    for (std::pair<uint8_t, GridNode*> node : grid.nodes)
    {
        if (node.second->value == '^')
        {
            startPosition = node.second->position;
            break;
        }
    }

    this->part1RenderData = new Part1RenderData(&grid, -1, nullptr);

    result = traverseGrid(grid, startPosition, Vec2::VEC_UP, nullptr, 0).visited;
    delete this->part1RenderData;
    this->part1RenderData = nullptr;

    return { result };
}

AocDayPartResult Aoc2024D06::runPart2()
{
    static std::map<uint16_t, Vec2> rotation = {
        { Vec2::VEC_UP.getHash(), Vec2::VEC_RIGHT },
        { Vec2::VEC_RIGHT.getHash(), Vec2::VEC_DOWN },
        { Vec2::VEC_DOWN.getHash(), Vec2::VEC_LEFT },
        { Vec2::VEC_LEFT.getHash(), Vec2::VEC_UP }
    };

    int result = 0;

    Grid visualGrid = Grid::fromString(*this->data);
    Grid grid = Grid::fromString(*this->data);
    Vec2 startPosition;

    for (std::pair<uint8_t, GridNode*> node : grid.nodes)
    {
        if (node.second->value == '^')
        {
            startPosition = node.second->position;
            break;
        }
    }

    auto traversed = traverseGrid(grid, startPosition, Vec2::VEC_UP, nullptr, 1);

    for (auto block : traversed.blocks)
    {
        if(!Vec2::equals(block, startPosition))
        {
            result += 1;
        }
    }

    return { result };
}

Aoc2024D06::TraverseResult Aoc2024D06::traverseGrid(
    Grid& grid, 
    const Vec2& startPosition, 
    const Vec2& startDirection,
    std::set<uint32_t>* inVisited,
    int depth
)
{
    TraverseResult result;
    result.blocks = std::vector<Vec2>();
    result.visited = 1;

    static std::map<uint16_t, Vec2> rotation = {
        { Vec2::VEC_UP.getHash(), Vec2::VEC_RIGHT },
        { Vec2::VEC_RIGHT.getHash(), Vec2::VEC_DOWN },
        { Vec2::VEC_DOWN.getHash(), Vec2::VEC_LEFT },
        { Vec2::VEC_LEFT.getHash(), Vec2::VEC_UP }
    };

    std::set<uint16_t> visitedPositions;
    std::set<uint32_t>* visited = new std::set<uint32_t>();

    if(inVisited != nullptr)
    {
        for (uint32_t value : *inVisited)
        {
            visited->insert(value);
        }
    }

    GridNode* currentNode = grid.getNodeAt(startPosition);
    Vec2 currentDirection = startDirection;

    if(currentNode == nullptr)
    {
        result.looping = false;
        return result;
    }

    for(;;)
    {
        if(this->window != nullptr)
        {
            if(this->part1RenderData != nullptr)
            {
                this->part1RenderData->inVisitedPositions = &visitedPositions;
                this->part1RenderData->changed = true;

                while(
                    this->window != nullptr
                    && (this->part1RenderData->changed || this->part1RenderData->paused)
                    && this->part1RenderData->step == 0 && !this->part1RenderData->skip
                )
                {
                    this->part1RenderData->changed = false;

                    const char old = currentNode->value;
                    currentNode->value = 'X';
                    this->handleVisual();
                    currentNode->value = old;
                }

                if (this->part1RenderData->step > 0)
                {
                    this->part1RenderData->step -= 1;
                }
            }
        }

        const uint16_t currentPosHash = currentNode->position.getHash();
        const uint16_t currentDirHash = currentDirection.getHash();
        const uint32_t mergedHash = (currentPosHash << 16 | currentDirHash);

        if (visited->contains(mergedHash))
        {
            result.looping = true;
            break;
        }

        visitedPositions.insert(currentPosHash);
        visited->insert(mergedHash);

        GridNode* nextNode = grid.getNodeAt(Vec2::add(currentNode->position, currentDirection));

        if(nextNode == nullptr)
        {
            result.looping = false;
            break;
        }

        if(nextNode->value == '#')
        {
            currentDirection = rotation[currentDirHash];
            continue;
        }

        if(depth > 0 && !visitedPositions.contains(nextNode->position.getHash()))
        {
            const char oldValue = nextNode->value;
            nextNode->value = '#';
            auto traversed = traverseGrid(
                grid,
                Vec2::add(currentNode->position, rotation[currentDirHash]),
                rotation[currentDirHash],
                visited,
                depth - 1
            );
            nextNode->value = oldValue;

            if(traversed.looping)
            {
                result.blocks.push_back(nextNode->position);
            }
            else
            {
                for (auto block : traversed.blocks)
                {
                    result.blocks.push_back(block);
                }
            }
        }

        currentNode = nextNode;
    }

    result.visited = visitedPositions.size();
    delete visited;
    return result;
}


void Aoc2024D06::run(std::vector<AocDayPartResult>& resultList, int part)
{
    switch (part)
    {
    case -1:
        resultList.push_back(runPart1());
        resultList.push_back(runPart2());
        break;

    case 1:
        resultList.push_back(runPart1());
        break;

    case 2:
        resultList.push_back(runPart2());
        break;

    default:
        break;
    }
}
