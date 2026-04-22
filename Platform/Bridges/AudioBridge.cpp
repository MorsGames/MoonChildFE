#include "AudioBridge.h"

#include "IAudio.h"
#include "audio.hpp"

static IAudio* AudioBackend = nullptr;

namespace AudioBridge
{
    void Attach(IAudio* audio)
    {
        AudioBackend = audio;
    }

    void Detach()
    {
        AudioBackend = nullptr;
    }
}

Caudio::Caudio(void)
{
}

Caudio::~Caudio(void)
{
    AudioBackend->StopMusic();
    AudioBackend->Reset();
}

void Caudio::reset_audio()
{
    AudioBackend->StopMusic();
    AudioBackend->Reset();
}

void Caudio::checkVolume()
{
}

uint16_t Caudio::play_cd(uint16_t tracknr)
{
    return AudioBackend->PlayMusic(tracknr) ? 0 : 1;
}

void Caudio::play_cd_cb(uint16_t /*tracknr*/)
{
}

void Caudio::stop_cd(void)
{
    AudioBackend->StopMusic();
}

HSNDOBJ Caudio::create_sound(int SoundID, int nrof_simult)
{
    return AudioBackend->CreateSound(SoundID, nrof_simult);
}

void Caudio::destroy_sound(HSNDOBJ sound)
{
    AudioBackend->DestroySound(sound);
}

void Caudio::play_sound_1shot(HSNDOBJ sound, int32_t volume, int32_t pan)
{
    AudioBackend->PlayOneShot(sound, volume, pan);
}

void Caudio::play_sound_loop(HSNDOBJ sound, int32_t volume, int32_t pan)
{
    AudioBackend->PlayLoop(sound, volume, pan);
}

void Caudio::stop_sound(HSNDOBJ sound)
{
    AudioBackend->StopSound(sound);
}

void Caudio::stop_cursound(HSNDOBJ sound)
{
    AudioBackend->StopCurrent(sound);
}

void Caudio::sound_volume(HSNDOBJ sound, int32_t volume)
{
    AudioBackend->SetVolume(sound, volume);
}

void Caudio::sound_pan(HSNDOBJ sound, int32_t pan)
{
    AudioBackend->SetPan(sound, pan);
}

int Caudio::get_dsound(void)
{
    return 1;
}
