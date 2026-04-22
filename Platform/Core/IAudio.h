#pragma once

#include <cstdint>

using SoundHandle = intptr_t;

class IAudio
{
public:
    virtual ~IAudio() = default;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual SoundHandle CreateSound(int soundId, int maxPolyphony) = 0;
    virtual void DestroySound(SoundHandle sound) = 0;

    virtual void PlayOneShot(SoundHandle sound, int32_t volume, int32_t pan) = 0;
    virtual void PlayLoop(SoundHandle sound, int32_t volume, int32_t pan) = 0;

    virtual void StopSound(SoundHandle sound) = 0;
    virtual void StopCurrent(SoundHandle sound) = 0;

    virtual void SetVolume(SoundHandle sound, int32_t volume) = 0;
    virtual void SetPan(SoundHandle sound, int32_t pan) = 0;

    virtual void Reset() = 0;

    virtual bool PlayMusic(int trackNumber) = 0;
    virtual void StopMusic() = 0;

    virtual bool OpenMovieStream(int sampleRate, int channels) = 0;
    virtual void SubmitMovieAudio(const float* samples, int sampleCount) = 0;
    virtual void CloseMovieStream() = 0;
};
