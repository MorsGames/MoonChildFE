# Moon Child FE

Moon Child FE (Friend Edition) is a modern source port of the 1997 Windows 95 classic, Moon Child. It's designed to be the definitive way to experience the game on modern hardware.

Differences from the original Windows 95 release include (but not limited to):

- Proper support for the latest Windows versions, as well as Linux
- A full web version that you can play in your browser
- Automatic saving and loading of progress
- Controller support
- Independent input mapping for "Up" and "Jump"
- Fullscreen and windowed display modes
- 50FPS and 60FPS toggle, with Vsync support
- Nice and shimmer-free image scaling
- New Speedrun mode, which adds a speedrun timer to the top left corner of the screen
- New Easier Shoot option, which makes the jetpack shooting controls more intuitive
- New Safe Visuals mode, which makes the final level slightly easier on the eyes
- Slightly higher quality audio
- Alternate title screen music :D
- Updated credits
- And more!

This port is based on the game's later iOS release, with various features (like the menu layout and FMVs) restored from the original Windows 95 version.

Remember, you've got the power to be his friend!


## Build Guide

### Windows

1. Install the tools you need:
    - Visual Studio, with the "Desktop development with C++" workload selected
    - LLVM/Clang
    - CMake
    - Ninja
    - Git
    - Python 3 (For the web build)
    - Emscripten SDK (For the web build)
2. Open the Command Prompt, PowerShell, or Windows Terminal.
3. Run the `cd` command, followed by the path of the folder where you want to keep the source code, in quotation marks. For example: `cd "C:\GitHub"`.
4. Clone the repository with submodules and enter it:

```bat
git clone --recursive https://github.com/MorsGames/MoonChildFE.git
cd MoonChildFE
```

5. You have three options here:
    1. Open the project in Visual Studio Code (or VSCodium) and use the built-in launch configs.
    2. Run a build script manually:
        - For a Windows x64 build, run `Scripts\BuildGameWindows.bat`
        - For a Windows x86 build, run `Scripts\BuildGameWindows.bat WindowsX86`
        - For a Linux build, Install Windows Subsystem for Linux (WSL) and follow the Linux build instructions below.
        - For a web build, run `Scripts\BuildGameWeb.bat`
        - You can also add `Release` to the end of the command to build the game in release mode.
    3. Use the CMake commands directly. If you're taking this route, you probably don't need a guide in the first place.

The executable will be end up in the `Bin` folder.


### Linux

1. Open a terminal and install the required packages. On Debian or Ubuntu based distros, you can do so by running:

```bash
sudo apt update
sudo apt install -y --no-install-recommends git cmake build-essential ninja-build clang pkg-config libasound2-dev libpulse-dev libaudio-dev libjack-dev libsndio-dev libfribidi-dev libthai-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxxf86vm-dev libxtst-dev libxkbcommon-dev libwayland-dev wayland-protocols libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev
```

2. Run the `cd` command, followed by the path of the folder where you want to keep the source code. For example: `cd ~/GitHub`.
3. Clone the repository with submodules and enter it:

```bash
git clone --recursive https://github.com/MorsGames/MoonChildFE.git
cd MoonChildFE
```

4. You have two options here:
    1. Run a build script manually:
        - For a 64-bit build, run `bash ./Scripts/BuildGameLinux.sh LinuxX64`
        - For an ARM64 build, run `bash ./Scripts/BuildGameLinux.sh LinuxArm64` (UNTESTED)
        - You can also add `Release` to the end of the command to build the game in release mode.
    2. Use the CMake commands directly. If you're taking this route, you probably don't need a guide in the first place.

The executable will be end up in the `Bin` folder.
 

## Credits

**Porting**: [Mors](https://mors.games)

**Original Game Code**: [Reinier van Vliet](https://www.proofofconcept.nl)

**Original Game Graphics**: [Metin Seven](https://www.metinseven.nl)

**Original Game Music & Sounds**: [Ramon Braumuller](https://open.spotify.com/artist/6ljLO5A329ym1FARh4xAz4?si=I2-mmFi4Qq-CLNZvoku7Pw)

**Special Thanks**: Eidolon, AlbertHamik, pyrox0, koluckirafal, SteelT1
