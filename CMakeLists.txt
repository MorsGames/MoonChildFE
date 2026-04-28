cmake_minimum_required(VERSION 3.25.1)

project(MoonChildFE LANGUAGES C CXX)
set(EXECUTABLE_NAME MoonChildFE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths
set(LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/Libraries)
set(MOONCHILD_CLASSES_DIR ${CMAKE_CURRENT_LIST_DIR}/Classes)
set(MOONCHILD_FRAMEWORK_DIR ${MOONCHILD_CLASSES_DIR}/framewrk)
set(MOONCHILD_GAME_DIR ${MOONCHILD_CLASSES_DIR}/moonchild)
set(PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR}/Platform)

# Backend defaults
set(MOONCHILD_WINDOW_BACKEND_DEFAULT "SDL3")
set(MOONCHILD_RENDERER_BACKEND_DEFAULT "OpenGL")
if(EMSCRIPTEN)
    set(MOONCHILD_RENDERER_BACKEND_DEFAULT "GLES")
endif()
set(MOONCHILD_INPUT_BACKEND_DEFAULT "SDL3")
set(MOONCHILD_AUDIO_BACKEND_DEFAULT "SDL3")

# Backend selection
set(MOONCHILD_WINDOW_BACKEND "${MOONCHILD_WINDOW_BACKEND_DEFAULT}" CACHE STRING "Windowing backend")
set_property(CACHE MOONCHILD_WINDOW_BACKEND PROPERTY STRINGS SDL3)
set(MOONCHILD_RENDERER_BACKEND "${MOONCHILD_RENDERER_BACKEND_DEFAULT}" CACHE STRING "Renderer backend")
set_property(CACHE MOONCHILD_RENDERER_BACKEND PROPERTY STRINGS OpenGL GLES SDL3)
set(MOONCHILD_INPUT_BACKEND "${MOONCHILD_INPUT_BACKEND_DEFAULT}" CACHE STRING "Input backend")
set_property(CACHE MOONCHILD_INPUT_BACKEND PROPERTY STRINGS SDL3)
set(MOONCHILD_AUDIO_BACKEND "${MOONCHILD_AUDIO_BACKEND_DEFAULT}" CACHE STRING "Audio backend")
set_property(CACHE MOONCHILD_AUDIO_BACKEND PROPERTY STRINGS SDL3)

# Enable Editor
option(MOONCHILD_ENABLE_EDITOR "Enable the level editor" OFF)
if(MOONCHILD_ENABLE_EDITOR)
  add_compile_definitions(EDITOR=1)
endif()

# Vendoring options
option(MOONCHILD_VENDORED_SDL3 "Use vendored SDL3 library" ON)
option(MOONCHILD_VENDORED_ZLIB "Use vendored zlib library" ON)
option(MOONCHILD_VENDORED_GAMECONTROLLERDB "Use vendored SDL_GameControllerDB" ON)
option(MOONCHILD_VENDORED_PL_MPEG "Use vendored pl_mpeg library" ON)

# Sanity checks
set(MOONCHILD_WINDOW_BACKEND_VALUES SDL3)
set(MOONCHILD_RENDERER_BACKEND_VALUES OpenGL GLES SDL3)
set(MOONCHILD_INPUT_BACKEND_VALUES SDL3)
set(MOONCHILD_AUDIO_BACKEND_VALUES SDL3)

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

set(MOONCHILD_WINDOW_SDL3 OFF)
if(MOONCHILD_WINDOW_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_WINDOW_SDL3 ON)
endif()

set(MOONCHILD_RENDERER_OPENGL OFF)
set(MOONCHILD_RENDERER_GLES OFF)
set(MOONCHILD_RENDERER_SDL3 OFF)
if(MOONCHILD_RENDERER_BACKEND STREQUAL "OpenGL")
    set(MOONCHILD_RENDERER_OPENGL ON)
elseif(MOONCHILD_RENDERER_BACKEND STREQUAL "GLES")
    set(MOONCHILD_RENDERER_GLES ON)
elseif(MOONCHILD_RENDERER_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_RENDERER_SDL3 ON)
endif()

set(MOONCHILD_INPUT_SDL3 OFF)
if(MOONCHILD_INPUT_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_INPUT_SDL3 ON)
endif()

set(MOONCHILD_AUDIO_SDL3 OFF)
if(MOONCHILD_AUDIO_BACKEND STREQUAL "SDL3")
    set(MOONCHILD_AUDIO_SDL3 ON)
endif()

# Aggregate flags
if(MOONCHILD_WINDOW_SDL3 OR MOONCHILD_RENDERER_SDL3 OR MOONCHILD_INPUT_SDL3 OR MOONCHILD_AUDIO_SDL3)
    set(MOONCHILD_USE_SDL3 ON)
