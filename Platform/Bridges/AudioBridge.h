#pragma once

class IAudio;

namespace AudioBridge
{
    void Attach(IAudio* audio);
    void Detach();
}
