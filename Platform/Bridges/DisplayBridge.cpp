#include "DisplayBridge.h"

#include "IRenderer.h"
#include "IWindow.h"
#include "frm_int.hpp"

static IWindow* WindowBackend = nullptr;
static IRenderer* RendererBackend = nullptr;
static Cvideo::FullscreenChangeCallback FullscreenChanged = nullptr;

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

    void NotifyFullscreenChange(int enabled)
    {
        if (FullscreenChanged != nullptr)
        {
            FullscreenChanged(enabled != 0 ? 1 : 0);
        }
    }
}

void Cvideo::set_fullscreen(int enabled)
{
    WindowBackend->DisplaySetFullscreen(enabled != 0);
}

void Cvideo::set_vsync(int enabled)
{
    RendererBackend->DisplaySetVSync(enabled != 0);
}

void Cvideo::set_cursor_visibility(int enabled)
{
    WindowBackend->SetCursorVisibility(enabled != 0);
}

void Cvideo::set_fullscreen_change_callback(FullscreenChangeCallback callback)
{
    FullscreenChanged = callback;
}
