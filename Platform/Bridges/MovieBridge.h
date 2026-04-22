#pragma once

#include <cstdint>

class IAudio;

namespace MovieBridge
{
    void Attach(IAudio* audio, uint8_t* framebuffer, int width, int height);
    void Detach();
    
    void Tick(double deltaSeconds);
}
