set(CMAKE_SYSTEM_NAME Windows)

if(NOT DEFINED MOONCHILD_TARGET_ARCH)
    set(MOONCHILD_TARGET_ARCH "WinX64")
endif()

find_program(_MOONCHILD_WINDOWS_COMPILER clang-cl)
if(NOT _MOONCHILD_WINDOWS_COMPILER)
    message(FATAL_ERROR "clang-cl was not found! Install LLVM and run this from a Visual Studio developer prompt.")
endif()

# Windows x64
if(MOONCHILD_TARGET_ARCH STREQUAL "WinX64")
    set(CMAKE_SYSTEM_PROCESSOR AMD64)

# Windows x86
elseif(MOONCHILD_TARGET_ARCH STREQUAL "WinX86")
    set(CMAKE_SYSTEM_PROCESSOR X86)
    set(CMAKE_C_FLAGS_INIT "--target=i686-pc-windows-msvc")
    set(CMAKE_CXX_FLAGS_INIT "--target=i686-pc-windows-msvc")
    set(CMAKE_EXE_LINKER_FLAGS_INIT "--target=i686-pc-windows-msvc")
    set(CMAKE_SHARED_LINKER_FLAGS_INIT "--target=i686-pc-windows-msvc")

else()
    message(FATAL_ERROR "Unsupported Windows target architecture: ${MOONCHILD_TARGET_ARCH}")
endif()

set(CMAKE_C_COMPILER "${_MOONCHILD_WINDOWS_COMPILER}" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${_MOONCHILD_WINDOWS_COMPILER}" CACHE FILEPATH "" FORCE)
