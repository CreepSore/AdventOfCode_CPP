#pragma once
#include <set>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>

#include "IRenderable.h"
#include "../lib/imgui-sfml/imgui-SFML.h"

class BaseWindow
{
private:
    std::thread windowThread;
    void eventLoop();
    bool handleEvent(const sf::Event& event);

public:
    sf::RenderWindow* window = nullptr;
    sf::Clock clock;
    std::set<IRenderable*> renderables;

    BaseWindow(std::string& title, bool newThread = false, bool standalone = false);
    ~BaseWindow();

    void addRenderable(IRenderable* renderable);
    void removeRenderable(IRenderable* renderable);

    template <typename ArgsType = void*>
    void tickOnce(void(*onRender)(ArgsType*) = nullptr, ArgsType* args = nullptr)
    {
        if(this->window == nullptr)
        {
            return;
        }

        while (const std::optional event = this->window->pollEvent())
        {
            if (event.has_value())
            {
                if (!handleEvent(event.value()))
                {
                    break;
                }
            }
        }

        if (this->window == nullptr)
        {
            return;
        }

        ImGui::SFML::Update(*this->window, clock.restart());

        if (onRender != nullptr)
        {
            onRender(args);
        }

        for(IRenderable* renderable : this->renderables)
        {
            renderable->render(this);
        }

        this->window->clear();
        ImGui::SFML::Render(*this->window);
        window->display();
    }

    bool destroyWindow();
};
