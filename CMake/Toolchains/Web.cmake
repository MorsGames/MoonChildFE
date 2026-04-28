if(NOT DEFINED MOONCHILD_TARGET_ARCH)
    set(MOONCHILD_TARGET_ARCH "WASM")
endif()

if(NOT DEFINED ENV{EMSDK})
    message(FATAL_ERROR "Emscripten SDK is not installed!")
endif()

include("$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
