cmake_minimum_required(VERSION 3.22.1)

project(MoonChildFE LANGUAGES C CXX VERSION 1.1.1)
set(MOONCHILD_OUTPUT_NAME "Moon Child FE")
set(EXECUTABLE_NAME MoonChildFE)
if(ANDROID)
    set(EXECUTABLE_NAME main)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths
set(MOONCHILD_LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/Libraries)
set(MOONCHILD_CLASSES_DIR ${CMAKE_CURRENT_LIST_DIR}/Classes)
set(MOONCHILD_FRAMEWORK_DIR ${MOONCHILD_CLASSES_DIR}/framewrk)
set(MOONCHILD_GAME_DIR ${MOONCHILD_CLASSES_DIR}/moonchild)
set(MOONCHILD_PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR}/Platform)

# Backend defaults
set(MOONCHILD_WINDOW_BACKEND_DEFAULT "SDL3")
set(MOONCHILD_RENDERER_BACKEND_DEFAULT "OpenGL")
if(EMSCRIPTEN OR ANDROID OR MOONCHILD_TARGET_ARCH STREQUAL "LinuxArm64")
    set(MOONCHILD_RENDERER_BACKEND_DEFAULT "GLES")
endif()
set(MOONCHILD_INPUT_BACKEND_DEFAULT "SDL3")
set(MOONCHILD_AUDIO_BACKEND_DEFAULT "SDL3")

# Backend selection
set(MOONCHILD_WINDOW_BACKEND "${MOONCHILD_WINDOW_BACKEND_DEFAULT}" CACHE STRING "Windowing backend")
set_property(CACHE MOONCHILD_WINDOW_BACKEND PROPERTY STRINGS SDL2 SDL3)
set(MOONCHILD_RENDERER_BACKEND "${MOONCHILD_RENDERER_BACKEND_DEFAULT}" CACHE STRING "Renderer backend")
set_property(CACHE MOONCHILD_RENDERER_BACKEND PROPERTY STRINGS OpenGL GLES)
set(MOONCHILD_INPUT_BACKEND "${MOONCHILD_INPUT_BACKEND_DEFAULT}" CACHE STRING "Input backend")
set_property(CACHE MOONCHILD_INPUT_BACKEND PROPERTY STRINGS SDL2 SDL3)
set(MOONCHILD_AUDIO_BACKEND "${MOONCHILD_AUDIO_BACKEND_DEFAULT}" CACHE STRING "Audio backend")
set_property(CACHE MOONCHILD_AUDIO_BACKEND PROPERTY STRINGS SDL2 SDL3)

# Build options
option(MOONCHILD_VENDORED_SDL2 "Use vendored SDL2 library" ON)
option(MOONCHILD_VENDORED_SDL3 "Use vendored SDL3 library" ON)
option(MOONCHILD_VENDORED_ZLIB "Use vendored zlib library" ON)
option(MOONCHILD_VENDORED_GAMECONTROLLERDB "Use vendored SDL_GameControllerDB" ON)
option(MOONCHILD_VENDORED_PL_MPEG "Use vendored pl_mpeg library" ON)

# Sanity checks
set(MOONCHILD_WINDOW_BACKEND_VALUES SDL2 SDL3)
set(MOONCHILD_RENDERER_BACKEND_VALUES OpenGL GLES)
set(MOONCHILD_INPUT_BACKEND_VALUES SDL2 SDL3)
set(MOONCHILD_AUDIO_BACKEND_VALUES SDL2 SDL3)

if(NOT MOONCHILD_WINDOW_BACKEND IN_LIST MOONCHILD_WINDOW_BACKEND_VALUES)
    message(FATAL_ERROR "Unsupported windowing backend: ${MOONCHILD_WINDOW_BACKEND}")
endif()
if(NOT MOONCHILD_RENDERER_BACKEND IN_LIST MOONCHILD_RENDERER_BACKEND_VALUES)
    message(FATAL_ERROR "Unsupported renderer backend: ${MOONCHILD_RENDERER_BACKEND}")
endif()
if(NOT MOONCHILD_INPUT_BACKEND IN_LIST MOONCHILD_INPUT_BACKEND_VALUES)
    message(FATAL_ERROR "Unsupported input backend: ${MOONCHILD_INPUT_BACKEND}")
