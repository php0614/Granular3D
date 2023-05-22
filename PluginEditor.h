/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Granular3DAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    Granular3DAudioProcessorEditor (Granular3DAudioProcessor&);
    ~Granular3DAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Granular3DAudioProcessor& audioProcessor;
    
    //////////////
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonChanged();
    void timerCallback() override;
    void updateLoopState (bool shouldLoop);

    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    //토글은 아직 미구현
    juce::ToggleButton loopingToggle;
    juce::Label currentPositionLabel;

    juce::AudioFormatManager formatManager;
 
    
    //GUI파일선택기능한 Chooser. 인터렉티브하게 파일을 불러올 때 아주 많이 쓰이는 듯////////
    std::unique_ptr<juce::FileChooser> chooser;
    ////////////////////////////////////////////
    
    
    enum TransportState
       {
           Stopped,
           Starting,
           Playing,
           Stopping
       };
    
    TransportState state;

    void changeState (TransportState newState);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Granular3DAudioProcessorEditor)
};
