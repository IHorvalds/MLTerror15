cmake_minimum_required(VERSION 3.15)


set(CMAKE_CXX_STANDARD 20)
project(NeuAmpNetDemo VERSION 0.1)

set(RTNEURAL_EIGEN ON CACHE BOOL "Use RTNeural with this backend" FORCE)
add_subdirectory(JUCE)
add_subdirectory(RTNeural)
include_directories(RTNeural)

set(JUCE_TARGET_HAS_BINARY_DATA TRUE)
set(JUCE_BUILD_EXTRAS TRUE)

juce_add_plugin(NeuAmpNetDemo
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE IHpd
    PLUGIN_CODE Nan0
    COMPANY_NAME "IH Pedals"    
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Tiny Terror NAN Demo")


juce_generate_juce_header(NeuAmpNetDemo)

set(NAN_CLIPPING_THRESHOLD TRUE)

if(NAN_CLIPPING_THRESHOLD)
    add_compile_definitions(CLIPPING_THRESHOLD 0.3f)
endif()


# Add source to this project's executable.
# add_executable (NeuAmpNetDemo2 "NeuAmpNetDemo2.cpp" "NeuAmpNetDemo2.h")

add_subdirectory(Source)

target_compile_definitions(NeuAmpNetDemo
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(NeuAmpBinaries 
    NAMESPACE assets
        SOURCES
            python/0.5-0.5-0.5-model-gru-4.json
            python/0.5-0.5-0.5-model-lstm-1.json
            Source/Assets/UI/UIBlank.png
            Source/Assets/UI/Switch.png
    )

target_link_libraries(NeuAmpNetDemo
    PRIVATE
        NeuAmpBinaries
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_data_structures
        juce::juce_core
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_processors
        juce::juce_audio_plugin_client
        juce::juce_audio_formats
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        RTNeural)