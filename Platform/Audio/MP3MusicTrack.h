#pragma once

#include <cstddef>

class MP3MusicTrack
{
public:
    MP3MusicTrack();
    ~MP3MusicTrack();

    MP3MusicTrack(const MP3MusicTrack&) = delete;
    MP3MusicTrack& operator=(const MP3MusicTrack&) = delete;

    bool Open(const char* path);
    void Close();
    bool IsOpen() const { return Ready; }

    int GetChannelCount() const { return ChannelCount; }
    int GetSampleRate() const { return SampleRate; }

    size_t ReadFrames(float* out, size_t frameCount);

private:
    void* Decoder = nullptr;
    bool Ready = false;
    int ChannelCount = 0;
    int SampleRate = 0;
};
