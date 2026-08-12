#pragma once

class IWindow;
class IRenderer;

namespace DisplayBridge
{
    void Attach(IWindow* window, IRenderer* renderer);
    void Detach();

    void NotifyFullscreenChange(int enabled);
}
