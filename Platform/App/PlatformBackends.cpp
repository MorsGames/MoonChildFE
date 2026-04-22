#include "PlatformBackends.h"

#ifdef MOONCHILD_WINDOW_SDL3
#include "SDL3Window.h"
#endif

#ifdef MOONCHILD_RENDERER_GL
#include "GLRenderer.h"
#endif

#ifdef MOONCHILD_RENDERER_SDL3
#include "SDL3Renderer.h"
#endif

#ifdef MOONCHILD_INPUT_SDL3
#include "SDL3Input.h"
#endif

#ifdef MOONCHILD_AUDIO_SDL3
#include "SDL3Audio.h"
#endif

PlatformBackends MakeDefaultBackends()
{
    PlatformBackends backends;

#ifdef MOONCHILD_WINDOW_SDL3
    backends.Window.reset(new SDL3Window());
#endif

#ifdef MOONCHILD_RENDERER_GL
    backends.Renderer.reset(new GLRenderer());
#else
#ifdef MOONCHILD_RENDERER_SDL3
    backends.Renderer.reset(new SDL3Renderer());
#endif
#endif

#ifdef MOONCHILD_INPUT_SDL3
    backends.Input.reset(new SDL3Input());
#endif

#ifdef MOONCHILD_AUDIO_SDL3
    backends.Audio.reset(new SDL3Audio());
#endif

    return backends;
}
