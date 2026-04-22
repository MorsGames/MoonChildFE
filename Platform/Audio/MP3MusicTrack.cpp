#include "MP3MusicTrack.h"

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

static drmp3* AsDecoder(void* handle)
{
    return static_cast<drmp3*>(handle);
}

MP3MusicTrack::MP3MusicTrack() = default;

MP3MusicTrack::~MP3MusicTrack()
{
    Close();
}

bool MP3MusicTrack::Open(const char* path)
{
    Close();

    drmp3* decoder = new drmp3();
    if (!drmp3_init_file(decoder, path, nullptr))
    {
        delete decoder;
        return false;
    }

    Decoder = decoder;
    Ready = true;
    ChannelCount = static_cast<int>(decoder->channels);
    SampleRate = static_cast<int>(decoder->sampleRate);
    return true;
}

void MP3MusicTrack::Close()
{
    if (Decoder != nullptr)
    {
        drmp3* mp3 = AsDecoder(Decoder);
        drmp3_uninit(mp3);
        delete mp3;
        Decoder = nullptr;
    }

    Ready = false;
    ChannelCount = 0;
    SampleRate = 0;
}

size_t MP3MusicTrack::ReadFrames(float* out, size_t frameCount)
{
    if (!Ready || out == nullptr || frameCount == 0)
    {
        return 0;
    }

    drmp3* decoder = AsDecoder(Decoder);
    size_t totalRead = 0;

    while (totalRead < frameCount)
    {
        const drmp3_uint64 framesRequested = static_cast<drmp3_uint64>(frameCount - totalRead);
        float* dest = out + totalRead * ChannelCount;

        const drmp3_uint64 framesRead = drmp3_read_pcm_frames_f32(decoder, framesRequested, dest);
        totalRead += static_cast<size_t>(framesRead);

        if (framesRead < framesRequested)
        {
            if (!drmp3_seek_to_pcm_frame(decoder, 0))
            {
                break;
            }
        }
    }

    return totalRead;
}
