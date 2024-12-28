#include "Aoc2024D08.h"

Aoc2024D08::Aoc2024D08()
{
    this->day = 8;
    this->year = 2024;
    this->isVisual = true;
}

Aoc2024D08::~Aoc2024D08()
{
    delete this->data;
}

AocDayPartResult Aoc2024D08::runPart1()
{
    int64_t result = 0;

    Grid grid = Grid::fromString(*this->data);
    std::map<uint8_t, std::vector<GridNode*>> frequencyNodeMapping;

    for(auto& pair : grid.nodes)
    {
        GridNode* node = pair.second;

        if(node->value == '.')
        {
            continue;
        }

        frequencyNodeMapping[node->value].push_back(node);
    }

    for(auto& pair : frequencyNodeMapping)
    {
        std::vector<GridNode*> nodes = pair.second;

        for(size_t i = 0; i < nodes.size(); i++)
        {
            GridNode* firstNode = nodes[i];

            for(size_t j = 0; j < nodes.size(); j++)
            {
                if(i == j)
                {
                    continue;
                }

                GridNode* secondNode = nodes[j];
                Vec2 delta = Vec2::subtract(secondNode->position, firstNode->position);
                delta.x *= -1;
                delta.y *= -1;

                Vec2 antinodePos = Vec2::add(firstNode->position, delta);

                if(grid.isInBounds(antinodePos))
                {
                    GridNode* antinode = grid.getNodeAt(antinodePos);

                    if(antinode != nullptr && antinode->value != '#')
                    {
                        antinode->value = '#';
                        result += 1;
                    }
                }
            }
        }
    }

    this->part1RenderArgs.valid = this->window != nullptr;
    this->part1RenderArgs.grid = &grid;

    if(this->part1RenderArgs.valid)
    {
        part1RenderArgs.colorMapping.clear();
        part1RenderArgs.colorMapping.emplace('#', ImVec4(1, 0, 0, 1));

        for(auto& pair : frequencyNodeMapping)
        {
            part1RenderArgs.colorMapping.emplace(pair.first, ImVec4(1, 0.5, 0, 1));
        }

        this->window->addRenderable(this);
        while (this->part1RenderArgs.valid)
        {
            this->window->tickOnce();
        }
    }

    return {result};
}

AocDayPartResult Aoc2024D08::runPart2()
{
    int64_t result = 0;

    return { result };
}

void Aoc2024D08::run(std::vector<AocDayPartResult>& resultList, int part)
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

void Aoc2024D08::render(BaseWindow* window)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    bool p_open = true;

    ImGui::Begin("Aoc2024D08", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

    if(!p_open)
    {
        this->part1RenderArgs.valid = false;

        ImGui::End();
        return;
    }

    if(this->part1RenderArgs.valid)
    {
        this->part1RenderArgs.grid->render(this->part1RenderArgs.colorMapping);
    }

    ImGui::End();
}