endif()
if(NOT MOONCHILD_AUDIO_BACKEND IN_LIST MOONCHILD_AUDIO_BACKEND_VALUES)
    message(FATAL_ERROR "Unsupported audio backend: ${MOONCHILD_AUDIO_BACKEND}")
endif()

set(MOONCHILD_WINDOW_SDL OFF)
set(MOONCHILD_WINDOW_SDL2 OFF)
set(MOONCHILD_WINDOW_SDL3 OFF)
set(MOONCHILD_WINDOW_GL_SURFACE OFF)
if(MOONCHILD_WINDOW_BACKEND STREQUAL "SDL2")
    set(MOONCHILD_WINDOW_SDL ON)
    set(MOONCHILD_WINDOW_SDL2 ON)
    set(MOONCHILD_WINDOW_GL_SURFACE ON)
elseif(MOONCHILD_WINDOW_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_WINDOW_SDL ON)
    set(MOONCHILD_WINDOW_SDL3 ON)
    set(MOONCHILD_WINDOW_GL_SURFACE ON)
endif()

set(MOONCHILD_RENDERER_OPENGL OFF)
set(MOONCHILD_RENDERER_GLES OFF)
if(MOONCHILD_RENDERER_BACKEND STREQUAL "OpenGL")
    set(MOONCHILD_RENDERER_OPENGL ON)
elseif(MOONCHILD_RENDERER_BACKEND STREQUAL "GLES")
    set(MOONCHILD_RENDERER_GLES ON)
endif()

set(MOONCHILD_INPUT_SDL OFF)
set(MOONCHILD_INPUT_SDL2 OFF)
set(MOONCHILD_INPUT_SDL3 OFF)
if(MOONCHILD_INPUT_BACKEND STREQUAL "SDL2")
    set(MOONCHILD_INPUT_SDL ON)
    set(MOONCHILD_INPUT_SDL2 ON)
elseif(MOONCHILD_INPUT_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_INPUT_SDL ON)
    set(MOONCHILD_INPUT_SDL3 ON)
endif()

set(MOONCHILD_AUDIO_SDL OFF)
set(MOONCHILD_AUDIO_SDL2 OFF)
set(MOONCHILD_AUDIO_SDL3 OFF)
if(MOONCHILD_AUDIO_BACKEND STREQUAL "SDL2")
    set(MOONCHILD_AUDIO_SDL ON)
    set(MOONCHILD_AUDIO_SDL2 ON)
elseif(MOONCHILD_AUDIO_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_AUDIO_SDL ON)
    set(MOONCHILD_AUDIO_SDL3 ON)
endif()

# Aggregate flags
if(MOONCHILD_WINDOW_SDL OR MOONCHILD_INPUT_SDL OR MOONCHILD_AUDIO_SDL)
    set(MOONCHILD_USE_SDL ON)
else()
    set(MOONCHILD_USE_SDL OFF)
endif()

if(MOONCHILD_WINDOW_SDL2 OR MOONCHILD_INPUT_SDL2 OR MOONCHILD_AUDIO_SDL2)
    set(MOONCHILD_USE_SDL2 ON)
else()
    set(MOONCHILD_USE_SDL2 OFF)
endif()

if(MOONCHILD_WINDOW_SDL3 OR MOONCHILD_INPUT_SDL3 OR MOONCHILD_AUDIO_SDL3)
    set(MOONCHILD_USE_SDL3 ON)
else()
    set(MOONCHILD_USE_SDL3 OFF)
endif()

if(MOONCHILD_RENDERER_OPENGL OR MOONCHILD_RENDERER_GLES)
    set(MOONCHILD_USE_OPENGL ON)
else()
    set(MOONCHILD_USE_OPENGL OFF)
endif()

if(MOONCHILD_USE_SDL2 AND MOONCHILD_USE_SDL3)
    message(FATAL_ERROR
        "SDL2 and SDL3 backends cannot be mixed in the same build!")
