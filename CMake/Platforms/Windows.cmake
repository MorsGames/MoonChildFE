target_compile_definitions(${EXECUTABLE_NAME} PRIVATE NOMINMAX)

target_sources(${EXECUTABLE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/MoonChild.rc")

if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2 /GL /fp:fast>
    )
    target_link_options(${EXECUTABLE_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>
    )
endif()
