#pragma once
#include "IRenderable.h"

template <typename ArgsType>
class PseudoRenderable : public IRenderable
{
    void(*fn)(ArgsType*);
    ArgsType* args;

    PseudoRenderable(void(*fn)(ArgsType*), ArgsType* args)
    {
        this->fn = fn;
        this->args = args;
    }

    void render(BaseWindow* window) override
    {
        this->fn(this->args);
    }
};