endif()
if(MOONCHILD_USE_OPENGL AND NOT MOONCHILD_WINDOW_GL_SURFACE)
    message(FATAL_ERROR
        "OpenGL renderers require a windowing backend that implements IGLSurface!")
endif()
if(EMSCRIPTEN AND MOONCHILD_RENDERER_OPENGL)
    message(FATAL_ERROR
        "The OpenGL 2.1 renderer is not supported on the web! Use the OpenGL ES 2.0 renderer instead.")
endif()
if(ANDROID AND MOONCHILD_RENDERER_OPENGL)
    message(FATAL_ERROR
        "The OpenGL 2.1 renderer is not supported on Android! Use the OpenGL ES 2.0 renderer instead.")
endif()
if(MOONCHILD_TARGET_ARCH STREQUAL "LinuxArm64" AND MOONCHILD_RENDERER_OPENGL)
    message(FATAL_ERROR
        "The OpenGL 2.1 renderer is not supported on Linux ARM64! Use the OpenGL ES 2.0 renderer instead.")
endif()

# Architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MOONCHILD_ARCH_64 ON)
else()
    set(MOONCHILD_ARCH_32 ON)
endif()

# SDL setup
if(MOONCHILD_USE_SDL2)
    if(MOONCHILD_VENDORED_SDL2)
        if(ANDROID)
            set(SDL_SHARED   ON  CACHE BOOL "" FORCE)
            set(SDL_STATIC   OFF CACHE BOOL "" FORCE)
        else()
            set(SDL_SHARED   OFF CACHE BOOL "" FORCE)
            set(SDL_STATIC   ON  CACHE BOOL "" FORCE)
        endif()
        set(SDL_TEST         OFF CACHE BOOL "" FORCE)
        set(SDL_TESTS        OFF CACHE BOOL "" FORCE)
        set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
        set(SDL2_DISABLE_UNINSTALL ON CACHE BOOL "" FORCE)
        set(SDL_INSTALL_TESTS OFF CACHE BOOL "" FORCE)
        add_subdirectory("${MOONCHILD_LIBRARY_DIR}/SDL2" "${CMAKE_BINARY_DIR}/SDL2" EXCLUDE_FROM_ALL)

        if(TARGET SDL2::SDL2-static)
            set(MOONCHILD_SDL_TARGET SDL2::SDL2-static)
        elseif(TARGET SDL2::SDL2)
            set(MOONCHILD_SDL_TARGET SDL2::SDL2)
        else()
            message(FATAL_ERROR "Could not find an SDL2 target to link!")
        endif()
    else()
        find_package(SDL2 REQUIRED CONFIG)
        set(MOONCHILD_SDL_TARGET SDL2::SDL2)
    endif()
elseif(MOONCHILD_USE_SDL3)
    if(MOONCHILD_VENDORED_SDL3)
        if(ANDROID)
            set(SDL_SHARED   ON  CACHE BOOL "" FORCE)
            set(SDL_STATIC   OFF CACHE BOOL "" FORCE)
        else()
            set(SDL_SHARED   OFF CACHE BOOL "" FORCE)
            set(SDL_STATIC   ON  CACHE BOOL "" FORCE)
        endif()
        set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
        set(SDL_TESTS        OFF CACHE BOOL "" FORCE)
        set(SDL_EXAMPLES     OFF CACHE BOOL "" FORCE)
        set(SDL_INSTALL      OFF CACHE BOOL "" FORCE)
        add_subdirectory("${MOONCHILD_LIBRARY_DIR}/SDL3" "${CMAKE_BINARY_DIR}/SDL3" EXCLUDE_FROM_ALL)

        if(TARGET SDL3::SDL3-static)
            set(MOONCHILD_SDL_TARGET SDL3::SDL3-static)
        elseif(TARGET SDL3::SDL3)
            set(MOONCHILD_SDL_TARGET SDL3::SDL3)
        elseif(TARGET SDL3::SDL3-shared)
            set(MOONCHILD_SDL_TARGET SDL3::SDL3-shared)
        else()
            message(FATAL_ERROR "Could not find an SDL3 target to link!")
        endif()
    else()
        find_package(SDL3 REQUIRED)
        set(MOONCHILD_SDL_TARGET SDL3::SDL3)
    endif()