else()
    set(MOONCHILD_USE_SDL3 OFF)
endif()

if(MOONCHILD_RENDERER_OPENGL OR MOONCHILD_RENDERER_GLES)
    set(MOONCHILD_USE_OPENGL ON)
else()
    set(MOONCHILD_USE_OPENGL OFF)
endif()

if(MOONCHILD_RENDERER_SDL3 AND NOT MOONCHILD_WINDOW_SDL3)
    message(FATAL_ERROR
        "MOONCHILD_RENDERER_SDL3 requires SDL3 for windowing!")
endif()
if(MOONCHILD_USE_OPENGL AND NOT MOONCHILD_WINDOW_SDL3)
    message(FATAL_ERROR
        "OpenGL renderers require SDL3 for windowing!")
endif()
if(EMSCRIPTEN AND MOONCHILD_RENDERER_OPENGL)
    message(FATAL_ERROR
        "The OpenGL 2.1 renderer is not supported on the web! Use the OpenGL ES 2.0 renderer instead.")
endif()

# Architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 ON)
else()
    set(ARCH_32 ON)
endif()

# SDL3 stuff
if(MOONCHILD_USE_SDL3)
  if(MOONCHILD_VENDORED_SDL3)
    set(SDL_SHARED       OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC       ON  CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS        OFF CACHE BOOL "" FORCE)
    set(SDL_EXAMPLES     OFF CACHE BOOL "" FORCE)
    set(SDL_INSTALL      OFF CACHE BOOL "" FORCE)
    add_subdirectory("${LIBRARY_DIR}/SDL3" "${CMAKE_BINARY_DIR}/SDL3" EXCLUDE_FROM_ALL)

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
    find_package(SDL3)
  endif()
endif()

if(MOONCHILD_RENDERER_OPENGL AND NOT EMSCRIPTEN)
  find_package(OpenGL REQUIRED)
endif()

# zlib stuff
if(MOONCHILD_VENDORED_ZLIB)
  set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(ZLIB_BUILD_SHARED   OFF CACHE BOOL "" FORCE)
  set(ZLIB_BUILD_STATIC   ON  CACHE BOOL "" FORCE)
  add_subdirectory("${LIBRARY_DIR}/zlib" "${CMAKE_BINARY_DIR}/zlib" EXCLUDE_FROM_ALL)
else()
  find_package(ZLIB)
endif()

# SDL_gamecontrollerdb path
if(MOONCHILD_VENDORED_GAMECONTROLLERDB)
  set(MOONCHILD_GAMECONTROLLERDB_PATH "gamecontrollerdb.txt")
else()
  set(MOONCHILD_GAMECONTROLLERDB_PATH "$ENV{MOONCHILD_GAMECONTROLLERDB_PATH}")
  file(TO_CMAKE_PATH "${MOONCHILD_GAMECONTROLLERDB_PATH}" MOONCHILD_GAMECONTROLLERDB_PATH)
endif()

# Game + framework
set(MOONCHILD_GAME_SOURCES
    Classes/moonchild/anim.cpp
    Classes/moonchild/asset.cpp
    Classes/moonchild/asteroid.cpp
    Classes/moonchild/basic.cpp
    Classes/moonchild/bat.cpp
    Classes/moonchild/bbot.cpp
    Classes/moonchild/bee.cpp
    Classes/moonchild/bolt.cpp
    Classes/moonchild/bonus.cpp
    Classes/moonchild/boss.cpp
    Classes/moonchild/bouncey.cpp
    Classes/moonchild/bullet.cpp
    Classes/moonchild/bump.cpp
    Classes/moonchild/cannon.cpp
    Classes/moonchild/chain.cpp
    Classes/moonchild/claw.cpp
    Classes/moonchild/cloud.cpp
    Classes/moonchild/diamond.cpp
    Classes/moonchild/doldoler.cpp
    Classes/moonchild/editor.cpp
    Classes/moonchild/element.cpp
    Classes/moonchild/elevat.cpp
    Classes/moonchild/glim.cpp
    Classes/moonchild/globals.cpp
    Classes/moonchild/goodday.cpp
    Classes/moonchild/gravlift.cpp
    Classes/moonchild/gumbal.cpp
    Classes/moonchild/hoi.cpp
    Classes/moonchild/ironauto.cpp
    Classes/moonchild/ironring.cpp
    Classes/moonchild/levinits.cpp
    Classes/moonchild/lift.cpp
    Classes/moonchild/mc.cpp
    Classes/moonchild/metbal.cpp
    Classes/moonchild/metcan.cpp
    Classes/moonchild/mine.cpp
    Classes/moonchild/mixer.cpp
    Classes/moonchild/mouth.cpp
    Classes/moonchild/mushroom.cpp
    Classes/moonchild/objects.cpp
    Classes/moonchild/paal.cpp
    Classes/moonchild/plant.cpp
    Classes/moonchild/plof.cpp
    Classes/moonchild/prefs.cpp
    Classes/moonchild/ptoei.cpp
    Classes/moonchild/punt.cpp
    Classes/moonchild/robyn.cpp
    Classes/moonchild/rock.cpp
    Classes/moonchild/rocket.cpp
    Classes/moonchild/score.cpp
    Classes/moonchild/scroll.cpp
    Classes/moonchild/seamine.cpp
    Classes/moonchild/smlheart.cpp
    Classes/moonchild/snake.cpp
    Classes/moonchild/sneak.cpp
    Classes/moonchild/sokoban.cpp
    Classes/moonchild/sound.cpp
    Classes/moonchild/spies.cpp
    Classes/moonchild/spike.cpp
    Classes/moonchild/stukhout.cpp
    Classes/moonchild/switsj.cpp
    Classes/moonchild/test.cpp
    Classes/moonchild/trigger.cpp
    Classes/moonchild/vgdll.cpp
    Classes/moonchild/warp.cpp
    Classes/moonchild/weight.cpp
    Classes/moonchild/wheel.cpp
)

