#include "SDL3Audio.h"

#include "AudioMixer.h"
#include "MediaCatalog.h"
#include "MP3MusicTrack.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

extern char* FullPath(char* file);

static constexpr int MIX_FREQUENCY = 48000;
static constexpr int MIX_CHANNELS  = 2;
static constexpr SDL_AudioFormat MIX_FORMAT = SDL_AUDIO_F32;
static constexpr int MIX_BUFFER_SAMPLES = 8192;
static constexpr int MUSIC_DECODE_BATCH_FRAMES = 4096;
static constexpr int MUSIC_DECODE_SCRATCH_SAMPLES = MUSIC_DECODE_BATCH_FRAMES * 2; // worst-case stereo

struct SfxState
{
    SDL_AudioStream* Stream = nullptr;
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
static AudioMixer SfxMixer;
static MusicState Music;
static MovieState Movie;

static float MixBuffer[MIX_BUFFER_SAMPLES];

static float MusicDecodeScratch[MUSIC_DECODE_SCRATCH_SAMPLES];
static float MixedAudioScratch[MIX_BUFFER_SAMPLES];

static bool LoadWav(const char* path, std::vector<float>& outSamples, int& outFrameCount)
{
    SDL_AudioSpec sourceSpec = {};
    Uint8* sourceData = nullptr;
    Uint32 sourceLen = 0;
    if (!SDL_LoadWAV(path, &sourceSpec, &sourceData, &sourceLen))
    {
        printf("SDL_LoadWAV failed for %s! %s\n", path, SDL_GetError());
        return false;
    }

    SDL_AudioSpec targetSpec = {};
    targetSpec.format = MIX_FORMAT;
    targetSpec.channels = MIX_CHANNELS;
    targetSpec.freq = MIX_FREQUENCY;

    Uint8* convertedData = nullptr;
    int convertedLen = 0;
    const bool conversionOk = SDL_ConvertAudioSamples(&sourceSpec, sourceData, static_cast<int>(sourceLen),
                                                      &targetSpec, &convertedData, &convertedLen);
    SDL_free(sourceData);

    if (!conversionOk)
    {
        printf("SDL_ConvertAudioSamples failed for %s! %s\n", path, SDL_GetError());
        return false;
    }

    const int sampleCount = convertedLen / static_cast<int>(sizeof(float));
    outSamples.resize(sampleCount);
    std::memcpy(outSamples.data(), convertedData, convertedLen);
    SDL_free(convertedData);

    outFrameCount = sampleCount / MIX_CHANNELS;
    return outFrameCount > 0;
}

static void CleanUpMusicStuff(MusicState& musicState)
{
    SDL_DestroyAudioStream(musicState.Converter);
    musicState.Converter = nullptr;
    musicState.Source.Close();
    musicState.Active = false;
}

static void SDLCALL FeedMusicConverter(void* /*UserData*/, SDL_AudioStream* stream,
                                       int AdditionalAmount, int /*TotalAmount*/)
{
    if (!Music.Active || !Music.Source.IsOpen())
    {
        return;
    }

    const int channels = Music.Source.GetChannelCount();
    const int bytesPerFrame = channels * static_cast<int>(sizeof(float));
    if (bytesPerFrame <= 0 || AdditionalAmount <= 0)
    {
        return;
    }

    const int requestedFrames = (AdditionalAmount + bytesPerFrame - 1) / bytesPerFrame;
    const int framesToRead = std::min(requestedFrames, MUSIC_DECODE_BATCH_FRAMES);

    const size_t framesRead = Music.Source.ReadFrames(MusicDecodeScratch, static_cast<size_t>(framesToRead));
    if (framesRead == 0)
    {
        return;
    }

    const int bytes = static_cast<int>(framesRead) * channels * static_cast<int>(sizeof(float));
    SDL_PutAudioStreamData(stream, MusicDecodeScratch, bytes);
}

static void MixMusic(float* dst, int sampleCount)
{
    if (!Music.Active || Music.Converter == nullptr)
    {
        return;
    }

    const int neededBytes = sampleCount * static_cast<int>(sizeof(float));
    const int got = SDL_GetAudioStreamData(Music.Converter, MixedAudioScratch, neededBytes);
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

    SDL_LockAudioStream(Movie.Converter);
    const int got = SDL_GetAudioStreamData(Movie.Converter, MixedAudioScratch, neededBytes);
    SDL_UnlockAudioStream(Movie.Converter);

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

static void SDLCALL FeedSfx(void* /*UserData*/, SDL_AudioStream* stream, int additionalAmount, int /*TotalAmount*/)
{
    int sampleCount = additionalAmount / static_cast<int>(sizeof(float));
    if (sampleCount > MIX_BUFFER_SAMPLES)
    {
        sampleCount = MIX_BUFFER_SAMPLES;
    }
    const int byteCount = sampleCount * static_cast<int>(sizeof(float));

    std::memset(MixBuffer, 0, byteCount);

    MixMusic(MixBuffer, sampleCount);
    MixMovie(MixBuffer, sampleCount);

    SfxMixer.MixVoicesInto(MixBuffer, sampleCount);

    for (int i = 0; i < sampleCount; i++)
    {
        MixBuffer[i] = SDL_clamp(MixBuffer[i], -1.0f, 1.0f);
    }

    SDL_PutAudioStreamData(stream, MixBuffer, byteCount);
}

static SoundAsset* ToAsset(SoundHandle handle)
{
    return reinterpret_cast<SoundAsset*>(handle);
}

SDL3Audio::SDL3Audio() = default;

SDL3Audio::~SDL3Audio()
{
    Destroy();
}

bool SDL3Audio::Init()
{
    if (Sfx.Ready)
    {
        return true;
    }

    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        printf("SDL audio subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

    Sfx.Spec.format = MIX_FORMAT;
    Sfx.Spec.channels = MIX_CHANNELS;
    Sfx.Spec.freq = MIX_FREQUENCY;
    Sfx.Stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &Sfx.Spec, FeedSfx, nullptr);
    if (Sfx.Stream == nullptr)
    {
        printf("SDL_OpenAudioDeviceStream failed! %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_ResumeAudioStreamDevice(Sfx.Stream))
    {
        printf("SDL_ResumeAudioStreamDevice failed! %s\n", SDL_GetError());
        SDL_DestroyAudioStream(Sfx.Stream);
        Sfx.Stream = nullptr;
        return false;
    }

    Sfx.Ready = true;
    return true;
}

void SDL3Audio::Destroy()
{
    StopMusic();
    Reset();
    SDL_DestroyAudioStream(Sfx.Stream);
    Sfx.Stream = nullptr;
    if (Sfx.Ready)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        Sfx.Ready = false;
    }
}

SoundHandle SDL3Audio::CreateSound(int soundId, int maxPolyphony)
{
    if (!Init())
    {
        return 0;
    }

    const char* name = GetSoundEffect(soundId);
    if (name == nullptr)
    {
        return 0;
    }

    std::unique_ptr<SoundAsset> asset(new SoundAsset());
    asset->SoundId = soundId;
    asset->MaxPolyphony = std::max(1, maxPolyphony);

    char relBuf[512];
    std::snprintf(relBuf, sizeof(relBuf), "audio/%s", name);
    const char* path = FullPath(relBuf);
    if (!LoadWav(path, asset->Samples, asset->FrameCount))
    {
        return 0;
    }

    return reinterpret_cast<SoundHandle>(asset.release());
}

void SDL3Audio::DestroySound(SoundHandle sound)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);
    SfxMixer.DeactivateVoicesFor(asset);
    SDL_UnlockAudioStream(Sfx.Stream);

    delete asset;
}

void SDL3Audio::PlayOneShot(SoundHandle sound, int32_t volume, int32_t pan)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);

    ActiveVoice* slot = SfxMixer.AcquireVoiceSlot(asset);
    slot->Asset = asset;
    slot->FrameOffset = 0;
    slot->Loop = false;
    slot->Active = true;
    slot->PlayId = SfxMixer.NextPlayId++;
    slot->Volume = volume;
    slot->Pan = pan;
    AudioMixer::CalculateGain(volume, pan, slot->GainLeft, slot->GainRight);

    SDL_UnlockAudioStream(Sfx.Stream);
}

