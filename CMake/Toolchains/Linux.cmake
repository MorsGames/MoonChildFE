set(CMAKE_SYSTEM_NAME Linux)

if(NOT DEFINED MOONCHILD_TARGET_ARCH)
    set(MOONCHILD_TARGET_ARCH "LinuxX64")
endif()

# Linux x64
if(MOONCHILD_TARGET_ARCH STREQUAL "LinuxX64")
    set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Linux AArch64
elseif(MOONCHILD_TARGET_ARCH STREQUAL "LinuxArm64")
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
else()
    message(FATAL_ERROR "Unsupported Linux target architecture: ${MOONCHILD_TARGET_ARCH}")
endif()
