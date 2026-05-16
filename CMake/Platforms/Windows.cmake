configure_file(
    "${CMAKE_SOURCE_DIR}/Windows/MoonChild.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MoonChild.rc"
    @ONLY
)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE NOMINMAX)

target_sources(${EXECUTABLE_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MoonChild.rc")
