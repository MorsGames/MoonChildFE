#include "SDL2Audio.h"

#include "AudioEngine.h"
#include "MediaCatalog.h"
#include "MP3MusicTrack.h"

#include <SDL.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern const char* FullPath(const char* file);

static constexpr int MIX_FREQUENCY = 44100;
static constexpr int MIX_CHANNELS  = 2;
static constexpr SDL_AudioFormat MIX_FORMAT = AUDIO_F32SYS;
static constexpr int MIX_BUFFER_SAMPLES = 8192;
static constexpr int MUSIC_DECODE_BATCH_FRAMES = 4096;
static constexpr int MUSIC_DECODE_SCRATCH_SAMPLES = MUSIC_DECODE_BATCH_FRAMES * 2; // worst-case stereo

struct SfxState
{
    SDL_AudioDeviceID Device = 0;
    SDL_AudioSpec Spec = {};
    bool Ready = false;
};

struct MusicState
{
    SDL_AudioStream* Converter = nullptr;
    MP3MusicTrack Source;
    float Gain = 1.0f;
    bool Active = false;
};

struct MovieState
{
    SDL_AudioStream* Converter = nullptr;
};

static SfxState Sfx;
static MusicState Music;
static MovieState Movie;
static AudioEngine Audio;

static float MixBuffer[MIX_BUFFER_SAMPLES];

static float MusicDecodeScratch[MUSIC_DECODE_SCRATCH_SAMPLES];
static float MixedAudioScratch[MIX_BUFFER_SAMPLES];

static void LockAudio()
{
    if (Sfx.Device != 0)
    {
        SDL_LockAudioDevice(Sfx.Device);
    }
}

static void UnlockAudio()
{
    if (Sfx.Device != 0)
    {
        SDL_UnlockAudioDevice(Sfx.Device);
    }
}

static bool LoadWav(const char* path, std::vector<float>& outSamples, int& outFrameCount)
{
    SDL_AudioSpec sourceSpec = {};
    Uint8* sourceData = nullptr;
    Uint32 sourceLen = 0;
    if (SDL_LoadWAV(path, &sourceSpec, &sourceData, &sourceLen) == nullptr)
    {
        printf("SDL_LoadWAV failed for %s! %s\n", path, SDL_GetError());
        return false;
    }

    SDL_AudioStream* converter = SDL_NewAudioStream(sourceSpec.format,
                                                    sourceSpec.channels,
                                                    sourceSpec.freq,
                                                    MIX_FORMAT,
                                                    MIX_CHANNELS,
                                                    MIX_FREQUENCY);
    if (converter == nullptr)
    {
        printf("SDL_NewAudioStream failed for %s! %s\n", path, SDL_GetError());
        SDL_FreeWAV(sourceData);
        return false;
    }

    if (SDL_AudioStreamPut(converter, sourceData, static_cast<int>(sourceLen)) != 0)
    {
        printf("SDL_AudioStreamPut failed for %s! %s\n", path, SDL_GetError());
        SDL_FreeAudioStream(converter);
        SDL_FreeWAV(sourceData);
        return false;
    }

    SDL_FreeWAV(sourceData);
    SDL_AudioStreamFlush(converter);

    const int convertedLen = SDL_AudioStreamAvailable(converter);
    if (convertedLen <= 0)
    {
        SDL_FreeAudioStream(converter);
        return false;
    }

    const int sampleCount = convertedLen / static_cast<int>(sizeof(float));
    outSamples.resize(sampleCount);
    const int got = SDL_AudioStreamGet(converter, outSamples.data(), convertedLen);
    SDL_FreeAudioStream(converter);

    if (got <= 0)
    {
        return false;
    }

    outSamples.resize(got / static_cast<int>(sizeof(float)));
    outFrameCount = static_cast<int>(outSamples.size()) / MIX_CHANNELS;
    return outFrameCount > 0;
}

static void CleanUpMusicStuff(MusicState& musicState)
{
    SDL_FreeAudioStream(musicState.Converter);
    musicState.Converter = nullptr;
    musicState.Source.Close();
    musicState.Active = false;
}