endif()

# SDL_gamecontrollerdb setup
if(MOONCHILD_VENDORED_GAMECONTROLLERDB)
    set(MOONCHILD_GAMECONTROLLERDB_PATH "data/gamecontrollerdb.txt")
else()
    set(MOONCHILD_GAMECONTROLLERDB_PATH "$ENV{MOONCHILD_GAMECONTROLLERDB_PATH}")
    file(TO_CMAKE_PATH "${MOONCHILD_GAMECONTROLLERDB_PATH}" MOONCHILD_GAMECONTROLLERDB_PATH)
endif()

# zlib setup
if(MOONCHILD_VENDORED_ZLIB)
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_SHARED   OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_STATIC   ON  CACHE BOOL "" FORCE)
    add_subdirectory("${MOONCHILD_LIBRARY_DIR}/zlib" "${CMAKE_BINARY_DIR}/zlib" EXCLUDE_FROM_ALL)
    set(MOONCHILD_ZLIB_TARGET zlibstatic)
else()
    find_package(ZLIB REQUIRED)
    set(MOONCHILD_ZLIB_TARGET ZLIB::ZLIB)
endif()

# OpenGL setup
if(MOONCHILD_RENDERER_OPENGL AND NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
endif()

# Game sources
set(MOONCHILD_GAME_SOURCES
    ${MOONCHILD_GAME_DIR}/anim.cpp
    ${MOONCHILD_GAME_DIR}/asset.cpp
    ${MOONCHILD_GAME_DIR}/asteroid.cpp
    ${MOONCHILD_GAME_DIR}/basic.cpp
    ${MOONCHILD_GAME_DIR}/bat.cpp
    ${MOONCHILD_GAME_DIR}/bbot.cpp
    ${MOONCHILD_GAME_DIR}/bee.cpp
    ${MOONCHILD_GAME_DIR}/bolt.cpp
    ${MOONCHILD_GAME_DIR}/bonus.cpp
    ${MOONCHILD_GAME_DIR}/boss.cpp
    ${MOONCHILD_GAME_DIR}/bouncey.cpp
    ${MOONCHILD_GAME_DIR}/bullet.cpp
    ${MOONCHILD_GAME_DIR}/bump.cpp
    ${MOONCHILD_GAME_DIR}/cannon.cpp
    ${MOONCHILD_GAME_DIR}/chain.cpp
    ${MOONCHILD_GAME_DIR}/claw.cpp
    ${MOONCHILD_GAME_DIR}/cloud.cpp
    ${MOONCHILD_GAME_DIR}/diamond.cpp
    ${MOONCHILD_GAME_DIR}/doldoler.cpp
    ${MOONCHILD_GAME_DIR}/editor.cpp
    ${MOONCHILD_GAME_DIR}/element.cpp
    ${MOONCHILD_GAME_DIR}/elevat.cpp
    ${MOONCHILD_GAME_DIR}/glim.cpp
    ${MOONCHILD_GAME_DIR}/globals.cpp
    ${MOONCHILD_GAME_DIR}/goodday.cpp
    ${MOONCHILD_GAME_DIR}/gravlift.cpp
    ${MOONCHILD_GAME_DIR}/gumbal.cpp
    ${MOONCHILD_GAME_DIR}/hoi.cpp
    ${MOONCHILD_GAME_DIR}/ironauto.cpp
    ${MOONCHILD_GAME_DIR}/ironring.cpp
    ${MOONCHILD_GAME_DIR}/levinits.cpp
    ${MOONCHILD_GAME_DIR}/lift.cpp
    ${MOONCHILD_GAME_DIR}/mc.cpp
    ${MOONCHILD_GAME_DIR}/metbal.cpp
    ${MOONCHILD_GAME_DIR}/metcan.cpp
    ${MOONCHILD_GAME_DIR}/mine.cpp
    ${MOONCHILD_GAME_DIR}/mixer.cpp
    ${MOONCHILD_GAME_DIR}/mouth.cpp
    ${MOONCHILD_GAME_DIR}/mushroom.cpp
    ${MOONCHILD_GAME_DIR}/objects.cpp
    ${MOONCHILD_GAME_DIR}/paal.cpp
    ${MOONCHILD_GAME_DIR}/plant.cpp
    ${MOONCHILD_GAME_DIR}/plof.cpp
    ${MOONCHILD_GAME_DIR}/prefs.cpp
    ${MOONCHILD_GAME_DIR}/ptoei.cpp
    ${MOONCHILD_GAME_DIR}/punt.cpp
    ${MOONCHILD_GAME_DIR}/robyn.cpp
    ${MOONCHILD_GAME_DIR}/rock.cpp
    ${MOONCHILD_GAME_DIR}/rocket.cpp
    ${MOONCHILD_GAME_DIR}/score.cpp
    ${MOONCHILD_GAME_DIR}/scroll.cpp
    ${MOONCHILD_GAME_DIR}/seamine.cpp
    ${MOONCHILD_GAME_DIR}/smlheart.cpp
    ${MOONCHILD_GAME_DIR}/snake.cpp
    ${MOONCHILD_GAME_DIR}/sneak.cpp
    ${MOONCHILD_GAME_DIR}/sokoban.cpp
    ${MOONCHILD_GAME_DIR}/sound.cpp
    ${MOONCHILD_GAME_DIR}/spies.cpp
    ${MOONCHILD_GAME_DIR}/spike.cpp
    ${MOONCHILD_GAME_DIR}/stukhout.cpp
    ${MOONCHILD_GAME_DIR}/switsj.cpp
    ${MOONCHILD_GAME_DIR}/trigger.cpp
    ${MOONCHILD_GAME_DIR}/vgdll.cpp
    ${MOONCHILD_GAME_DIR}/warp.cpp
    ${MOONCHILD_GAME_DIR}/weight.cpp
    ${MOONCHILD_GAME_DIR}/wheel.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/blitbuf.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/fastfile.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/pcxff.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/sprite.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/timer.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/unzip.c
    ${MOONCHILD_FRAMEWORK_DIR}/video.cpp
)

# Platform sources
set(MOONCHILD_PLATFORM_SOURCES
    ${MOONCHILD_PLATFORM_DIR}/App/Main.cpp
    ${MOONCHILD_PLATFORM_DIR}/App/Host.cpp
    ${MOONCHILD_PLATFORM_DIR}/App/PlatformBackends.cpp
    ${MOONCHILD_PLATFORM_DIR}/App/Resources.cpp
    ${MOONCHILD_PLATFORM_DIR}/App/FrameworkGlue.cpp
    ${MOONCHILD_PLATFORM_DIR}/App/MediaCatalog.cpp
    ${MOONCHILD_PLATFORM_DIR}/Core/Random.cpp
    ${MOONCHILD_PLATFORM_DIR}/Bridges/AudioBridge.cpp
    ${MOONCHILD_PLATFORM_DIR}/Bridges/DisplayBridge.cpp
    ${MOONCHILD_PLATFORM_DIR}/Bridges/InputBridge.cpp
    ${MOONCHILD_PLATFORM_DIR}/Bridges/MovieBridge.cpp
    ${MOONCHILD_PLATFORM_DIR}/Audio/AudioEngine.cpp
    ${MOONCHILD_PLATFORM_DIR}/Audio/MP3MusicTrack.cpp
)

# Backends
set(MOONCHILD_BACKEND_SOURCES "")
set(MOONCHILD_BACKEND_INCLUDES "")

if(MOONCHILD_WINDOW_SDL2)
    list(APPEND MOONCHILD_BACKEND_SOURCES  ${MOONCHILD_PLATFORM_DIR}/Backends/Window/SDL2Window.cpp)
    list(APPEND MOONCHILD_BACKEND_INCLUDES ${MOONCHILD_PLATFORM_DIR}/Backends/Window)
endif()

if(MOONCHILD_WINDOW_SDL3)
    list(APPEND MOONCHILD_BACKEND_SOURCES  ${MOONCHILD_PLATFORM_DIR}/Backends/Window/SDL3Window.cpp)
    list(APPEND MOONCHILD_BACKEND_INCLUDES ${MOONCHILD_PLATFORM_DIR}/Backends/Window)
endif()

if(MOONCHILD_RENDERER_GLES)
    list(APPEND MOONCHILD_BACKEND_SOURCES
        ${MOONCHILD_PLATFORM_DIR}/Backends/Renderer/GLESRenderer.cpp
        ${MOONCHILD_LIBRARY_DIR}/glad/glad.c
    )
    list(APPEND MOONCHILD_BACKEND_INCLUDES
        ${MOONCHILD_PLATFORM_DIR}/Backends/Renderer
        ${MOONCHILD_LIBRARY_DIR}/glad
    )
endif()

if(MOONCHILD_RENDERER_OPENGL)
    list(APPEND MOONCHILD_BACKEND_SOURCES
        ${MOONCHILD_PLATFORM_DIR}/Backends/Renderer/OpenGLRenderer.cpp
        ${MOONCHILD_LIBRARY_DIR}/glad/glad.c
    )
    list(APPEND MOONCHILD_BACKEND_INCLUDES
        ${MOONCHILD_PLATFORM_DIR}/Backends/Renderer
        ${MOONCHILD_LIBRARY_DIR}/glad
    )
endif()

if(MOONCHILD_INPUT_SDL2)
    list(APPEND MOONCHILD_BACKEND_SOURCES  ${MOONCHILD_PLATFORM_DIR}/Backends/Input/SDL2Input.cpp)
    list(APPEND MOONCHILD_BACKEND_INCLUDES ${MOONCHILD_PLATFORM_DIR}/Backends/Input)
endif()

if(MOONCHILD_INPUT_SDL3)
    list(APPEND MOONCHILD_BACKEND_SOURCES  ${MOONCHILD_PLATFORM_DIR}/Backends/Input/SDL3Input.cpp)
    list(APPEND MOONCHILD_BACKEND_INCLUDES ${MOONCHILD_PLATFORM_DIR}/Backends/Input)
endif()

if(MOONCHILD_AUDIO_SDL2)
    list(APPEND MOONCHILD_BACKEND_SOURCES  ${MOONCHILD_PLATFORM_DIR}/Backends/Audio/SDL2Audio.cpp)
    list(APPEND MOONCHILD_BACKEND_INCLUDES
        ${MOONCHILD_PLATFORM_DIR}/Backends/Audio
        ${MOONCHILD_PLATFORM_DIR}/Audio
        ${MOONCHILD_LIBRARY_DIR}/dr_libs
    )
endif()

if(MOONCHILD_AUDIO_SDL3)
    list(APPEND MOONCHILD_BACKEND_SOURCES  ${MOONCHILD_PLATFORM_DIR}/Backends/Audio/SDL3Audio.cpp)
    list(APPEND MOONCHILD_BACKEND_INCLUDES
        ${MOONCHILD_PLATFORM_DIR}/Backends/Audio
        ${MOONCHILD_PLATFORM_DIR}/Audio
        ${MOONCHILD_LIBRARY_DIR}/dr_libs
    )
endif()

list(REMOVE_DUPLICATES MOONCHILD_BACKEND_INCLUDES)

# Final sources list
set(MOONCHILD_SOURCES
    ${MOONCHILD_GAME_SOURCES}
    ${MOONCHILD_PLATFORM_SOURCES}
    ${MOONCHILD_BACKEND_SOURCES}
)

# Target
if(ANDROID)
    add_library(${EXECUTABLE_NAME} SHARED ${MOONCHILD_SOURCES})
elseif(WIN32)
    add_executable(${EXECUTABLE_NAME} WIN32 ${MOONCHILD_SOURCES})
else()
    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${MOONCHILD_SOURCES})
