/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Granular3DAudioProcessor  : public juce::AudioProcessor
                              #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
                            , public juce::ChangeListener
{
public:
    //==============================================================================
    Granular3DAudioProcessor();
    ~Granular3DAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    //오디오 샘플 플레이 관련//////
    void audioFileLoader(juce::File& file);
    juce::AudioTransportSource transportSource;
    //transport를 set할 수 있는 기능들을 함수로 따로 만들었음. 에디터에서 reader를 받아 funtion이 트리거 되며 작동
    void setTransportSource(juce::AudioFormatReader* reader);
    ////////////////////////////////////////////
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
private:

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Granular3DAudioProcessor)
};
