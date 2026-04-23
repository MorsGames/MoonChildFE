cmake_minimum_required(VERSION 3.28.3)

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

# Backend selection
option(MOONCHILD_WINDOW_SDL3 "Use SDL3 windowing backend" ON)

option(MOONCHILD_RENDERER_GL "Use the OpenGL ES 2.0 renderer" ON)
option(MOONCHILD_RENDERER_SDL3 "Use SDL3's default renderer" OFF)

option(MOONCHILD_INPUT_SDL3 "Use SDL3 input backend" ON)

option(MOONCHILD_AUDIO_SDL3 "Use SDL3 audio backend" ON)

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

# Aggregate flags
if(MOONCHILD_WINDOW_SDL3 OR MOONCHILD_RENDERER_SDL3 OR MOONCHILD_INPUT_SDL3 OR MOONCHILD_AUDIO_SDL3)
    set(MOONCHILD_USE_SDL3 ON)
else()
    set(MOONCHILD_USE_SDL3 OFF)
endif()

if(MOONCHILD_RENDERER_GL)
    set(MOONCHILD_USE_OPENGL ON)
else()
    set(MOONCHILD_USE_OPENGL OFF)
endif()

# Sanity checks
if(MOONCHILD_RENDERER_GL AND MOONCHILD_RENDERER_SDL3)
    message(FATAL_ERROR
        "MOONCHILD_RENDERER_GL and MOONCHILD_RENDERER_SDL3 cannot be enabled at the same time.")
endif()
if(MOONCHILD_RENDERER_SDL3 AND NOT MOONCHILD_WINDOW_SDL3)
    message(FATAL_ERROR
        "MOONCHILD_RENDERER_SDL3 requires MOONCHILD_WINDOW_SDL3.")
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
# TODO: Unglobify this
file(GLOB MOONCHILD_GAME_SOURCES CONFIGURE_DEPENDS
    ${MOONCHILD_GAME_DIR}/*.cpp
)

file(GLOB MOONCHILD_FRAMEWORK_SOURCES CONFIGURE_DEPENDS
    ${MOONCHILD_FRAMEWORK_DIR}/*.cpp
    ${MOONCHILD_FRAMEWORK_DIR}/*.c
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

if(MOONCHILD_RENDERER_GL)
    list(APPEND BACKEND_SOURCES
        ${PLATFORM_DIR}/Backends/Renderer/GLRenderer.cpp
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
    ${LIBRARY_DIR}/pl_mpeg
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
if(MOONCHILD_RENDERER_GL)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_GL)
endif()
if(MOONCHILD_RENDERER_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_RENDERER_SDL3)
endif()
if(MOONCHILD_INPUT_SDL3)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_INPUT_SDL3)
    if(NOT MOONCHILD_GAMECONTROLLERDB_PATH STREQUAL "")
        target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
            MOONCHILD_GAMECONTROLLERDB_PATH=\"${MOONCHILD_GAMECONTROLLERDB_PATH}\"
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

# MSVC shut up
if(MSVC)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
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

# Platform specific configuration
if(EMSCRIPTEN)
    set(PLATFORM_NAME "Emscripten")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_EMSCRIPTEN)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Emscripten.cmake)
elseif(WIN32 AND ARCH_64)
    set(PLATFORM_NAME "Windows64")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Windows.cmake)
elseif(WIN32 AND ARCH_32)
    set(PLATFORM_NAME "Windows32")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Windows.cmake)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_NAME "Linux")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE MOONCHILD_LINUX)
    include(${CMAKE_CURRENT_LIST_DIR}/CMake/Platforms/Linux.cmake)
else()
    message(FATAL_ERROR "Unrecognized compile target!")
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
