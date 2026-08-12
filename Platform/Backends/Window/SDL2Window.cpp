#include "SDL2Window.h"

#include "DisplayBridge.h"
#include "IInput.h"

#ifdef MOONCHILD_USE_OPENGL
#include "glad.h"
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>

#ifdef MOONCHILD_USE_OPENGL
static void* LoadSdlGlProc(const char* name)
{
    return reinterpret_cast<void*>(SDL_GL_GetProcAddress(name));
}
#endif

struct GameViewport
{
    float X = 0.0f;
    float Y = 0.0f;
    float Width = 0.0f;
    float Height = 0.0f;
};

constexpr float GAME_WIDTH = 640.0f;
constexpr float GAME_HEIGHT = 480.0f;

static GameViewport GetGameViewport(SDL_Window* window)
{
    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    GameViewport viewport;
    if (windowWidth <= 0 || windowHeight <= 0)
    {
        return viewport;
    }

    const float scaleX = static_cast<float>(windowWidth) / GAME_WIDTH;
    const float scaleY = static_cast<float>(windowHeight) / GAME_HEIGHT;
    const float scale = std::min(scaleX, scaleY);

    viewport.Width = GAME_WIDTH * scale;
    viewport.Height = GAME_HEIGHT * scale;
    viewport.X = (static_cast<float>(windowWidth) - viewport.Width) * 0.5f;
    viewport.Y = (static_cast<float>(windowHeight) - viewport.Height) * 0.5f;
    
    return viewport;
}

static void ScaleAbsoluteCoordinates(SDL_Window* window, float inX, float inY, float& outX, float& outY)
{
    const GameViewport viewport = GetGameViewport(window);
    if (viewport.Width <= 0.0f || viewport.Height <= 0.0f)
    {
        outX = inX;
        outY = inY;
        return;
    }

    const float normalizedX = (inX - viewport.X) / viewport.Width;
    const float normalizedY = (inY - viewport.Y) / viewport.Height;
    const float clampedX = std::max(0.0f, std::min(1.0f, normalizedX));
    const float clampedY = std::max(0.0f, std::min(1.0f, normalizedY));

    outX = clampedX * GAME_WIDTH;
    outY = clampedY * GAME_HEIGHT;
}

static void ScaleRelativeCoordinates(SDL_Window* window, float inX, float inY, float& outX, float& outY)
{
    const GameViewport viewport = GetGameViewport(window);
    if (viewport.Width <= 0.0f || viewport.Height <= 0.0f)
    {
        outX = inX;
        outY = inY;
        return;
    }

    outX = inX * (GAME_WIDTH / viewport.Width);
    outY = inY * (GAME_HEIGHT / viewport.Height);
}

SDL2Window::SDL2Window() = default;

SDL2Window::~SDL2Window()
{
    Destroy();
}

bool SDL2Window::Create(const char* title, int width, int height)
{
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    // Only video + events are needed here
    // The rest are initialized separately in their own places
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        printf("SDL video + init subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

    Uint32 windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    
#ifdef MOONCHILD_USE_OPENGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);

#ifdef MOONCHILD_RENDERER_OPENGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

#ifdef MOONCHILD_RENDERER_GLES
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    windowFlags |= SDL_WINDOW_OPENGL;
#endif

    Window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width,
                              height,
                              windowFlags);
    if (Window == nullptr)
    {
        printf("SDL window creation failed! %s\n", SDL_GetError());
        return false;
    }

#ifdef MOONCHILD_USE_OPENGL
    GlContext = SDL_GL_CreateContext(Window);
    if (GlContext == nullptr)
    {
        printf("SDL OpenGL context creation failed! %s\n", SDL_GetError());
        SDL_DestroyWindow(Window);
        Window = nullptr;
        return false;
    }

    SDL_GL_MakeCurrent(Window, GlContext);
#endif
    return true;
}

void SDL2Window::Destroy()
{
#ifdef MOONCHILD_USE_OPENGL
    if (GlContext != nullptr)
    {
        SDL_GL_DeleteContext(GlContext);
        GlContext = nullptr;
    }
#endif
    if (Window != nullptr)
    {
        SDL_DestroyWindow(Window);
        Window = nullptr;
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

WindowSize SDL2Window::GetPixelSize() const
{
    WindowSize size;
    SDL_GetWindowSizeInPixels(Window, &size.Width, &size.Height);
    return size;
}

void SDL2Window::DisplaySetFullscreen(bool enabled)
{
    if (SDL_SetWindowFullscreen(Window, enabled ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) == 0)
    {
        DisplayBridge::NotifyFullscreenChange(enabled ? 1 : 0);
    }
}

void SDL2Window::SetCursorVisibility(bool visible)
{
    if (RelativeMouseMode)
    {
        return;
    }

    const bool isFullscreen = (SDL_GetWindowFlags(Window) & SDL_WINDOW_FULLSCREEN) != 0;
    if (!isFullscreen || visible)
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }
}

void SDL2Window::SetRelativeMouseMode(bool enabled)
{
    if (RelativeMouseMode == enabled)
    {
        return;
    }

    if (SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) == 0)
    {
        RelativeMouseMode = enabled;
    }
}

