#include "SDL3Window.h"

#include "DisplayBridge.h"
#include "IInput.h"

#ifdef MOONCHILD_USE_OPENGL
#include "glad.h"
#endif

#include <cmath>
#include <cstdio>

#ifdef MOONCHILD_USE_OPENGL
static void* LoadSdlGlProc(const char* name)
{
    return reinterpret_cast<void*>(SDL_GL_GetProcAddress(name));
}
#endif

SDL3Window::SDL3Window() = default;

SDL3Window::~SDL3Window()
{
    Destroy();
}

bool SDL3Window::Create(const char* title, int width, int height)
{
    if (!SDL_SetAppMetadata(title, NULL, NULL))
    {
        printf("SDL set app metadata failed! %s\n", SDL_GetError());
        return false;
    }

    // Only video + events are needed here
    // The rest are initialized separately in their own places
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        printf("SDL video + init subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

    SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE;
    
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

    Window = SDL_CreateWindow(title, width, height, windowFlags);
    if (Window == nullptr)
    {
        printf("SDL window creation failed! %s\n", SDL_GetError());
        return false;
    }

    SDL_DisplayID displayId = SDL_GetDisplayForWindow(Window);
    float scale = ceil(SDL_GetDisplayContentScale(displayId));
    if (scale > 1.0f)
    {
        SDL_SetWindowSize(Window, width * scale, height * scale);
        SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
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

void SDL3Window::Destroy()
{
#ifdef MOONCHILD_USE_OPENGL
    if (GlContext != nullptr)
    {
        SDL_GL_DestroyContext(GlContext);
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

WindowSize SDL3Window::GetPixelSize() const
{
    WindowSize size;
    SDL_GetWindowSize(Window, &size.Width, &size.Height);
    return size;
}

void SDL3Window::DisplaySetFullscreen(bool enabled)
{
    SDL_SetWindowFullscreen(Window, enabled);
}

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
bool SDL3Window::HandleFullscreenHotkey(const SDL_Event& ev)
{
    if (ev.type != SDL_EVENT_KEY_DOWN && ev.type != SDL_EVENT_KEY_UP)
    {
        return false;
    }

    const SDL_Keycode key = ev.key.key;
    if (key != SDLK_RETURN && key != SDLK_KP_ENTER)
    {
        return false;
    }

    if (ev.type == SDL_EVENT_KEY_DOWN)
    {
        if (ev.key.repeat)
        {
            return false;
        }
        if ((ev.key.mod & SDL_KMOD_ALT) == 0)
        {
            return false;
        }
        
        const bool currentlyFullscreen = (SDL_GetWindowFlags(Window) & SDL_WINDOW_FULLSCREEN) != 0;
        const bool targetFullscreen = !currentlyFullscreen;
        if (SDL_SetWindowFullscreen(Window, targetFullscreen))
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
void SDL3Window::MakeCurrent()
{
    SDL_GL_MakeCurrent(Window, GlContext);
}

bool SDL3Window::LoadOpenGLFunctions()
{
    return gladLoadGLLoader(LoadSdlGlProc) != 0;
}

bool SDL3Window::LoadOpenGLESFunctions()
{
    return gladLoadGLES2Loader(LoadSdlGlProc) != 0;
}

void SDL3Window::SwapBuffers()
{
    SDL_GL_SwapWindow(Window);
}

void SDL3Window::SetSwapInterval(int interval)
{
    SDL_GL_SetSwapInterval(interval);
}
#endif

void SDL3Window::PumpOSEvents(IInput* sink, bool& outExitRequested)
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        if (sdlEvent.type == SDL_EVENT_QUIT)
        {
            outExitRequested = true;
            continue;
        }

        switch (sdlEvent.type)
        {
            case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
                SDL_HideCursor();
                DisplayBridge::NotifyFullscreenChange(1);
                break;

            case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
                SDL_ShowCursor();
                DisplayBridge::NotifyFullscreenChange(0);
                break;

            case SDL_EVENT_WINDOW_FOCUS_LOST:
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
                SwallowEnterKey = false;
#endif
                sink->OnFocusLost();
                break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
                if (HandleFullscreenHotkey(sdlEvent))
                {
                    break;
                }
#endif
                sink->OnKeyEvent(static_cast<int>(sdlEvent.key.key),
                                 sdlEvent.type == SDL_EVENT_KEY_DOWN,
                                 sdlEvent.key.repeat != 0);
                break;

            case SDL_EVENT_GAMEPAD_ADDED:
                sink->OnGamepadConnected(static_cast<int>(sdlEvent.gdevice.which));
                break;

            case SDL_EVENT_GAMEPAD_REMOVED:
                sink->OnGamepadDisconnected(static_cast<int>(sdlEvent.gdevice.which));
                break;

            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            case SDL_EVENT_GAMEPAD_BUTTON_UP:
                sink->OnGamepadButton(static_cast<int>(sdlEvent.gbutton.which),
                                      sdlEvent.gbutton.button,
                                      sdlEvent.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN);
                break;

            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                sink->OnGamepadAxis(static_cast<int>(sdlEvent.gaxis.which),
                                    sdlEvent.gaxis.axis,
                                    sdlEvent.gaxis.value);
                break;

            default:
                break;
        }
    }
}
