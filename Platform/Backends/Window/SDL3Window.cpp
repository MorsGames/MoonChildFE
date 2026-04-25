#include "SDL3Window.h"

#include "DisplayBridge.h"
#include "IInput.h"

#include <cstdio>

SDL3Window::SDL3Window() = default;

SDL3Window::~SDL3Window()
{
    Destroy();
}

/*
 * NOTE: Only video + events are initialized here; audio & input are initialized in
 *       SDL3Audio & SDL3Input respectively.
 */
constexpr static SDL_InitFlags MY_SDL_SUBSYSTEM_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS;

bool SDL3Window::Create(const char* title, int width, int height)
{
    if (!SDL_SetAppMetadata(title, NULL, NULL))
    {
        printf("SDL set app metadata failed! %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_InitSubSystem(MY_SDL_SUBSYSTEM_FLAGS))
    {
        printf("SDL video + init subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

    SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE;
    
#ifdef MOONCHILD_RENDERER_GL
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);

    windowFlags |= SDL_WINDOW_OPENGL;
#endif

    m_Window = SDL_CreateWindow(title, width, height, windowFlags);
    if (m_Window == nullptr)
    {
        printf("SDL window creation failed! %s\n", SDL_GetError());
        return false;
    }

    SDL_DisplayID displayId = SDL_GetDisplayForWindow(m_Window);
    float scale = SDL_GetDisplayContentScale(displayId);
    if (scale != 1.0f)
    {
        SDL_SetWindowSize(m_Window, width * scale, height * scale);
        SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }

#ifdef MOONCHILD_RENDERER_GL
    m_GlContext = SDL_GL_CreateContext(m_Window);
    if (m_GlContext == nullptr)
    {
        printf("SDL OpenGL context creation failed! %s\n", SDL_GetError());
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
        return false;
    }

    SDL_GL_MakeCurrent(m_Window, m_GlContext);
#endif
    return true;
}

void SDL3Window::Destroy()
{
#ifdef MOONCHILD_RENDERER_GL
    if (m_GlContext != nullptr)
    {
        SDL_GL_DestroyContext(m_GlContext);
        m_GlContext = nullptr;
    }
#endif
    if (m_Window != nullptr)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    SDL_QuitSubSystem(MY_SDL_SUBSYSTEM_FLAGS);
}

WindowSize SDL3Window::GetPixelSize() const
{
    WindowSize size;
    SDL_GetWindowSize(m_Window, &size.Width, &size.Height);
    return size;
}

void SDL3Window::DisplaySetFullscreen(bool enabled)
{
    SDL_SetWindowFullscreen(m_Window, enabled);
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
        
        const bool currentlyFullscreen = (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN) != 0;
        const bool targetFullscreen = !currentlyFullscreen;
        if (SDL_SetWindowFullscreen(m_Window, targetFullscreen))
        {
            DisplayBridge::NotifyFullscreenChange(targetFullscreen ? 1 : 0);
        }
        m_SwallowEnterKey = true; // gulp!
        return true;
    }

    if (m_SwallowEnterKey)
    {
        m_SwallowEnterKey = false;
        return true;
    }

    return false;
}
#endif // MOONCHILD_HAS_DISPLAY_OPTIONS

#ifdef MOONCHILD_RENDERER_GL
void SDL3Window::MakeCurrent()
{
    SDL_GL_MakeCurrent(m_Window, m_GlContext);
}

void SDL3Window::SwapBuffers()
{
    SDL_GL_SwapWindow(m_Window);
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
                DisplayBridge::NotifyFullscreenChange(1);
                break;

            case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
                DisplayBridge::NotifyFullscreenChange(0);
                break;

            case SDL_EVENT_WINDOW_FOCUS_LOST:
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
                m_SwallowEnterKey = false;
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