void SDL3Audio::PlayLoop(SoundHandle sound, int32_t volume, int32_t pan)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);

    for (ActiveVoice& voice : SfxMixer.Voices)
    {
        if (voice.Active && voice.Asset == asset && voice.Loop)
        {
            voice.Volume = volume;
            voice.Pan = pan;
            AudioMixer::CalculateGain(volume, pan, voice.GainLeft, voice.GainRight);
            SDL_UnlockAudioStream(Sfx.Stream);
            return;
        }
    }

    ActiveVoice* slot = SfxMixer.AcquireVoiceSlot(asset);
    slot->Asset = asset;
    slot->FrameOffset = 0;
    slot->Loop = true;
    slot->Active = true;
    slot->PlayId = SfxMixer.NextPlayId++;
    slot->Volume = volume;
    slot->Pan = pan;
    AudioMixer::CalculateGain(volume, pan, slot->GainLeft, slot->GainRight);

    SDL_UnlockAudioStream(Sfx.Stream);
}

void SDL3Audio::StopSound(SoundHandle sound)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);
    SfxMixer.DeactivateVoicesFor(asset);
    SDL_UnlockAudioStream(Sfx.Stream);
}

void SDL3Audio::StopCurrent(SoundHandle sound)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);

    ActiveVoice* newest = nullptr;
    for (ActiveVoice& voice : SfxMixer.Voices)
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

    SDL_UnlockAudioStream(Sfx.Stream);
}