endif()

if(NOT ANDROID)
    if(EMSCRIPTEN)
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}")
    else()
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${MOONCHILD_OUTPUT_NAME}")
    endif()
endif()

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${MOONCHILD_CLASSES_DIR}
    ${MOONCHILD_FRAMEWORK_DIR}
    ${MOONCHILD_GAME_DIR}
    ${MOONCHILD_PLATFORM_DIR}/Core
    ${MOONCHILD_PLATFORM_DIR}/App
    ${MOONCHILD_PLATFORM_DIR}/Bridges
    ${MOONCHILD_BACKEND_INCLUDES}
)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
    "MOONCHILD_APP_VERSION_STR=\"${CMAKE_PROJECT_VERSION}\""
)

# pl_mpeg setup
if(MOONCHILD_VENDORED_PL_MPEG)
    target_include_directories(${EXECUTABLE_NAME} PRIVATE
      ${MOONCHILD_LIBRARY_DIR}/pl_mpeg
    )
else()
    target_include_directories(${EXECUTABLE_NAME} PRIVATE
      $ENV{MOONCHILD_PL_MPEG_PATH}
    )
endif()

# Backend selection macros
if(MOONCHILD_WINDOW_SDL2)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOW_SDL2)
    if(NOT ANDROID)
        target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_DESKTOP_MODE)
    endif()
