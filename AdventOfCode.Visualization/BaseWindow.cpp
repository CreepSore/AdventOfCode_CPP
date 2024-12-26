#include "BaseWindow.h"

#include <functional>

#include "SFML/Window.hpp"

#include <iostream>
#include <string>

#include <imgui.h>
#include <imgui-SFML.h>

BaseWindow::BaseWindow(
    std::string& title,
    bool newThread,
    bool standalone
)
{
    if(newThread && !standalone)
    {
        return;
    }

    auto init = [this, title, standalone] {
        this->window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(1000, 1000)), title);

        ImGui::SFML::Init(*this->window);
        ImGui::GetIO().IniFilename = nullptr;

        if(standalone)
        {
            this->eventLoop();
        }
    };

    if(newThread)
    {
        this->windowThread = std::thread([this, init] {
            init();
        });

        return;
    }

    init();
}

void BaseWindow::eventLoop()
{
    while(this->window != nullptr && this->window->isOpen())
    {
        this->tickOnce<void*>(nullptr, nullptr);
    }
}

bool BaseWindow::handleEvent(const sf::Event& event)
{
    if(event.getIf<sf::Event::Closed>())
    {
        this->destroyWindow();
        return false;
    }

    ImGui::SFML::ProcessEvent(*this->window, event);
    return true;
}

BaseWindow::~BaseWindow()
{
    this->destroyWindow();
}

void BaseWindow::addRenderable(IRenderable* renderable)
{
    this->renderables.insert(renderable);
}

void BaseWindow::removeRenderable(IRenderable* renderable)
{
    this->renderables.erase(renderable);
}

bool BaseWindow::hasRenderable(IRenderable* renderable) const
{
    return this->renderables.contains(renderable);
}

bool BaseWindow::destroyWindow()
{
    if (this->window == nullptr) {
        return false;
    }

    ImGui::SFML::Shutdown(*this->window);
    this->window->close();
    delete this->window;
    this->window = nullptr;
    return true;
}
