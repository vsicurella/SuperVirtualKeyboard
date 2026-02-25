cmake_minimum_required(VERSION 3.27)

project(SUPERVIRTUALKEYBOARD VERSION 0.4.0)

add_subdirectory($ENV{juce_DIR} subprojects/juce)

juce_add_plugin(
    SuperVirtualKeyboard
        PRODUCT_NAME "SuperVirtualKeyboard"
        VERSION "0.4.0a"
        BUNDLE_ID "com.everytone.supervirtualkeyboard"
        PLUGIN_MANUFACTURER_CODE evtn
        PLUGIN_CODE Svk0
        FORMATS VST3 Standalone
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

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
    )
