cmake_minimum_required(VERSION 3.27)

# ── Version ───────────────────────────────────────────────────────────────────
# Developer edits MAJOR.MINOR.PATCH here. The trailing rev is derived
# automatically from the git commit count (see cmake/GenerateVersion.cmake).
project(SUPERVIRTUALKEYBOARD VERSION 0.4.0)

# Derive the rev now (configure time) so it can be baked into the plugin's
# version metadata. The header is additionally regenerated at build time so the
# rev stays current after new commits without re-running CMake configure.
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateVersion.cmake")
set(SVK_VERSION_FULL "${PROJECT_VERSION}.${SVK_VERSION_REV}")
message(STATUS "SuperVirtualKeyboard version: ${SVK_VERSION_FULL}")

add_subdirectory($ENV{juce_DIR} subprojects/juce)

# When ON, the built VST3 is copied to the OS default plugin folder after each
# build (Windows: %CommonProgramFiles%\VST3, macOS: ~/Library/Audio/Plug-Ins/VST3,
# Linux: ~/.vst3). On Windows the system VST3 folder is under Program Files, so
# the build must run elevated (admin) or the copy step will fail.
option(SVK_COPY_PLUGIN_AFTER_BUILD "Copy the built VST3 to the OS plugin folder after building" ON)

# On Windows, copy to a user-writable folder so the post-build copy doesn't need
# admin rights (the default %CommonProgramFiles%\VST3 lives under Program Files).
# JUCE_VST3_COPY_DIR is an INHERITED property read during juce_add_plugin, so it
# must be set (at directory scope) before the call below. Point your DAW to also
# scan this folder.
if(WIN32)
    set_property(DIRECTORY PROPERTY JUCE_VST3_COPY_DIR "$ENV{LOCALAPPDATA}/Programs/Common/VST3")
endif()

juce_add_plugin(
    SuperVirtualKeyboard
        PRODUCT_NAME "SuperVirtualKeyboard"
        VERSION "${SVK_VERSION_FULL}"
        BUNDLE_ID "com.everytone.supervirtualkeyboard"
        PLUGIN_MANUFACTURER_CODE evtn
        PLUGIN_CODE Svk0
        FORMATS VST3 Standalone
        COPY_PLUGIN_AFTER_BUILD ${SVK_COPY_PLUGIN_AFTER_BUILD}
        DOCUMENT_EXTENSIONS "svk"
        )
juce_generate_juce_header(SuperVirtualKeyboard)

set_directory_properties(
    PROPERTIES
        COMPANY_COPYRIGHT "AGPL-3"
        COMPANY_NAME "Everytone"
        COMPANY_WEBSITE "https://www.everytone.zone"
        COMPANY_EMAIL "someone@everytone.zone"
)

juce_add_binary_data(SuperVirtualKeyboard_BinaryData
    SOURCES
        Resources/FactoryModes.txt
)

file(GLOB_RECURSE SourceCode
    CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
    )

target_sources(
    SuperVirtualKeyboard
        PRIVATE
            ${SourceCode}
            )

# ── Generated Version.h ───────────────────────────────────────────────────────
# Regenerate Version.h on every build so the git-derived rev stays current
# without re-running CMake configure. Source files use #include "Version.h".
set(SVK_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(SVK_VERSION_HEADER "${SVK_GENERATED_DIR}/Version.h")

add_custom_target(SvkGenerateVersion
    BYPRODUCTS "${SVK_VERSION_HEADER}"
    COMMAND "${CMAKE_COMMAND}"
        -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
        -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
        -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
        -DDST=${SVK_VERSION_HEADER}
        -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateVersion.cmake
    COMMENT "Generating Version.h (git rev)"
    VERBATIM
)
add_dependencies(SuperVirtualKeyboard SvkGenerateVersion)
target_include_directories(SuperVirtualKeyboard PRIVATE "${SVK_GENERATED_DIR}")

target_compile_definitions(SuperVirtualKeyboard
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        # JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:SuperVirtualKeyboard,JUCE_PRODUCT_NAME>"
        # JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:SuperVirtualKeyboard,JUCE_VERSION>"
        DONT_SET_USING_JUCE_NAMESPACE=0
        JUCE_VST3_CAN_REPLACE_VST2=0
    )

target_link_libraries(SuperVirtualKeyboard
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_opengl
        SuperVirtualKeyboard_BinaryData

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
    )