static void FeedMusicConverter(int neededBytes)
{
    if (!Music.Active || !Music.Source.IsOpen() || Music.Converter == nullptr)
    {
        return;
    }

    const int channels = Music.Source.GetChannelCount();
    const int bytesPerFrame = channels * static_cast<int>(sizeof(float));
    if (bytesPerFrame <= 0 || neededBytes <= 0)
    {
        return;
    }

    while (SDL_AudioStreamAvailable(Music.Converter) < neededBytes)
    {
        const int framesToRead = std::min((neededBytes + bytesPerFrame - 1) / bytesPerFrame,
                                          MUSIC_DECODE_BATCH_FRAMES);
        const size_t framesRead = Music.Source.ReadFrames(MusicDecodeScratch, static_cast<size_t>(framesToRead));
        if (framesRead == 0)
        {
            break;
        }

        const int bytes = static_cast<int>(framesRead) * channels * static_cast<int>(sizeof(float));
        if (SDL_AudioStreamPut(Music.Converter, MusicDecodeScratch, bytes) != 0)
        {
            printf("SDL_AudioStreamPut failed for music! %s\n", SDL_GetError());
            break;
        }
    }
}

static void MixMusic(float* dst, int sampleCount)
{
    if (!Music.Active || Music.Converter == nullptr)
    {
        return;
    }

    const int neededBytes = sampleCount * static_cast<int>(sizeof(float));
    FeedMusicConverter(neededBytes);

    const int got = SDL_AudioStreamGet(Music.Converter, MixedAudioScratch, neededBytes);
    if (got <= 0)
    {
        return;
    }

    const int gotSamples = got / static_cast<int>(sizeof(float));
    const float gain = Music.Gain;
    for (int i = 0; i < gotSamples; i++)
    {
        dst[i] += MixedAudioScratch[i] * gain;
    }
}

static void MixMovie(float* dst, int sampleCount)
{
    if (Movie.Converter == nullptr)
    {
        return;
    }

    const int neededBytes = sampleCount * static_cast<int>(sizeof(float));
    const int got = SDL_AudioStreamGet(Movie.Converter, MixedAudioScratch, neededBytes);
    if (got <= 0)
    {
        return;
    }

    const int gotSamples = got / static_cast<int>(sizeof(float));
    for (int i = 0; i < gotSamples; i++)
    {
        dst[i] += MixedAudioScratch[i];
    }
}

static void SDLCALL FeedSfx(void* /*UserData*/, Uint8* stream, int len)
{
    int bytesRemaining = len;
    Uint8* writePtr = stream;

    while (bytesRemaining > 0)
    {
        int sampleCount = bytesRemaining / static_cast<int>(sizeof(float));
        if (sampleCount > MIX_BUFFER_SAMPLES)
        {
            sampleCount = MIX_BUFFER_SAMPLES;
        }

        const int byteCount = sampleCount * static_cast<int>(sizeof(float));
        if (byteCount <= 0)
        {
            std::memset(writePtr, 0, bytesRemaining);
            break;
        }

        std::memset(MixBuffer, 0, byteCount);

        MixMusic(MixBuffer, sampleCount);
        MixMovie(MixBuffer, sampleCount);

        Audio.MixVoicesInto(MixBuffer, sampleCount);

        for (int i = 0; i < sampleCount; i++)
        {
            MixBuffer[i] = SDL_clamp(MixBuffer[i], -1.0f, 1.0f);
        }

        std::memcpy(writePtr, MixBuffer, byteCount);
        writePtr += byteCount;
        bytesRemaining -= byteCount;
    }
}

SDL2Audio::SDL2Audio() = default;

SDL2Audio::~SDL2Audio()
{
    Destroy();
}

