/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             PlayingSoundFilesTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Plays audio files.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
// juce::Component가 아니라 juce::AudioAppComponent를 상속한다.
/*
 AudioAppComponent 클래스는 abstract base class로써, 3개의 순수한 가상함수를 제공한다. 이들은 오디오 응용의 lifecycle을 나타내며 derived class에서 반드시 구현해야하는 함수들이다.

 AudioAppComponent::prepareToPlay(): This is called just before audio processing starts.
 AudioAppComponent::releaseResources(): This is called when audio processing has finished.
 AudioAppComponent::getNextAudioBlock(): This is called "each time" the audio hardware needs a "new block of audio data".
 
 위 3개의 함수 중에 가장 중요한 함수는 getNextAudioBlock이다.(since this is where you will either generate or process audio in your JUCE audio application)
 
 오디오 하드웨어는 오디오의 각 초당 각 채널(모노,스테레오,...)에 특정 개수의 샘플을 필요로 한다.
 예로들어, CD는 각 초당 각 채널에 44100개의 샘플을 필요로 한다.
 오디오 하드웨어에 샘플을 넘겨줄 때 한개씩 넘겨주지 않고, 실제로는 샘플 덩어리(버퍼)를 넘겨주는 방식으로 작동한다.
 예로 들어, sample rate가 44.1kHz이고 버퍼 사이즈가 441이면 1초에 100번씩 버퍼가 오디오 하드웨어에 들어온다고 생각하면 된다. 이는 즉, AudioAppComponent::getNextAudioBlock() 함수가 1초에 100번씩 불러지는 것이다.

 사실 버퍼 사이즈가 441인 것은 매우 부자연스럽고, 256, 512, 1024 이런 식으로 power of 2로 설정하곤 한다.
 
 JUCE audio application이 잘 작동하기 위해서는 중요히 사용되는 함수가 2개 더 있다.
 
 AudioAppComponent::setAudioChannels(): We must call this to register the number of input and output channels we need. Typically, we do this in our constructor. In turn, this function triggers the start of audio processing in our application.
 
 AudioAppComponent::shutdownAudio(): We must call this to shutdown the audio system. Typically, we do this in our destructor.
 
 */
class MainContentComponent   : public juce::AudioAppComponent,
                               public juce::ChangeListener
{
public:
    MainContentComponent()
        : state (Stopped)
    {
        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.onClick = [this] { openButtonClicked(); };

        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);
        
        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);

        setSize (300, 150);

        formatManager.registerBasicFormats();       // [1]
        transportSource.addChangeListener (this);   // [2]

        setAudioChannels (0, 2); // 0개 인풋, 2개 아웃풋 채널 {오디오 프로세싱 시작 신호탄}
    }

    ~MainContentComponent() override  // 소멸자
    {
        shutdownAudio(); // 오디오 시스템을 셧다운하기 위해서 꼭 불러줘야 함.
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
        g.setColour(juce::Colours::white);
        g.setFont(20.0f);
        g.drawFittedText ("Hello Philip!, Hi Yonghyun!", getLocalBounds(), juce::Justification::centred, 2);
    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        transportSource.getNextAudioBlock(bufferToFill);

        if (state == Playing)
        {
            const int numSamples = bufferToFill.numSamples;
            const int numChannels = bufferToFill.buffer->getNumChannels();

            for (int channel = 0; channel < numChannels; ++channel)
            {
                float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

                for (int sample = 0; sample < numSamples; ++sample)
                {
                    // Generate a random value between -1 and 1
                    const float noiseValue = 0.1f * (juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f);

                    // Add the white noise to the sample
                    channelData[sample] += noiseValue;
                }
            }
        }
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void resized() override
    {
        openButton.setBounds (10, 10, getWidth() - 20, 20);
        playButton.setBounds (10, 40, getWidth() - 20, 20);
        stopButton.setBounds (10, 70, getWidth() - 20, 20);
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState (Playing);
            else if ((state == Stopping) || (state == Playing))
                changeState (Stopped);
            else if (Pausing == state)
                changeState (Paused);
        }
    }

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };

    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:                           // [3]
                    playButton.setButtonText ("Play");
                    stopButton.setButtonText ("Stop");
                    stopButton.setEnabled (false);
                    transportSource.setPosition (0.0);
                    break;

                case Starting:                          // [4]
                    transportSource.start();
                    break;

                case Playing:                           // [5]
                    playButton.setButtonText ("Pause");
                    stopButton.setButtonText ("Stop");
                    stopButton.setEnabled (true);
                    break;
                    
                case Pausing:
                    transportSource.stop();
                    break;
 
                case Paused:
                    playButton.setButtonText ("Resume");
                    stopButton.setButtonText ("Return to Zero");
                    break;

                case Stopping:                          // [6]
                    transportSource.stop();
                    break;
            }
        }
    }

    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File{},
                                                       "*.wav");                     // [7]
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)     // [8]
        {
            auto file = fc.getResult();

            if (file != juce::File{})                                                // [9]
            {
                auto* reader = formatManager.createReaderFor (file);                 // [10]

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);   // [11]
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);       // [12]
                    playButton.setEnabled (true);                                                      // [13]
                    readerSource.reset (newSource.release());                                          // [14]
                }
            }
        });
    }

    void playButtonClicked()
    {
        if ((state == Stopped) || (state == Paused))
            changeState (Starting);
        else if (state == Playing)
            changeState (Pausing);    }

    void stopButtonClicked()
    {
        if (state == Paused)
            changeState (Stopped);
        else
            changeState (Stopping);
    }


    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
