#pragma once

struct MusicTrack
{
    const char* Path;
    float Volume;
};

struct MovieFile
{
    const char* OriginalPath;
    const char* NewPath;
};

const char* GetSoundEffect(int soundId);
const MusicTrack* GetMusicTrack(int trackNumber);
const char* GetMovieFile(const char* smkFilename);