#ifdef MOONCHILD_DESKTOP_MODE
bool SDL2Window::HandleFullscreenHotkey(const SDL_Event& ev)
{
    if (ev.type != SDL_KEYDOWN && ev.type != SDL_KEYUP)
    {
        return false;
    }

    const SDL_Keycode key = ev.key.keysym.sym;
    if (key != SDLK_RETURN && key != SDLK_KP_ENTER)
    {
        return false;
    }

    if (ev.type == SDL_KEYDOWN)
    {
        if (ev.key.repeat)
        {
            return false;
        }
        if ((ev.key.keysym.mod & KMOD_ALT) == 0)
        {
            return false;
        }
        
        const bool currentlyFullscreen = (SDL_GetWindowFlags(Window) & SDL_WINDOW_FULLSCREEN) != 0;
        const bool targetFullscreen = !currentlyFullscreen;
        if (SDL_SetWindowFullscreen(Window, targetFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) == 0)
        {
            DisplayBridge::NotifyFullscreenChange(targetFullscreen ? 1 : 0);
        }
        SwallowEnterKey = true; // gulp!
        return true;
    }

    if (SwallowEnterKey)
    {
        SwallowEnterKey = false;
        return true;
    }
    return false;
}
#endif

#ifdef MOONCHILD_USE_OPENGL
void SDL2Window::MakeCurrent()
{
    SDL_GL_MakeCurrent(Window, GlContext);
}

bool SDL2Window::LoadOpenGLFunctions()
{
    return gladLoadGLLoader(LoadSdlGlProc) != 0;
}

bool SDL2Window::LoadOpenGLESFunctions()
{
    return gladLoadGLES2Loader(LoadSdlGlProc) != 0;
}

void SDL2Window::SwapBuffers()
{
    SDL_GL_SwapWindow(Window);
}

void SDL2Window::SetSwapInterval(int interval)
{
    SDL_GL_SetSwapInterval(interval);
}
#endif

void SDL2Window::PumpOSEvents(IInput* sink, bool& outExitRequested)
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        if (sdlEvent.type == SDL_QUIT)
        {
            outExitRequested = true;
            continue;
        }

        switch (sdlEvent.type)
        {
            case SDL_WINDOWEVENT:
            {
                if (sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                {
#ifdef MOONCHILD_DESKTOP_MODE
                    SwallowEnterKey = false;
#endif
                    sink->OnFocusLost();
                }
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
#ifdef MOONCHILD_DESKTOP_MODE
                if (HandleFullscreenHotkey(sdlEvent))
                {
                    break;
                }
#endif
                sink->OnKeyEvent(static_cast<int>(sdlEvent.key.keysym.sym),
                                 sdlEvent.type == SDL_KEYDOWN,
                                 sdlEvent.key.repeat != 0);
                break;
            }

            case SDL_MOUSEMOTION:
            {
                float gameX = 0.0f;
                float gameY = 0.0f;
                float gameDeltaX = 0.0f;
                float gameDeltaY = 0.0f;
                ScaleAbsoluteCoordinates(Window, sdlEvent.motion.x, sdlEvent.motion.y, gameX, gameY);
                ScaleRelativeCoordinates(Window, sdlEvent.motion.xrel, sdlEvent.motion.yrel, gameDeltaX, gameDeltaY);
                sink->OnMouseMovement(gameX, gameY, gameDeltaX, gameDeltaY);
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                float gameX = 0.0f;
                float gameY = 0.0f;
                ScaleAbsoluteCoordinates(Window, sdlEvent.button.x, sdlEvent.button.y, gameX, gameY);
                sink->OnMouseButton(static_cast<int>(sdlEvent.button.button),
                                    sdlEvent.type == SDL_MOUSEBUTTONDOWN,
                                    gameX,
                                    gameY);
                break;
            }

            case SDL_CONTROLLERDEVICEADDED:
            {
                sink->OnGamepadConnected(static_cast<int>(sdlEvent.cdevice.which));
                break;
            }

            case SDL_CONTROLLERDEVICEREMOVED:
            {
                sink->OnGamepadDisconnected(static_cast<int>(sdlEvent.cdevice.which));
                break;
            }

            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            {
                sink->OnGamepadButton(static_cast<int>(sdlEvent.cbutton.which),
                                      sdlEvent.cbutton.button,
                                      sdlEvent.type == SDL_CONTROLLERBUTTONDOWN);
                break;
            }

            case SDL_CONTROLLERAXISMOTION:
            {
                sink->OnGamepadAxis(static_cast<int>(sdlEvent.caxis.which),
                                    sdlEvent.caxis.axis,
                                    sdlEvent.caxis.value);
                break;
            }

            default:
            {
                break;
            }
        }
    }
}