endif()

if(MOONCHILD_WINDOW_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOW_SDL3)
    if(NOT ANDROID)
        target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_DESKTOP_MODE)
    endif()
endif()

if(MOONCHILD_RENDERER_OPENGL)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_OPENGL)
endif()

if(MOONCHILD_RENDERER_GLES)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_GLES)
endif()

if(MOONCHILD_INPUT_SDL2)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_INPUT_SDL2)
endif()

if(MOONCHILD_INPUT_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_INPUT_SDL3)
endif()

if(MOONCHILD_INPUT_SDL)
    if(NOT MOONCHILD_GAMECONTROLLERDB_PATH STREQUAL "")
        target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_GAMECONTROLLERDB_PATH=\"${MOONCHILD_GAMECONTROLLERDB_PATH}\"
        )
    endif()
endif()

if(MOONCHILD_AUDIO_SDL2)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_AUDIO_SDL2)
endif()

if(MOONCHILD_AUDIO_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_AUDIO_SDL3)
endif()

if(MOONCHILD_USE_SDL)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_USE_SDL SDL_MAIN_HANDLED)
endif()

if(MOONCHILD_USE_SDL2)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_USE_SDL2)
endif()

if(MOONCHILD_USE_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_USE_SDL3)
endif()

if(MOONCHILD_USE_OPENGL)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_USE_OPENGL)
endif()

