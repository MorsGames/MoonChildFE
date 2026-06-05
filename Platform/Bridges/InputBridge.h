#pragma once

#include "IInput.h"

namespace InputBridge
{
    void Attach(IInput* input);
    void Detach();

    void OnMouseMovement(int x, int y, int deltaX, int deltaY);
    void OnMouseButton(int button, bool isDown, int x, int y);
    void OnFocusLost();

    void Tick();
    bool PollNext(InputEvent& out);

    int GetMouseIdleFrames();
}
