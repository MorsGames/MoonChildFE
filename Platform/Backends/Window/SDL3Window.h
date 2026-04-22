#pragma once

#include "IWindow.h"

#ifdef MOONCHILD_RENDERER_GL
#include "IGLSurface.h"
#endif

#include <SDL3/SDL.h>

class SDL3Window final : public IWindow
#ifdef MOONCHILD_RENDERER_GL
                       , public IGLSurface
#endif
{
public:
    SDL3Window();
    ~SDL3Window() override;

    bool Create(const char* title, int width, int height) override;
    void Destroy() override;

    WindowSize GetPixelSize() const override;

    SDL_Window* GetNativeWindow() const { return Window; }

    void DisplaySetFullscreen(bool enabled) override;

    void PumpOSEvents(IInput* sink, bool& outExitRequested) override;

#ifdef MOONCHILD_RENDERER_GL
    void MakeCurrent() override;
    void SwapBuffers() override;
    void SetSwapInterval(int interval) override;
#endif

private:
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
    bool SwallowEnterKey = false;
    bool HandleFullscreenHotkey(const SDL_Event& ev);
#endif

    SDL_Window* Window = nullptr;
#ifdef MOONCHILD_RENDERER_GL
    SDL_GLContext GlContext = nullptr;
#endif
};
