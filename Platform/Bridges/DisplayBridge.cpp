#include "DisplayBridge.h"

#include "IRenderer.h"
#include "IWindow.h"

static IWindow* WindowBackend = nullptr;
static IRenderer* RendererBackend = nullptr;
static DisplayBridge::FullscreenChangeCallback FullscreenChanged = nullptr;

namespace DisplayBridge
{
    void Attach(IWindow* window, IRenderer* renderer)
    {
        WindowBackend = window;
        RendererBackend = renderer;
    }

    void Detach()
    {
        WindowBackend = nullptr;
        RendererBackend = nullptr;
        FullscreenChanged = nullptr;
    }

    void SetFullscreenChangeCallback(FullscreenChangeCallback callback)
    {
        FullscreenChanged = callback;
    }

    void NotifyFullscreenChange(int enabled)
    {
        if (FullscreenChanged != nullptr)
        {
            FullscreenChanged(enabled != 0 ? 1 : 0);
        }
    }

    void SetFullscreen(int enabled)
    {
        WindowBackend->DisplaySetFullscreen(enabled != 0);
    }

    void SetVSync(int enabled)
    {
        RendererBackend->DisplaySetVSync(enabled != 0);
    }
}
