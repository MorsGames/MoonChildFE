@echo off
REM Usage: BuildGameWin.bat [Debug|Release] [WinX64|WinX86]
setlocal EnableDelayedExpansion

set "CONFIG=Debug"
set "ARCH_EXPORT=WinX64"
set "PRESET_BASE=winx64"
set "PRESET=!PRESET_BASE!-debug"
set "BUILD_PRESET=build-!PRESET_BASE!-debug"
set "VS_ARCH=x64"

for %%A in (%*) do (
    if /I "%%A"=="Release" (
        set "CONFIG=Release"
        set "PRESET=!PRESET_BASE!-release"
        set "BUILD_PRESET=build-!PRESET_BASE!-release"
    ) else if /I "%%A"=="Debug" (
        set "CONFIG=Debug"
        set "PRESET=!PRESET_BASE!-debug"
        set "BUILD_PRESET=build-!PRESET_BASE!-debug"
    ) else if /I "%%A"=="WinX86" (
        set "ARCH_EXPORT=WinX86"
        set "PRESET_BASE=winx86"
        set "VS_ARCH=x86"
        if /I "!CONFIG!"=="Release" (
            set "PRESET=!PRESET_BASE!-release"
            set "BUILD_PRESET=build-!PRESET_BASE!-release"
        ) else (
            set "PRESET=!PRESET_BASE!-debug"
            set "BUILD_PRESET=build-!PRESET_BASE!-debug"
        )
    ) else if /I "%%A"=="WinX64" (
        set "ARCH_EXPORT=WinX64"
        set "PRESET_BASE=winx64"
        set "VS_ARCH=x64"
        if /I "!CONFIG!"=="Release" (
            set "PRESET=!PRESET_BASE!-release"
            set "BUILD_PRESET=build-!PRESET_BASE!-release"
        ) else (
            set "PRESET=!PRESET_BASE!-debug"
            set "BUILD_PRESET=build-!PRESET_BASE!-debug"
        )
    )
)

for /f %%a in ('echo prompt $E ^| cmd') do (
    set "ESC=%%a"
)
set "R_ERR=%ESC%[41m"
set "R_OK=%ESC%[32m"
set "R_LOG=%ESC%[35m"
set "R_0=%ESC%[0m"

set "VSWHERE_EXE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE_EXE%" (
    echo %R_ERR%vswhere.exe not found! Do you have Visual Studio installed?%R_0%
    exit /b 1
)

set "VSDEVCMD="
for /f "usebackq delims=" %%I in (`"%VSWHERE_EXE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find Common7\Tools\VsDevCmd.bat`) do (
    set "VSDEVCMD=%%~fI"
)

if not defined VSDEVCMD (
    echo %R_ERR%VsDevCmd.bat not found! Do you have the Visual Studio C++ build tools installed?%R_0%
    exit /b 1
)

call "%VSDEVCMD%" -arch=%VS_ARCH% -host_arch=x64
if %ERRORLEVEL% neq 0 (
    exit /b %ERRORLEVEL%
)

cd /d "%~dp0.."

where clang-cl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo %R_ERR%clang-cl was not found! Do you have LLVM installed?%R_0%
    exit /b 1
)

echo %R_LOG%Configuring MoonChildFE ^(!ARCH_EXPORT! / !CONFIG!^)...%R_0%
cmake --preset "!PRESET!"
if %ERRORLEVEL% neq 0 (
    exit /b %ERRORLEVEL%
)

echo %R_LOG%Building MoonChildFE...%R_0%
cmake --build --preset "!BUILD_PRESET!" --target "MoonChildFE" --parallel "%NUMBER_OF_PROCESSORS%"
if %ERRORLEVEL% neq 0 (
    exit /b %ERRORLEVEL%
)

echo %R_OK%Build complete: MoonChildFE ^(!ARCH_EXPORT! / !CONFIG!^).%R_0%
