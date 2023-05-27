/****************************************************************************
dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                  juce_audio_processors, juce_audio_utils, juce_core,
                  juce_data_structures, juce_events, juce_graphics,
                  juce_gui_basics, juce_gui_extra
exporters:        xcode_mac, vs2019, linux_make

type:             Component
mainClass:        MainComponent

useLocalCopy:     1

END_JUCE_PIP_METADATA

***************************************************************************/

#include <JuceHeader.h>

#pragma once

//==============================================================================
class MainComponent   : public juce::AudioAppComponent
{
public:
    MainComponent();
    
    ~MainComponent() override;

    void prepareToPlay (int, double) override;

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    void resized() override;
    
    void paint (juce::Graphics& g) override;

private:
    void openButtonClicked();

    void clearButtonClicked();
    
    double generateHannWindow(int size, int pos);
    

   //==========================================================================
   juce::TextButton openButton;
   juce::TextButton clearButton;
    juce::Slider    grainPositionSlider;
    juce::Slider     grainLengthSlider;

    
    int grainSize = 0;
    //std::vector<float> windowVector(0);
    float* windowArray;
    int position = 0;
    int startPosition = 1024;
    int windowPosition = 0;
    int newStartPosition = 1024;
    int newGrainSize = 0;
    int windowSize = 0;
    
   std::unique_ptr<juce::FileChooser> chooser;


   juce::AudioFormatManager formatManager;
   juce::AudioSampleBuffer fileBuffer;
   

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
