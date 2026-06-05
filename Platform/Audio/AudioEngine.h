#pragma once

#include "IAudio.h"

#include <cstddef>
#include <cstdint>
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

class AudioEngine
{
public:
    using LoadSoundSamplesFn = bool (*)(const char* path, std::vector<float>& outSamples, int& outFrameCount);

    SoundHandle CreateSound(int soundId, int maxPolyphony, LoadSoundSamplesFn loadSamples);
    void DestroySound(SoundHandle sound);

    void PlayOneShot(SoundHandle sound, int32_t volume, int32_t pan);
    void PlayLoop(SoundHandle sound, int32_t volume, int32_t pan);

    void StopSound(SoundHandle sound);
    void StopCurrent(SoundHandle sound);

    void SetVolume(SoundHandle sound, int32_t volume);
    void SetPan(SoundHandle sound, int32_t pan);

    void Reset();
    void MixVoicesInto(float* mixBuffer, int sampleCount);

private:
    static constexpr int MIX_CHANNELS = 2;
    static constexpr int MAX_VOICES = 64;

    static void CalculateGain(int32_t volume, int32_t pan, float& outLeft, float& outRight);

    ActiveVoice* AcquireVoiceSlot(SoundAsset* asset);
    void DeactivateVoicesFor(const SoundAsset* asset);
    void ResetAllVoices();

    ActiveVoice Voices[MAX_VOICES];
    uint64_t NextPlayId = 1;
};