if(ANDROID)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_ANDROID)
endif()

target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${MOONCHILD_ZLIB_TARGET})

if(MOONCHILD_USE_SDL)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${MOONCHILD_SDL_TARGET})
endif()

if(MOONCHILD_RENDERER_OPENGL)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenGL::GL)
endif()

# Platform specific configuration
if(EMSCRIPTEN)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WEB)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Web.cmake)
elseif(WIN32 AND MOONCHILD_ARCH_64)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Windows.cmake)
elseif(WIN32 AND MOONCHILD_ARCH_32)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Windows.cmake)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_LINUX)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Linux.cmake)
elseif(APPLE)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_MACOS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/macOS.cmake)
endif()

if(NOT ANDROID)
    # Output directories
    set(MOONCHILD_BIN_DIR ${CMAKE_SOURCE_DIR}/Bin/${MOONCHILD_TARGET_ARCH})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${MOONCHILD_BIN_DIR}/Debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${MOONCHILD_BIN_DIR}/Release
    )

    if(MOONCHILD_INPUT_SDL AND NOT EMSCRIPTEN AND MOONCHILD_VENDORED_GAMECONTROLLERDB AND APPLE)
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/data
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MOONCHILD_LIBRARY_DIR}/SDL_GameControllerDB/gamecontrollerdb.txt"
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/data/gamecontrollerdb.txt
        )
    elseif(MOONCHILD_INPUT_SDL AND NOT EMSCRIPTEN AND MOONCHILD_VENDORED_GAMECONTROLLERDB AND NOT APPLE)
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/data
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MOONCHILD_LIBRARY_DIR}/SDL_GameControllerDB/gamecontrollerdb.txt"
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/data/gamecontrollerdb.txt
        )
    endif()

    set(MOONCHILD_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)
    if (APPLE)
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${MOONCHILD_RESOURCES_DIR}"
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/data
        )
    else()
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${MOONCHILD_RESOURCES_DIR}"
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/data
        )
    endif()
endif()

# MSVC shut up
if(MSVC)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
