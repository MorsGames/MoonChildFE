#pragma once

#include "IInput.h"

namespace InputBridge
{
    void Attach(IInput* input);
    void Detach();

    void Tick();
    bool PollNext(InputEvent& out);
}
