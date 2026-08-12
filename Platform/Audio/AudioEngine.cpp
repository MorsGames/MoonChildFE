#include "AudioEngine.h"

#include "MediaCatalog.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <memory>

extern const char* FullPath(const char* file);

static SoundAsset* ToAsset(SoundHandle handle)
{
    return reinterpret_cast<SoundAsset*>(handle);
}

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

SoundHandle AudioEngine::CreateSound(int soundId, int maxPolyphony, LoadSoundSamplesFn loadSamples)
{
    const char* name = GetSoundEffect(soundId);
    if (name == nullptr || loadSamples == nullptr)
    {
        return 0;
    }

    std::unique_ptr<SoundAsset> asset(new SoundAsset());
    asset->SoundId = soundId;
    asset->MaxPolyphony = std::max(1, maxPolyphony);

    char relBuf[512];
    std::snprintf(relBuf, sizeof(relBuf), "audio/%s", name);
    const char* path = FullPath(relBuf);
    if (!loadSamples(path, asset->Samples, asset->FrameCount))
    {
        return 0;
    }

    return reinterpret_cast<SoundHandle>(asset.release());
}

void AudioEngine::DestroySound(SoundHandle sound)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    DeactivateVoicesFor(asset);
    delete asset;
}

void AudioEngine::PlayOneShot(SoundHandle sound, int32_t volume, int32_t pan)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    ActiveVoice* slot = AcquireVoiceSlot(asset);
    slot->Asset = asset;
    slot->FrameOffset = 0;
    slot->Loop = false;
    slot->Active = true;
    slot->PlayId = NextPlayId++;
    slot->Volume = volume;
    slot->Pan = pan;
    CalculateGain(volume, pan, slot->GainLeft, slot->GainRight);
}

void AudioEngine::PlayLoop(SoundHandle sound, int32_t volume, int32_t pan)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    for (ActiveVoice& voice : Voices)
    {
        if (voice.Active && voice.Asset == asset && voice.Loop)
        {
            voice.Volume = volume;
            voice.Pan = pan;
            CalculateGain(volume, pan, voice.GainLeft, voice.GainRight);
            return;
        }
    }

    ActiveVoice* slot = AcquireVoiceSlot(asset);
    slot->Asset = asset;
    slot->FrameOffset = 0;
    slot->Loop = true;
    slot->Active = true;
    slot->PlayId = NextPlayId++;
    slot->Volume = volume;
    slot->Pan = pan;
    CalculateGain(volume, pan, slot->GainLeft, slot->GainRight);
}

void AudioEngine::StopSound(SoundHandle sound)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    DeactivateVoicesFor(asset);
}

void AudioEngine::StopCurrent(SoundHandle sound)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    ActiveVoice* newest = nullptr;
    for (ActiveVoice& voice : Voices)
    {
        if (voice.Active && voice.Asset == asset)
        {
            if (newest == nullptr || voice.PlayId > newest->PlayId)
            {
                newest = &voice;
            }
        }
    }

    if (newest != nullptr)
    {
        newest->Active = false;
        newest->Asset = nullptr;
    }
}

void AudioEngine::SetVolume(SoundHandle sound, int32_t volume)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    for (ActiveVoice& voice : Voices)
    {
        if (voice.Active && voice.Asset == asset)
        {
            voice.Volume = volume;
            CalculateGain(voice.Volume, voice.Pan, voice.GainLeft, voice.GainRight);
        }
    }
}

void AudioEngine::SetPan(SoundHandle sound, int32_t pan)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    for (ActiveVoice& voice : Voices)
    {
        if (voice.Active && voice.Asset == asset)
        {
            voice.Pan = pan;
            CalculateGain(voice.Volume, voice.Pan, voice.GainLeft, voice.GainRight);
        }
    }
}

void AudioEngine::Reset()
{
    ResetAllVoices();
}

void AudioEngine::MixVoicesInto(float* mixBuffer, int sampleCount)
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

void AudioEngine::CalculateGain(int32_t originalVolume, int32_t originalPan, float& outLeft, float& outRight)
{
    const float gain = CalculateVolume(originalVolume);
    const float pan = CalculatePan(originalPan);

    constexpr float quarterPi = 0.7853981633974483f;
    const float angle = (pan + 1.0f) * quarterPi;
    outLeft = gain * std::cos(angle);
    outRight = gain * std::sin(angle);
}

ActiveVoice* AudioEngine::AcquireVoiceSlot(SoundAsset* asset)
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
        if (voice.Asset == asset)
        {
            matching++;
            if (oldestMatching == nullptr || voice.PlayId < oldestMatching->PlayId)
            {
                oldestMatching = &voice;
            }
        }
    }

    if (matching >= asset->MaxPolyphony)
    {
        return oldestMatching;
    }
    if (firstInactive != nullptr)
    {
        return firstInactive;
    }
    return oldestActive;
}

void AudioEngine::DeactivateVoicesFor(const SoundAsset* asset)
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

void AudioEngine::ResetAllVoices()
{
    for (ActiveVoice& voice : Voices)
    {
        voice.Active = false;
        voice.Asset = nullptr;
    }
}
