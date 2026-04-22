#pragma once

#include "PlatformBackends.h"

#include <chrono>
#include <cstdint>
#include <memory>

class Host
{
public:
    Host();
    ~Host();

    int Run();

private:
#ifdef __EMSCRIPTEN__
    friend void EmscriptenMainLoop(void* userData);
#endif

    bool Initialize();
    void Shutdown();
    void RunFrame();

    void DetachBridges();

    bool InitializeFramework();
    void ShutdownFramework();
    void TickFramework(double tickDuration);

    PlatformBackends Backends;
    bool Running = true;
    bool Ready = false;
    uint64_t Accumulator = 0;
    std::chrono::steady_clock::time_point PreviousFrameTime;
    std::unique_ptr<unsigned char[]> PixelBuffer;
};