void SDL3Audio::SetVolume(SoundHandle sound, int32_t volume)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);

    for (ActiveVoice& voice : SfxMixer.Voices)
    {
        if (voice.Active && voice.Asset == asset)
        {
            voice.Volume = volume;
            AudioMixer::CalculateGain(voice.Volume, voice.Pan, voice.GainLeft, voice.GainRight);
        }
    }

    SDL_UnlockAudioStream(Sfx.Stream);
}

void SDL3Audio::SetPan(SoundHandle sound, int32_t pan)
{
    SoundAsset* asset = ToAsset(sound);
    if (asset == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);

    for (ActiveVoice& voice : SfxMixer.Voices)
    {
        if (voice.Active && voice.Asset == asset)
        {
            voice.Pan = pan;
            AudioMixer::CalculateGain(voice.Volume, voice.Pan, voice.GainLeft, voice.GainRight);
        }
    }

    SDL_UnlockAudioStream(Sfx.Stream);
}

void SDL3Audio::Reset()
{
    CloseMovieStream();

    SDL_LockAudioStream(Sfx.Stream);

    SfxMixer.ResetAllVoices();

    SDL_UnlockAudioStream(Sfx.Stream);
}

bool SDL3Audio::PlayMusic(int trackNumber)
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

    SDL_AudioSpec srcSpec = {};
    srcSpec.format   = SDL_AUDIO_F32;
    srcSpec.channels = Music.Source.GetChannelCount();
    srcSpec.freq     = Music.Source.GetSampleRate();

    SDL_AudioSpec dstSpec = {};
    dstSpec.format   = MIX_FORMAT;
    dstSpec.channels = MIX_CHANNELS;
    dstSpec.freq     = MIX_FREQUENCY;

    SDL_AudioStream* converter = SDL_CreateAudioStream(&srcSpec, &dstSpec);
    if (converter == nullptr)
    {
        printf("SDL_CreateAudioStream failed for music! %s\n", SDL_GetError());
        Music.Source.Close();
        return false;
    }

    if (!SDL_SetAudioStreamGetCallback(converter, FeedMusicConverter, nullptr))
    {
        printf("SDL_SetAudioStreamGetCallback failed! %s\n", SDL_GetError());
        SDL_DestroyAudioStream(converter);
        Music.Source.Close();
        return false;
    }

    SDL_LockAudioStream(Sfx.Stream);

    Music.Converter = converter;
    Music.Gain = track->Volume;
    Music.Active = true;

    SDL_UnlockAudioStream(Sfx.Stream);

    return true;
}

void SDL3Audio::StopMusic()
{
    SDL_LockAudioStream(Sfx.Stream);
    CleanUpMusicStuff(Music);
    SDL_UnlockAudioStream(Sfx.Stream);
}

bool SDL3Audio::OpenMovieStream(int sampleRate, int channels)
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

    SDL_AudioSpec srcSpec = {};
    srcSpec.format = MIX_FORMAT;
    srcSpec.channels = channels;
    srcSpec.freq = sampleRate;

    SDL_AudioSpec dstSpec = {};
    dstSpec.format = MIX_FORMAT;
    dstSpec.channels = MIX_CHANNELS;
    dstSpec.freq = MIX_FREQUENCY;

    SDL_AudioStream* converter = SDL_CreateAudioStream(&srcSpec, &dstSpec);
    if (converter == nullptr)
    {
        printf("SDL_CreateAudioStream failed for movie! %s\n", SDL_GetError());
        return false;
    }

    Movie.Converter = converter;
    return true;
}

void SDL3Audio::SubmitMovieAudio(const float* samples, int sampleCount)
{
    if (Movie.Converter == nullptr || samples == nullptr || sampleCount <= 0)
    {
        return;
    }

    const int bytes = sampleCount * static_cast<int>(sizeof(float));

    SDL_LockAudioStream(Movie.Converter);
    SDL_PutAudioStreamData(Movie.Converter, samples, bytes);
    SDL_UnlockAudioStream(Movie.Converter);
}

void SDL3Audio::CloseMovieStream()
{
    SDL_AudioStream* converter = Movie.Converter;
    if (converter == nullptr)
    {
        return;
    }

    SDL_LockAudioStream(Sfx.Stream);
    Movie.Converter = nullptr;
    SDL_UnlockAudioStream(Sfx.Stream);

    SDL_DestroyAudioStream(converter);
}
