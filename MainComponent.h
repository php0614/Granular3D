/****************************************************************************
dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                  juce_audio_processors, juce_audio_utils, juce_core,
                  juce_data_structures, juce_events, juce_graphics,
                  juce_gui_basics, juce_gui_extra, juce_dsp, juce_opengl
exporters:        xcode_mac, vs2019, linux_make

type:             Component
mainClass:        MainComponent

useLocalCopy:     1

END_JUCE_PIP_METADATA

***************************************************************************/

#include <JuceHeader.h>
#include "Oscilloscope2D.h"
#include "RingBuffer.h"

#pragma once

//==============================================================================
class MainComponent   : public juce::AudioAppComponent, public juce::Timer
{
public:
    MainComponent();
    
    ~MainComponent() override;

    void prepareToPlay (int, double) override;

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    void resized() override;
    
    void paint (juce::Graphics& g) override;
    
    void timerCallback() override;
    
    void mouseMove (const juce::MouseEvent& event);

private:
    // 첫번째 음원용
    void openButtonClicked();

    void clearButtonClicked();
    
    void visualizeButtonClicked();
    
    // 두번째 음원용
    void openButtonClicked2();

    void clearButtonClicked2();
    
    void visualizeButtonClicked2();
    
    double generateHannWindow(int size, int pos);
    

    //==========================================================================
    // 첫번째 음원용
    juce::TextButton openButton;
    juce::TextButton clearButton;
    juce::TextButton visualizeButton;
    juce::Slider    grainPositionSlider;    // 첫번째 슬라이더: 불러온 오디오의 위치를 결정한다
    juce::Slider    grainLengthSlider;      // 두번째 슬라이더: 불러올 오디오 그레인의 길이를 결정한다
    juce::Slider    grainNumberSlider;      // 세번째 슬라이더: 불러온 그레인의 개수를 결정한다
    juce::Slider    panningRandomizeSlider; // 네번째 슬라이더: 패닝의 랜덤성을 결정한다
    juce::Slider    panningSlider;          // 다섯번째 슬라이더: 패닝의 위치를 결정한다
    
    // 두번째 음원용
    juce::TextButton openButton2;
    juce::TextButton clearButton2;
    juce::TextButton visualizeButton2;
    juce::Slider    grainPositionSlider2;    // 첫번째 슬라이더: 불러온 오디오의 위치를 결정한다
    juce::Slider    grainLengthSlider2;      // 두번째 슬라이더: 불러올 오디오 그레인의 길이를 결정한다
    juce::Slider    grainNumberSlider2;      // 세번째 슬라이더: 불러온 그레인의 개수를 결정한다
    juce::Slider    panningRandomizeSlider2; // 네번째 슬라이더: 패닝의 랜덤성을 결정한다
    juce::Slider    panningSlider2;          // 다섯번째 슬라이더: 패닝의 위치를 결정한다

    
    float pannings = 0.5;
    float pannings2 = 0.5;
    
    ///////// 첫번째 음원
    int grainSize = 0;
    int grainNumber = 1;
    //std::vector<float> windowVector(0);
    float* windowArray;
    int position = 0;
    int startPosition = 1024;
    int windowPosition = 0;
    
    int newStartPosition = 1024;
    int newGrainSize = 0;
    int newGrainNumber = 1;
    int windowSize = 0;
    float currentGrainPan = 0.5;
    
    ////////// 두번째 음원
    int grainSize2 = 0;
    int grainNumber2 = 1;
    float* windowArray2;
    int position2 = 0;
    int startPosition2 = 1024;
    int windowPosition2 = 0;
    
    int newStartPosition2 = 1024;
    int newGrainSize2 = 0;
    int newGrainNumber2 = 1;
    int windowSize2 = 0;
    float currentGrainPan2 = 0.5;
    
    float mouseX = 0;
    float mouseY = 0;
    
    std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<juce::FileChooser> chooser2;

    RingBuffer<float> * ringBuffer;
    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer fileBuffer;
    Oscilloscope2D * oscilloscope2D;
    
    RingBuffer<float> * ringBuffer2;
    juce::AudioFormatManager formatManager2;
    juce::AudioSampleBuffer fileBuffer2;
    Oscilloscope2D * oscilloscope2D2;
    
    float currentRand;
    float currentRand2;
    juce::Random random;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
