#include "MediaCatalog.h"

#include <cstring>

static const char* const SOUND_EFFECTS[] = {
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
        "mcskid.wav", "spring.wav", "mcwall.wav", "mcdrain.wav", "mcfall.wav",
        "blub.wav", "door.wav", "switch.wav", "bonus.wav", "warp.wav",
        "shoot.wav", "rumble.wav", "waterval.wav", "mcloop1.wav", "mcloop2.wav",
        "sapstap1.wav", "sapstap2.wav", "helmlp1.wav", "helmlp2.wav",
        "houtpunt.wav", "ball.wav", "segmshot.wav", "segmexpl.wav",
        "segmhit.wav", "vlamwerp.wav", "tanden.wav", "tandenm.wav",
        "camstart.wav", "camstop.wav", "cammove.wav", "madeit.wav",
        "heks.wav", "graspod.wav", "bat.wav", "vogel.wav", "vuur.wav",
        "drilboor.wav", "loopband.wav", "ventltor.wav", "bee.wav",
        "bee2.wav", "ptoei.wav", "schuif.wav", "smexp.wav", "backpak.wav",
        "restart.wav", "bigexp.wav", "stroom.wav", "cannon.wav",
        "gewicht.wav", "wheel.wav", "appel.wav", "mcdood.wav", "mcfart.wav",
        "chemo.wav", "tik.wav", "tak.wav", "raket.wav", "chemo2.wav",
        "helmdood.wav", "ketting.wav", "dimndsht.wav", "glasblok.wav",
        "hyprlift.wav", "lightwav.wav", "morphsht.wav", "mushup.wav",
        "mushdwn.wav", "plntlft.wav", "pltfdwn.wav", "pltfup.wav",
        "qbert1.wav", "qbert2.wav", "roltnlp.wav", "slowlift.wav",
        "tangjmp.wav", "tangclos.wav", "woeiwoei.wav", "heatskr.wav",
        "demo.wav", "explo.wav", "mcdrainold.wav"
};
static constexpr int SOUND_EFFECT_COUNT =
        static_cast<int>(sizeof(SOUND_EFFECTS) / sizeof(SOUND_EFFECTS[0]));

static const MusicTrack MUSIC_TRACKS[] = {
        { "", 0.0f },
        { "", 0.0f },
        { "title.mp3",     0.8f },
        { "world1.mp3",    0.5f },
        { "world2.mp3",    0.5f },
        { "world3.mp3",    0.5f },
        { "world4.mp3",    0.5f },
        { "title2.mp3",    0.3f },
    };
static constexpr int MUSIC_TRACK_COUNT =
        static_cast<int>(sizeof(MUSIC_TRACKS) / sizeof(MUSIC_TRACKS[0]));

static const MovieFile MOVIE_FILES[] = {
        { "bumper12.smk", "bumper12.mpg" },
        { "bumper23.smk", "bumper23.mpg" },
        { "bumper34.smk", "bumper34.mpg" },
        { "extro.smk",    "extro.mpg" },
        { "gameover.smk", "gameover.mpg" },
        { "intro.smk",    "intro.mpg" },
};
static constexpr int MOVIE_FILE_COUNT =
        static_cast<int>(sizeof(MOVIE_FILES) / sizeof(MOVIE_FILES[0]));

const char* GetSoundEffect(int soundId)
{
    if (soundId < 0 || soundId >= SOUND_EFFECT_COUNT)
    {
        return nullptr;
    }
    const char* name = SOUND_EFFECTS[soundId];
    return name[0] != '\0' ? name : nullptr;
}

const MusicTrack* GetMusicTrack(int trackNumber)
{
    if (trackNumber < 0 || trackNumber >= MUSIC_TRACK_COUNT)
    {
        return nullptr;
    }
    const MusicTrack* track = &MUSIC_TRACKS[trackNumber];
    return track->Path[0] != '\0' ? track : nullptr;
}

const char* GetMovieFile(const char* smkFilename)
{
    if (smkFilename == nullptr)
    {
        return nullptr;
    }
    for (int i = 0; i < MOVIE_FILE_COUNT; i++)
    {
        if (std::strcmp(smkFilename, MOVIE_FILES[i].OriginalPath) == 0)
        {
            return MOVIE_FILES[i].NewPath;
        }
    }
    return nullptr;
}
