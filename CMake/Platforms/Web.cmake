target_compile_options(${EXECUTABLE_NAME} PRIVATE
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-O3>
)

target_link_options(${EXECUTABLE_NAME} PRIVATE
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-O3>
    --shell-file "${CMAKE_SOURCE_DIR}/Platform/Web/shell.html"
    --pre-js "${CMAKE_SOURCE_DIR}/Platform/Web/pre.js"
    "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/Data@/data"
    "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/Libraries/SDL_GameControllerDB/gamecontrollerdb.txt@/gamecontrollerdb.txt"
    -sALLOW_MEMORY_GROWTH=1
    -sEXIT_RUNTIME=0
    -sFORCE_FILESYSTEM=1
    -sEXPORTED_RUNTIME_METHODS=callMain
    -sMIN_WEBGL_VERSION=1
    -sMAX_WEBGL_VERSION=1
    -lidbfs.js
)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    SUFFIX ".html"
)
