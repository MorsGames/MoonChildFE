#include "Resources.h"

#include "globals.hpp"
#include "SDL3/SDL_filesystem.h"

#include <cstdio>
#include <cstring>

#ifdef _WIN32
// i love this macro
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static char BasePath[2048];
static char DataPath[2048];
static char WritablePath[2048];

static void TrimToParentDirectory(char* path)
{
    char* last = nullptr;
    for (char* p = path; *p != '\0'; ++p)
    {
        if (*p == '/' || *p == '\\')
        {
            last = p;
        }
    }
    if (last != nullptr)
    {
        *last = '\0';
    }
    else
    {
        path[0] = '\0';
    }
}

static void FailedToGetPathsViaSDL()
{
    char exeDir[4096];

    exeDir[0] = '\0';

#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len > 0 && len < MAX_PATH)
    {
        std::snprintf(exeDir, sizeof(exeDir), "%s", buffer);
        TrimToParentDirectory(exeDir);
    }
#else
#ifdef __linux__
    char buffer[4096];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len > 0)
    {
        buffer[len] = '\0';
        std::snprintf(exeDir, sizeof(exeDir), "%s", buffer);
        TrimToParentDirectory(exeDir);
    }
#endif
#endif

    if (exeDir[0] == '\0')
    {
        char cwd[4096];
#ifdef _WIN32
        if (GetCurrentDirectoryA(sizeof(cwd), cwd) > 0)
        {
            std::snprintf(exeDir, sizeof(exeDir), "%s", cwd);
        }
#else
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
        {
            std::snprintf(exeDir, sizeof(exeDir), "%s", cwd);
        }
#endif
    }

    if (exeDir[0] != '\0')
    {
        const size_t n = std::strlen(exeDir);
        if (n > 0 && exeDir[n - 1] != '/' && exeDir[n - 1] != '\\')
        {
            if (n + 1 < sizeof(exeDir))
            {
                exeDir[n] = '/';
                exeDir[n + 1] = '\0';
            }
        }
    }

    std::snprintf(BasePath, sizeof(BasePath), "%s", exeDir);
    std::snprintf(DataPath, sizeof(DataPath), "%s%s", BasePath, "data/");
    std::snprintf(WritablePath, sizeof(WritablePath), "%s", BasePath);
}

static void EnsureBasePath()
{
    if (BasePath[0] != '\0')
    {
        return;
    }

#ifdef __EMSCRIPTEN__
    std::snprintf(BasePath, sizeof(BasePath), "/");
    std::snprintf(DataPath, sizeof(DataPath), "/data/");
    std::snprintf(WritablePath, sizeof(WritablePath), "/persistent/");
#else
    char exeDir[4096];
    char writeablePath[4096];
    std::snprintf(exeDir, sizeof(exeDir), "%s", SDL_GetBasePath());
    std::snprintf(writeablePath, sizeof(writeablePath), "%s", SDL_GetPrefPath(NULL, "MoonchildFE"));

    if (exeDir[0] == '\0' || writeablePath[0] == '\0')
    {
        FailedToGetPathsViaSDL();
    }
    else
    {

    std::snprintf(BasePath, sizeof(BasePath), "%s", exeDir);
    std::snprintf(DataPath, sizeof(DataPath), "%s%s", BasePath, "data/");
    std::snprintf(WritablePath, sizeof(WritablePath), "%s", writeablePath);

    char portableFilePath[4096];
    std::snprintf(portableFilePath, sizeof(portableFilePath), "%s%s", BasePath,
                  ".portable");

    bool isPortable = SDL_GetPathInfo(portableFilePath, NULL);

    if (isPortable) {
      std::snprintf(WritablePath, sizeof(WritablePath), "%s", BasePath);
    }
  }

#endif
}

static char FullPathBuf[2048];
static char WritablePathBuf[2048];

const char* FullPath(const char* file)
{
    EnsureBasePath();
    std::snprintf(FullPathBuf, sizeof(FullPathBuf), "%s%s", DataPath, file);
    return FullPathBuf;
}

const char* FullWritablePath(const char* file)
{
    EnsureBasePath();
    std::snprintf(WritablePathBuf, sizeof(WritablePathBuf), "%s%s", WritablePath, file);
    return WritablePathBuf;
}

#ifdef __EMSCRIPTEN__
void SyncPersistentStorage()
{
    EM_ASM({
        if (typeof FS !== 'undefined') {
            FS.syncfs(false, function(err) {
                if (err) {
                    console.warn('Failed to sync persistent storage!', err);
                }
            });
        }
    });
}
#endif

void LoadProgress()
{
    FILE* fp = std::fopen(FullWritablePath(const_cast<char*>("mc_progress.dat")), "rb");
    if (fp == nullptr)
    {
        return;
    }
    std::fread(&maxlevel, 1, 2, fp);
    for (int i = 0; i < 13; ++i)
    {
        std::fread(&blacksperlevel[i], 1, 2, fp);
    }
    std::fclose(fp);
}

void SaveProgress()
{
    FILE* fp = std::fopen(FullWritablePath(const_cast<char*>("mc_progress.dat")), "wb");
    if (fp == nullptr)
    {
        return;
    }
    std::fwrite(&maxlevel, 1, 2, fp);
    for (int i = 0; i < 13; ++i)
    {
        std::fwrite(&blacksperlevel[i], 1, 2, fp);
    }
    std::fclose(fp);
#ifdef __EMSCRIPTEN__
    SyncPersistentStorage();
#endif
}
