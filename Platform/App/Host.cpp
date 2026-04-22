#include "Host.h"

#include "AudioBridge.h"
#include "DisplayBridge.h"
#include "InputBridge.h"
#include "MovieBridge.h"
#include "PlatformConfig.h"
#include "Resources.h"

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
extern unsigned short gamespeedflg;
#endif

#define _IN_MAIN
#include "frm_int.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <thread>
#endif

extern int g_SettingsFlg;
extern int g_ReqKeyFlg;
extern int g_KeyTimeOut;
extern int frmwrk_CenterX;
extern int frmwrk_CenterY;
extern void framework_EventHandle(int event, int param);
extern HEARTBEAT_FN framework_InitGame(Cvideo* video, Caudio* audio, Ctimer* timer, Cmovie* movie);
extern void framework_ExitGame(void);

Host::Host()
{
    Backends = MakeDefaultBackends();
}

Host::~Host() = default;

bool Host::Initialize()
{
    Running = true;
    Ready = false;
    Accumulator = 0;

    if (!Backends.Window->Create("Moon Child FE", 640, 480))
    {
        return false;
    }

    if (!Backends.Renderer->Init(Backends.Window.get()))
    {
        printf("Renderer backend initialization failed!\n");
        Backends.Window->Destroy();
        return false;
    }

    if (!Backends.Input->Init())
    {
        printf("Input backend initialization failed!\n");
        Backends.Renderer->Destroy();
        Backends.Window->Destroy();
        return false;
    }

    if (!Backends.Audio->Init())
    {
        printf("Audio backend initialization failed!\n");
        Backends.Input->Destroy();
        Backends.Renderer->Destroy();
        Backends.Window->Destroy();
        return false;
    }

    PixelBuffer.reset(new unsigned char[GAME_FRAMEBUFFER_WIDTH * GAME_FRAMEBUFFER_HEIGHT * 4]);
    std::memset(PixelBuffer.get(), 0, GAME_FRAMEBUFFER_WIDTH * GAME_FRAMEBUFFER_HEIGHT * 4);

    AudioBridge::Attach(Backends.Audio.get());
    DisplayBridge::Attach(Backends.Window.get(), Backends.Renderer.get());
    InputBridge::Attach();
    MovieBridge::Attach(Backends.Audio.get(),
        PixelBuffer.get(), GAME_FRAMEBUFFER_WIDTH, GAME_FRAMEBUFFER_HEIGHT);

    if (!InitializeFramework())
    {
        printf("Framework initialization failed!\n");
        DetachBridges();
        Backends.Audio->Destroy();
        Backends.Input->Destroy();
        Backends.Renderer->Destroy();
        Backends.Window->Destroy();
        PixelBuffer.reset();
        return false;
    }

    LoadProgress();

    PreviousFrameTime = std::chrono::steady_clock::now();
    Ready = true;
    return true;
}

void Host::Shutdown()
{
    if (!Ready)
    {
        return;
    }

    SaveProgress();
    ShutdownFramework();

    DetachBridges();

    Backends.Audio->Destroy();
    Backends.Input->Destroy();
    Backends.Renderer->Destroy();
    Backends.Window->Destroy();

    PixelBuffer.reset();
    Ready = false;
}

void Host::RunFrame()
{
    if (!Running)
    {
        return;
    }

    bool exitRequested = false;
    Backends.Window->PumpOSEvents(Backends.Input.get(), exitRequested);
    if (exitRequested)
    {
        Running = false;
    }

    InputEvent inputEvent;
    while (Backends.Input->PollNext(inputEvent))
    {
        if (inputEvent.GameKeyCode != 0)
        {
            framework_EventHandle(inputEvent.IsDown ? FW_KEYDOWN : FW_KEYUP, inputEvent.GameKeyCode);
        }
    }

    const auto current = std::chrono::steady_clock::now(); // cato wouldn't be proud of me

    const uint64_t deltaNs = static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(current - PreviousFrameTime).count()); // c++ is a mess

    PreviousFrameTime = current;

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
    const uint64_t tickNs =
        (gamespeedflg == 1u) ? GAME_FRAME_DURATION_NS_50 : GAME_FRAME_DURATION_NS_60;
#else
    const uint64_t tickNs = GAME_FRAME_DURATION_NS_60;
#endif
    const double tickDuration = tickNs / 1e9;

    Accumulator += deltaNs;
    Accumulator = std::min<uint64_t>(Accumulator, tickNs * 5ull);

    while (Accumulator >= tickNs && Running)
    {
        TickFramework(tickDuration);
        Accumulator -= tickNs;
    }

    Backends.Renderer->BeginFrame();
    Backends.Renderer->DrawFrame(PixelBuffer.get(), GAME_FRAMEBUFFER_WIDTH, GAME_FRAMEBUFFER_HEIGHT);
    Backends.Renderer->EndFrame();
}

void Host::DetachBridges()
{
    MovieBridge::Detach();
    InputBridge::Detach();
    DisplayBridge::Detach();
    AudioBridge::Detach();
}

static void ResetFrameworkState()
{
    g_SettingsFlg = 0;
    g_ReqKeyFlg = 0;
    g_KeyTimeOut = 0;
    frmwrk_CenterX = 0;
    frmwrk_CenterY = 0;
    gbGameLoop = true;
    heartbeat = nullptr;
    lvideo = nullptr;
    laudio = nullptr;
    ltimer = nullptr;
    lmovie = nullptr;
}

bool Host::InitializeFramework()
{
    std::unique_ptr<Cvideo> video(new Cvideo());
    std::unique_ptr<Caudio> audio(new Caudio());
    std::unique_ptr<Ctimer> timer(new Ctimer());
    std::unique_ptr<Cmovie> movie(new Cmovie(audio.get()));

    ResetFrameworkState();
    lvideo = video.get();
    laudio = audio.get();
    ltimer = timer.get();
    lmovie = movie.get();

    if (!lvideo->on(PixelBuffer.get(), GAME_FRAMEBUFFER_WIDTH, GAME_FRAMEBUFFER_HEIGHT, 256))
    {
        ResetFrameworkState();
        return false;
    }

    heartbeat = framework_InitGame(lvideo, laudio, ltimer, lmovie);
    if (heartbeat == nullptr)
    {
        ResetFrameworkState();
        return false;
    }

    video.release();
    audio.release();
    timer.release();
    movie.release();
    return true;
}

void Host::ShutdownFramework()
{
    framework_ExitGame();

    delete lmovie;
    delete ltimer;
    delete laudio;
    delete lvideo;

    ResetFrameworkState();
}

void Host::TickFramework(double tickDuration)
{
    if (lmovie->videoFilename != nullptr)
    {
        MovieBridge::Tick(tickDuration);
    }

    heartbeat = reinterpret_cast<HEARTBEAT_FN>(heartbeat());
    if (heartbeat == nullptr)
    {
        Running = false;
    }
}

#ifdef __EMSCRIPTEN__
void EmscriptenMainLoop(void* userData)
{
    Host* host = static_cast<Host*>(userData);
    host->RunFrame();
    if (!host->Running)
    {
        host->Shutdown();
        emscripten_cancel_main_loop();
    }
}
#endif

int Host::Run()
{
    if (!Initialize())
    {
        return 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(EmscriptenMainLoop, this, 0, 1);
#else
    while (Running)
    {
        RunFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Shutdown();
#endif

    return 0;
}
