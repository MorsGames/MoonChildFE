target_link_libraries(${EXECUTABLE_NAME} PRIVATE dl)

# Bundling macOS application
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Moon Child FE"
    MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
    MACOSX_BUNDLE_GUI_IDENTIFIER com.mors.${EXECUTABLE_NAME}
    MACOSX_BUNDLE_ICON_FILE AppIcon.icns
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION}
    MACOSX_BUNDLE_LONG_VERSION_STRING ${CMAKE_PROJECT_VERSION}
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/macOS/MoonChild.plist.in"
)

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/macOS/AppIcon.icns"
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/AppIcon.icns
)

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/macOS/Assets.car"
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/Assets.car
)

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/macOS/Credits.rtf"
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/Credits.rtf
)