set(MOONCHILD_FRAMEWORK_SOURCES
    Classes/framewrk/blitbuf.cpp
    Classes/framewrk/fastfile.cpp
    Classes/framewrk/pcxff.cpp
    Classes/framewrk/sprite.cpp
    Classes/framewrk/timer.cpp
    Classes/framewrk/unzip.c
    Classes/framewrk/video.cpp
)

# Desktop app shell + bridges + audio helpers
set(PLATFORM_APP_SOURCES
    ${PLATFORM_DIR}/App/Main.cpp
    ${PLATFORM_DIR}/App/Host.cpp
    ${PLATFORM_DIR}/App/PlatformBackends.cpp
    ${PLATFORM_DIR}/App/Resources.cpp
    ${PLATFORM_DIR}/App/FrameworkGlue.cpp
    ${PLATFORM_DIR}/App/MediaCatalog.cpp
    ${PLATFORM_DIR}/Core/Random.cpp
    ${PLATFORM_DIR}/Bridges/AudioBridge.cpp
    ${PLATFORM_DIR}/Bridges/DisplayBridge.cpp
    ${PLATFORM_DIR}/Bridges/InputBridge.cpp
    ${PLATFORM_DIR}/Bridges/MovieBridge.cpp
    ${PLATFORM_DIR}/Audio/MP3MusicTrack.cpp
    ${PLATFORM_DIR}/Audio/AudioMixer.cpp
)

# Backends
set(BACKEND_SOURCES "")
set(BACKEND_INCLUDES "")

if(MOONCHILD_WINDOW_SDL3)
    list(APPEND BACKEND_SOURCES  ${PLATFORM_DIR}/Backends/Window/SDL3Window.cpp)
    list(APPEND BACKEND_INCLUDES ${PLATFORM_DIR}/Backends/Window)
endif()

if(MOONCHILD_RENDERER_GLES)
    list(APPEND BACKEND_SOURCES
        ${PLATFORM_DIR}/Backends/Renderer/GLESRenderer.cpp
        ${LIBRARY_DIR}/glad/glad.c
    )
    list(APPEND BACKEND_INCLUDES
        ${PLATFORM_DIR}/Backends/Renderer
        ${LIBRARY_DIR}/glad
    )
endif()

if(MOONCHILD_RENDERER_OPENGL)
    list(APPEND BACKEND_SOURCES
        ${PLATFORM_DIR}/Backends/Renderer/OpenGLRenderer.cpp
        ${LIBRARY_DIR}/glad/glad.c
    )
    list(APPEND BACKEND_INCLUDES
        ${PLATFORM_DIR}/Backends/Renderer
        ${LIBRARY_DIR}/glad
    )
endif()

if(MOONCHILD_RENDERER_SDL3)
    list(APPEND BACKEND_SOURCES  ${PLATFORM_DIR}/Backends/Renderer/SDL3Renderer.cpp)
    list(APPEND BACKEND_INCLUDES ${PLATFORM_DIR}/Backends/Renderer)
endif()

if(MOONCHILD_INPUT_SDL3)
    list(APPEND BACKEND_SOURCES  ${PLATFORM_DIR}/Backends/Input/SDL3Input.cpp)
    list(APPEND BACKEND_INCLUDES ${PLATFORM_DIR}/Backends/Input)
endif()

