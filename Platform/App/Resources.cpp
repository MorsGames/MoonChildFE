#include "Resources.h"

#include "globals.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
// i love this macro
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <climits>
#include <sys/stat.h>
#include <unistd.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static char BasePath[MAX_PATH];
static char DataPath[MAX_PATH];
static char WritablePath[MAX_PATH];

static bool IsPathSeparator(char ch)
{
    return ch == '/' || ch == '\\';
}

static void TrimToParentDirectory(char* path)
{
    char* last = nullptr;
    for (char* p = path; *p != '\0'; ++p)
    {
        if (IsPathSeparator(*p))
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

static bool PathExists(const char* path, bool lookingForDir)
{
#ifdef _WIN32
    const DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    const bool isDir = (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
    return lookingForDir ? isDir : !isDir;
#else
    struct stat info;
    if (stat(path, &info) != 0)
    {
        return false;
    }
    return lookingForDir ? S_ISDIR(info.st_mode) : S_ISREG(info.st_mode);
#endif
}

static bool CreateDirectoryIfMissing(const char* path)
{
    if (path[0] == '\0' || PathExists(path, true))
    {
        return true;
    }

#ifdef _WIN32
    return CreateDirectoryA(path, nullptr) != 0 || GetLastError() == ERROR_ALREADY_EXISTS;
#else
    return mkdir(path, 0755) == 0 || PathExists(path, true);
#endif
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
    char exeDir[MAX_PATH];
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
#ifdef __APPLE__
    char buffer[MAX_PATH];
    uint32_t size = static_cast<uint32_t>(sizeof(buffer));
    if (_NSGetExecutablePath(buffer, &size) == 0)
    {
        std::snprintf(exeDir, sizeof(exeDir), "%s", buffer);
        TrimToParentDirectory(exeDir);
    }
#endif
#ifdef __linux__
    char buffer[MAX_PATH];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len > 0)
    {
        buffer[len] = '\0';
        std::snprintf(exeDir, sizeof(exeDir), "%s", buffer);
        TrimToParentDirectory(exeDir);
    }
#endif
#endif

    const size_t n = std::strlen(exeDir);
    if (n > 0 && !IsPathSeparator(exeDir[n - 1]) && n + 1 < sizeof(exeDir))
    {
        exeDir[n] = '/';
        exeDir[n + 1] = '\0';
    }

#ifdef __APPLE__
    std::snprintf(BasePath, sizeof(BasePath), "%s%s", exeDir, "../Resources/");
#else
    std::snprintf(BasePath, sizeof(BasePath), "%s", exeDir);
#endif

    std::snprintf(DataPath, sizeof(DataPath), "%s%s", BasePath, "data/");

    char portableFilePath[MAX_PATH];
    std::snprintf(portableFilePath, sizeof(portableFilePath), "%s%s", BasePath, ".portable");

    if (PathExists(portableFilePath, false))
    {
        std::snprintf(WritablePath, sizeof(WritablePath), "%s", BasePath);
    }
    else
    {
        WritablePath[0] = '\0';

#ifdef _WIN32
        const char* appData = std::getenv("APPDATA");
        if (appData != nullptr && appData[0] != '\0')
        {
            std::snprintf(WritablePath, sizeof(WritablePath), "%s/MoonchildFE/", appData);
        }
        else
        {
            const char* userProfile = std::getenv("USERPROFILE");
            if (userProfile != nullptr && userProfile[0] != '\0')
            {
                std::snprintf(WritablePath, sizeof(WritablePath), "%s/AppData/Roaming/MoonchildFE/", userProfile);
            }
        }
#elif defined(__APPLE__)
        const char* home = std::getenv("HOME");
        if (home != nullptr && home[0] != '\0')
        {
            std::snprintf(WritablePath, sizeof(WritablePath), "%s/Library/Application Support/MoonchildFE/", home);
        }
#else
        const char* xdgDataHome = std::getenv("XDG_DATA_HOME");
        if (xdgDataHome != nullptr && xdgDataHome[0] != '\0')
        {
            std::snprintf(WritablePath, sizeof(WritablePath), "%s/MoonchildFE/", xdgDataHome);
        }
        else
        {
            const char* home = std::getenv("HOME");
            if (home != nullptr && home[0] != '\0')
            {
                std::snprintf(WritablePath, sizeof(WritablePath), "%s/.local/share/MoonchildFE/", home);
            }
        }
#endif

        if (WritablePath[0] != '\0')
        {
            char scratch[MAX_PATH];
            std::snprintf(scratch, sizeof(scratch), "%s", WritablePath);

            const size_t len = std::strlen(scratch);
            size_t start = IsPathSeparator(scratch[0]) ? 1 : 0;
#ifdef _WIN32
            if (len >= 3 && scratch[1] == ':' && IsPathSeparator(scratch[2]))
            {
                start = 3;
            }
#endif

            for (size_t i = start; i < len; ++i)
            {
                if (!IsPathSeparator(scratch[i]))
                {
                    continue;
                }

                scratch[i] = '\0';
                CreateDirectoryIfMissing(scratch);
                scratch[i] = '/';
            }

            CreateDirectoryIfMissing(scratch);
        }
    }

#endif
}

static char FullPathBuf[MAX_PATH];
static char WritablePathBuf[MAX_PATH];

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
    FILE* fp = std::fopen(FullWritablePath("mc_progress.dat"), "rb");
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
    FILE* fp = std::fopen(FullWritablePath("mc_progress.dat"), "wb");
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
