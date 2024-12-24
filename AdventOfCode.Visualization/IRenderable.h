#pragma once

class BaseWindow;

class IRenderable
{
public:
    virtual ~IRenderable() = default;
    virtual void render(BaseWindow* window) = 0;
};
