#include "PlatformBackends.h"

#ifdef MOONCHILD_WINDOW_SDL3
#include "SDL3Window.h"
#endif

#ifdef MOONCHILD_WINDOW_SDL2
#include "SDL2Window.h"
#endif

#ifdef MOONCHILD_RENDERER_OPENGL
#include "OpenGLRenderer.h"
#endif

#ifdef MOONCHILD_RENDERER_GLES
#include "GLESRenderer.h"
#endif

#ifdef MOONCHILD_INPUT_SDL3
#include "SDL3Input.h"
#endif

#ifdef MOONCHILD_INPUT_SDL2
#include "SDL2Input.h"
#endif

#ifdef MOONCHILD_AUDIO_SDL3
#include "SDL3Audio.h"
#endif

#ifdef MOONCHILD_AUDIO_SDL2
#include "SDL2Audio.h"
#endif

PlatformBackends MakeDefaultBackends()
{
    PlatformBackends backends;

#if defined(MOONCHILD_WINDOW_SDL2)
    backends.Window.reset(new SDL2Window());
#elif defined(MOONCHILD_WINDOW_SDL3)
    backends.Window.reset(new SDL3Window());
#endif

#if defined(MOONCHILD_RENDERER_OPENGL)
    backends.Renderer.reset(new OpenGLRenderer());
#elif defined(MOONCHILD_RENDERER_GLES)
    backends.Renderer.reset(new GLESRenderer());
#endif

#if defined(MOONCHILD_INPUT_SDL2)
    backends.Input.reset(new SDL2Input());
#elif defined(MOONCHILD_INPUT_SDL3)
    backends.Input.reset(new SDL3Input());
#endif

#if defined(MOONCHILD_AUDIO_SDL2)
    backends.Audio.reset(new SDL2Audio());
#elif defined(MOONCHILD_AUDIO_SDL3)
    backends.Audio.reset(new SDL3Audio());
#endif

    return backends;
}