bool SDL2Audio::Init()
{
    if (Sfx.Ready)
    {
        return true;
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        printf("SDL audio subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec desired = {};
    desired.format = MIX_FORMAT;
    desired.channels = MIX_CHANNELS;
    desired.freq = MIX_FREQUENCY;
    desired.samples = MIX_BUFFER_SAMPLES / MIX_CHANNELS;
    desired.callback = FeedSfx;

    Sfx.Device = SDL_OpenAudioDevice(nullptr, 0, &desired, &Sfx.Spec, 0);
    if (Sfx.Device == 0)
    {
        printf("SDL_OpenAudioDevice failed! %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    SDL_PauseAudioDevice(Sfx.Device, 0);

    Sfx.Ready = true;
    return true;
}

void SDL2Audio::Destroy()
{
    StopMusic();
    Reset();

    if (Sfx.Device != 0)
    {
        SDL_CloseAudioDevice(Sfx.Device);
        Sfx.Device = 0;
    }

    if (Sfx.Ready)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        Sfx.Ready = false;
    }
}

SoundHandle SDL2Audio::CreateSound(int soundId, int maxPolyphony)
{
    if (!Init())
    {
        return 0;
    }

    return Audio.CreateSound(soundId, maxPolyphony, LoadWav);
}

void SDL2Audio::DestroySound(SoundHandle sound)
{
    LockAudio();
    Audio.DestroySound(sound);
    UnlockAudio();
}

void SDL2Audio::PlayOneShot(SoundHandle sound, int32_t volume, int32_t pan)
{
    LockAudio();
    Audio.PlayOneShot(sound, volume, pan);
    UnlockAudio();
}

void SDL2Audio::PlayLoop(SoundHandle sound, int32_t volume, int32_t pan)
{
    LockAudio();
    Audio.PlayLoop(sound, volume, pan);
    UnlockAudio();
}

void SDL2Audio::StopSound(SoundHandle sound)
{
    LockAudio();
    Audio.StopSound(sound);
    UnlockAudio();
}

void SDL2Audio::StopCurrent(SoundHandle sound)
{
    LockAudio();
    Audio.StopCurrent(sound);
    UnlockAudio();
}

void SDL2Audio::SetVolume(SoundHandle sound, int32_t volume)
{
    LockAudio();
    Audio.SetVolume(sound, volume);
    UnlockAudio();
}

void SDL2Audio::SetPan(SoundHandle sound, int32_t pan)
{
    LockAudio();
    Audio.SetPan(sound, pan);
    UnlockAudio();
}

void SDL2Audio::Reset()
{
    CloseMovieStream();

    LockAudio();

    Audio.Reset();

    UnlockAudio();
}

bool SDL2Audio::PlayMusic(int trackNumber)
{
    const MusicTrack* track = GetMusicTrack(trackNumber);
    if (track == nullptr)
    {
        return false;
    }

    if (!Init())
    {
        return false;
    }

    StopMusic();

    char relBuf[512];
    std::snprintf(relBuf, sizeof(relBuf), "mp3/%s", track->Path);
    const char* path = FullPath(relBuf);

    if (!Music.Source.Open(path))
    {
        printf("Opening MP3MusicTrack failed! %s\n", path);
        return false;
    }

    SDL_AudioStream* converter = SDL_NewAudioStream(MIX_FORMAT,
                                                    Music.Source.GetChannelCount(),
                                                    Music.Source.GetSampleRate(),
                                                    MIX_FORMAT,
                                                    MIX_CHANNELS,
                                                    MIX_FREQUENCY);
    if (converter == nullptr)
    {
        printf("SDL_NewAudioStream failed for music! %s\n", SDL_GetError());
        Music.Source.Close();
        return false;
    }

    LockAudio();

    Music.Converter = converter;
    Music.Gain = track->Volume;
    Music.Active = true;

    UnlockAudio();

    return true;
}

void SDL2Audio::StopMusic()
{
    LockAudio();
    CleanUpMusicStuff(Music);
    UnlockAudio();
}

bool SDL2Audio::OpenMovieStream(int sampleRate, int channels)
{
    CloseMovieStream();

    if (sampleRate <= 0 || channels <= 0 || channels > 2)
    {
        return false;
    }

    if (!Init())
    {
        return false;
    }

    SDL_AudioStream* converter = SDL_NewAudioStream(MIX_FORMAT,
                                                    static_cast<Uint8>(channels),
                                                    sampleRate,
                                                    MIX_FORMAT,
                                                    MIX_CHANNELS,
                                                    MIX_FREQUENCY);
    if (converter == nullptr)
    {
        printf("SDL_NewAudioStream failed for movie! %s\n", SDL_GetError());
        return false;
    }

    LockAudio();
    Movie.Converter = converter;
    UnlockAudio();

    return true;
}

void SDL2Audio::SubmitMovieAudio(const float* samples, int sampleCount)
{
    if (Sfx.Device == 0 || samples == nullptr || sampleCount <= 0)
    {
        return;
    }

    const int bytes = sampleCount * static_cast<int>(sizeof(float));

    LockAudio();

    SDL_AudioStream* converter = Movie.Converter;
    if (converter != nullptr)
    {
        SDL_AudioStreamPut(converter, samples, bytes);
    }

    UnlockAudio();
}

void SDL2Audio::CloseMovieStream()
{
    if (Sfx.Device == 0)
    {
        SDL_FreeAudioStream(Movie.Converter);
        Movie.Converter = nullptr;
        return;
    }

    LockAudio();

    SDL_AudioStream* converter = Movie.Converter;
    Movie.Converter = nullptr;

    UnlockAudio();

    if (converter != nullptr)
    {
        SDL_FreeAudioStream(converter);
    }
}