if(MOONCHILD_AUDIO_SDL3)
    list(APPEND BACKEND_SOURCES  ${PLATFORM_DIR}/Backends/Audio/SDL3Audio.cpp)
    list(APPEND BACKEND_INCLUDES
        ${PLATFORM_DIR}/Backends/Audio
        ${PLATFORM_DIR}/Audio
        ${LIBRARY_DIR}/dr_libs
    )
endif()

list(REMOVE_DUPLICATES BACKEND_INCLUDES)

set(SOURCES
    ${MOONCHILD_GAME_SOURCES}
    ${MOONCHILD_FRAMEWORK_SOURCES}
    ${PLATFORM_APP_SOURCES}
    ${BACKEND_SOURCES}
)

# Target
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${MOONCHILD_CLASSES_DIR}
    ${MOONCHILD_FRAMEWORK_DIR}
    ${MOONCHILD_GAME_DIR}
    ${PLATFORM_DIR}/Core
    ${PLATFORM_DIR}/App
    ${PLATFORM_DIR}/Bridges
    ${BACKEND_INCLUDES}
)
if(MOONCHILD_VENDORED_PL_MPEG)
  target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${LIBRARY_DIR}/pl_mpeg
  )
else()
  target_include_directories(${EXECUTABLE_NAME} PRIVATE
    $ENV{MOONCHILD_PL_MPEG_PATH}
  )
endif()

# Backend selection macros
if(MOONCHILD_WINDOW_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOW_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_HAS_DISPLAY_OPTIONS)
endif()
if(MOONCHILD_RENDERER_OPENGL)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_OPENGL)
endif()
if(MOONCHILD_RENDERER_GLES)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_GLES)
endif()
if(MOONCHILD_RENDERER_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_SDL3)
endif()
if(MOONCHILD_INPUT_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_INPUT_SDL3)
    if(NOT MOONCHILD_GAMECONTROLLERDB_PATH STREQUAL "")
        target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_GAMECONTROLLERDB_PATH=\"${MOONCHILD_GAMECONTROLLERDB_PATH}\"
        )
    endif()
endif()
if(MOONCHILD_AUDIO_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_AUDIO_SDL3)
endif()
if(MOONCHILD_USE_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_USE_SDL3 SDL_MAIN_HANDLED)
endif()
if(MOONCHILD_USE_OPENGL)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_USE_OPENGL)
endif()

if(MOONCHILD_VENDORED_ZLIB)
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE zlibstatic)
else()
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE ZLIB::ZLIB)
endif()

if(MOONCHILD_USE_SDL3 AND MOONCHILD_VENDORED_SDL3)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${MOONCHILD_SDL_TARGET})
elseif(MOONCHILD_USE_SDL3 AND NOT MOONCHILD_VENDORED_SDL3)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL3::SDL3)
endif()

if(MOONCHILD_RENDERER_OPENGL)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenGL::GL)
endif()

# Platform specific configuration
if(EMSCRIPTEN)
    set(PLATFORM_NAME "Web")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WEB)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Web.cmake)
    if(NOT MOONCHILD_OUTPUT_PLATFORM)
        set(MOONCHILD_OUTPUT_PLATFORM "Web")
    endif()
elseif(WIN32 AND ARCH_64)
    set(PLATFORM_NAME "Windows")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Windows.cmake)
    if(NOT MOONCHILD_OUTPUT_PLATFORM)
        set(MOONCHILD_OUTPUT_PLATFORM "WindowsX64")
    endif()
elseif(WIN32 AND ARCH_32)
    set(PLATFORM_NAME "Windows")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Windows.cmake)
    if(NOT MOONCHILD_OUTPUT_PLATFORM)
        set(MOONCHILD_OUTPUT_PLATFORM "WindowsX86")
    endif()
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_NAME "Linux")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_LINUX)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Linux.cmake)
endif()

# Output directories
if(NOT MOONCHILD_OUTPUT_PLATFORM)
    set(MOONCHILD_OUTPUT_PLATFORM "${MOONCHILD_TARGET_ARCH}")
endif()

set(MOONCHILD_BIN_DIR ${CMAKE_SOURCE_DIR}/Bin/${MOONCHILD_OUTPUT_PLATFORM})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${MOONCHILD_BIN_DIR}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${MOONCHILD_BIN_DIR}/Release
)

if(MOONCHILD_INPUT_SDL3 AND NOT EMSCRIPTEN AND MOONCHILD_VENDORED_GAMECONTROLLERDB)
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBRARY_DIR}/SDL_GameControllerDB/gamecontrollerdb.txt"
            $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/gamecontrollerdb.txt
    )
endif()

set(MOONCHILD_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${MOONCHILD_RESOURCES_DIR}"
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/data
)

# MSVC shut up
if(MSVC)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
