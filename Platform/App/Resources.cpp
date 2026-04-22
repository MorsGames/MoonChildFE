#include "Resources.h"

#include "globals.hpp"

#include <cstdio>
#include <cstring>

#ifdef _WIN32
// i love this macro
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#if defined(__linux__) || defined(__EMSCRIPTEN__)
#include <unistd.h>
#endif
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

static void EnsureBasePath()
{
    if (BasePath[0] != '\0')
    {
        return;
    }

    char exeDir[4096];
    exeDir[0] = '\0';

#ifdef _WIN32
    char  buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len > 0 && len < MAX_PATH)
    {
        std::snprintf(exeDir, sizeof(exeDir), "%s", buffer);
        TrimToParentDirectory(exeDir);
    }
#else
#ifdef __linux__
    char    buffer[4096];
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
#ifdef __EMSCRIPTEN__
    std::snprintf(BasePath, sizeof(BasePath), "/");
    std::snprintf(DataPath, sizeof(DataPath), "/data/");
    std::snprintf(WritablePath, sizeof(WritablePath), "/persistent/");
#else
    std::snprintf(BasePath, sizeof(BasePath), "%s", exeDir);
    std::snprintf(DataPath, sizeof(DataPath), "%s%s", BasePath, "data/");
    std::snprintf(WritablePath, sizeof(WritablePath), "%s", BasePath);
#endif
}

static char FullPathBuf[2048];
static char WritablePathBuf[2048];

char* FullPath(char* file)
{
    EnsureBasePath();
    std::snprintf(FullPathBuf, sizeof(FullPathBuf), "%s%s", DataPath, file);
    return FullPathBuf;
}

char* FullWritablePath(char* file)
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
    FILE* fp = std::fopen(FullWritablePath(const_cast<char*>("progress.dat")), "rb");
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
    FILE* fp = std::fopen(FullWritablePath(const_cast<char*>("progress.dat")), "wb");
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
