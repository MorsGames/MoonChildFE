#pragma once

#include <cstdint>
#include <stddef.h>
#include <vector>

struct SoundAsset
{
    int SoundId = 0;
    int MaxPolyphony = 1;
    std::vector<float> Samples;
    int FrameCount = 0;
};

struct ActiveVoice
{
    SoundAsset* Asset = nullptr;
    size_t FrameOffset = 0;
    float GainLeft = 1.0f;
    float GainRight = 1.0f;
    int32_t Volume = 0;
    int32_t Pan = 0;
    bool Loop = false;
    bool Active = false;
    uint64_t PlayId = 0;
};

class AudioMixer
{
public:
    static constexpr int MIX_CHANNELS = 2;
    static constexpr int MAX_VOICES = 64;

    static void CalculateGain(int32_t volume, int32_t pan, float& outLeft, float& outRight);

    void MixVoicesInto(float* mixBuffer, int sampleCount);
    ActiveVoice* AcquireVoiceSlot(SoundAsset* asset);
    void DeactivateVoicesFor(const SoundAsset* asset);
    void ResetAllVoices();

    ActiveVoice Voices[MAX_VOICES];
    uint64_t NextPlayId = 1;
};
