#pragma once
#include "IRenderable.h"

template <typename ArgsType>
class PseudoRenderable : public IRenderable
{
    void(*fn)(PseudoRenderable*, ArgsType&);
    ArgsType args;

public:
    PseudoRenderable(void(*fn)(PseudoRenderable*, ArgsType&), ArgsType args)
    {
        this->fn = fn;
        this->args = args;
    }

    void render(BaseWindow* window) override
    {
        this->fn(this, this->args);
    }
};
