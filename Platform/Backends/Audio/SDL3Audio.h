#pragma once

#include "IAudio.h"

class SDL3Audio final : public IAudio
{
public:
    SDL3Audio();
    ~SDL3Audio() override;

    bool Init() override;
    void Destroy() override;

    SoundHandle CreateSound(int soundId, int maxPolyphony) override;
    void DestroySound(SoundHandle sound) override;

    void PlayOneShot(SoundHandle sound, int32_t volume, int32_t pan) override;
    void PlayLoop(SoundHandle sound, int32_t volume, int32_t pan) override;

    void StopSound(SoundHandle sound) override;
    void StopCurrent(SoundHandle sound) override;

    void SetVolume(SoundHandle sound, int32_t volume) override;
    void SetPan(SoundHandle sound, int32_t pan) override;

    void Reset() override;

    bool PlayMusic(int trackNumber) override;
    void StopMusic() override;

    bool OpenMovieStream(int sampleRate, int channels) override;
    void SubmitMovieAudio(const float* samples, int sampleCount) override;
    void CloseMovieStream() override;
};
