#pragma once

class IWindow;
class IRenderer;

namespace DisplayBridge
{
    using FullscreenChangeCallback = void (*)(int enabled);

    void Attach(IWindow* window, IRenderer* renderer);
    void Detach();

    void SetFullscreenChangeCallback(FullscreenChangeCallback callback);
    void NotifyFullscreenChange(int enabled);

    void SetFullscreen(int enabled);
    void SetVSync(int enabled);
}
