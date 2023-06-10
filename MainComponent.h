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
#include "RubberBandStretcher.h"

#pragma once

//==============================================================================
class MainComponent   : public juce::AudioAppComponent, public juce::Timer
{
public:
    MainComponent();
    
    ~MainComponent() override;

    
    // prepareToPlay, getNextAudioBlock, releaseResources는 juce::AudioAppComponent의 pure virtual functions이다.
    void prepareToPlay (int, double) override; // 오디오 진행 시작전에 호출된다.
    
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override; // 오디오 데이터의 새 블럭이 오디오 하드웨어가 필요할때마다 호출된다.
    
    void releaseResources() override; // 오디오 진행이 끝난 후 호출된다.
    

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
    
    juce::TextButton header;
    juce::TextButton visualize;
    juce::TextButton footer;
    juce::TextButton sidebar;
    
    // 첫번째 음원용
    juce::TextButton openButton;
    juce::TextButton clearButton;
    juce::TextButton visualizeButton;
    
    juce::Slider    masterVolumeSlider;     // 볼륨 슬라이더: 오디오의 볼륨을 조절한다
    juce::Slider    grainNumberSlider;      // 첫번째 슬라이더: 그레인의 개수를 결정한다
    juce::Slider    grainPositionSlider;    // 두번째 슬라이더: 불러온 오디오의 위치를 결정한다
    juce::Slider    grainLengthSlider;      // 세번째 슬라이더: 불러올 오디오 그레인의 길이를 결정한다
    juce::Slider    grainFrequencySlider;   // 네번째 슬라이더: 불러온 그레인의 개수를 결정한다
    juce::Slider    panningRandomizeSlider; // 다섯번째 슬라이더: 패닝의 랜덤성을 결정한다
    juce::Slider    panningSlider;          // 여섯번째 슬라이더: 패닝의 위치를 결정한다
    
    juce::Label  masterVolumeLabel;         // 각 슬라이더에 대한 레이블링 적용
    juce::Label  grainNumberLabel;
    juce::Label  grainPositionLabel;
    juce::Label  grainLengthLabel;
    juce::Label  grainFrequencyLabel;
    juce::Label  panningRandomizeLabel;
    juce::Label  panningLabel;
    
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
    juce::AudioSampleBuffer fileBuffer; // AudioSampleBuffer -> Necessary for sampling applications that manipulate recorded audio data. (https://docs.juce.com/master/tutorial_looping_audio_sample_buffer.html)
    Oscilloscope2D * oscilloscope2D;
    
    RingBuffer<float> * ringBuffer2;
    juce::AudioFormatManager formatManager2;
    juce::AudioSampleBuffer fileBuffer2;
    Oscilloscope2D * oscilloscope2D2;
    
    float currentRand;
    float currentRand2;
    juce::Random random;
    
    //juce::dsp::WindowingFunction<float> window();
    int iteration = 0;
    double pitchShiftRatio = 1.0;

    
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
