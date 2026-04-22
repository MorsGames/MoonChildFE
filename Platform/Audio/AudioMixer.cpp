#include "AudioMixer.h"

#include <cmath>

static float CalculateVolume(int32_t volume)
{
    if (volume < -4000)
        volume = -4000;
    if (volume > 0)
        volume = 0;

    return (volume + 4000) / 4000.0f;
}

static float CalculatePan(int32_t pan)
{
    if (pan < -1000)
        pan = -1000;
    if (pan > 1000)
        pan = 1000;

    return pan / 1000.0f;
}

void AudioMixer::CalculateGain(int32_t originalVolume, int32_t originalPan, float& outLeft, float& outRight)
{
    const float gain = CalculateVolume(originalVolume);
    const float pan = CalculatePan(originalPan);

    // Apparently this is the pro way of doing it
    constexpr float quarterPi = 0.7853981633974483f;
    const float angle = (pan + 1.0f) * quarterPi;
    outLeft = gain * std::cos(angle);
    outRight = gain * std::sin(angle);
}

void AudioMixer::MixVoicesInto(float* mixBuffer, int sampleCount)
{
    const int framesRequested = sampleCount / MIX_CHANNELS;

    for (ActiveVoice& voice : Voices)
    {
        if (!voice.Active || voice.Asset == nullptr)
        {
            continue;
        }

        for (int frame = 0; frame < framesRequested; frame++)
        {
            if (voice.FrameOffset >= static_cast<size_t>(voice.Asset->FrameCount))
            {
                if (voice.Loop)
                {
                    voice.FrameOffset = 0;
                }
                else
                {
                    voice.Active = false;
                    voice.Asset = nullptr;
                    break;
                }
            }

            const size_t sampleIndex = voice.FrameOffset * MIX_CHANNELS;
            mixBuffer[frame * 2] += voice.Asset->Samples[sampleIndex] * voice.GainLeft;
            mixBuffer[frame * 2 + 1] += voice.Asset->Samples[sampleIndex + 1] * voice.GainRight;
            voice.FrameOffset++;
        }
    }
}

ActiveVoice* AudioMixer::AcquireVoiceSlot(SoundAsset* asset)
{
    int matching = 0;
    ActiveVoice* oldestMatching = nullptr;
    ActiveVoice* firstInactive = nullptr;
    ActiveVoice* oldestActive = nullptr;

    for (ActiveVoice& voice : Voices)
    {
        if (!voice.Active)
        {
            if (firstInactive == nullptr)
            {
                firstInactive = &voice;
            }
            continue;
        }
        if (oldestActive == nullptr || voice.PlayId < oldestActive->PlayId)
        {
            oldestActive = &voice;
        }
        if (asset != nullptr && voice.Asset == asset)
        {
            matching++;
            if (oldestMatching == nullptr || voice.PlayId < oldestMatching->PlayId)
            {
                oldestMatching = &voice;
            }
        }
    }

    if (asset != nullptr && matching >= asset->MaxPolyphony)
    {
        return oldestMatching;
    }
    if (firstInactive != nullptr)
    {
        return firstInactive;
    }
    return oldestActive;
}

void AudioMixer::DeactivateVoicesFor(const SoundAsset* asset)
{
    for (ActiveVoice& voice : Voices)
    {
        if (voice.Active && voice.Asset == asset)
        {
            voice.Active = false;
            voice.Asset = nullptr;
        }
    }
}

void AudioMixer::ResetAllVoices()
{
    for (ActiveVoice& voice : Voices)
    {
        voice.Active = false;
        voice.Asset = nullptr;
    }
}
