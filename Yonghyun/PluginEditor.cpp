/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

int Granular3DAudioProcessorEditor::state = 0;

//==============================================================================
//audioProcess -> PluginEditor에서 PluginProcessor에 접근하는 포인터.
//둘 간의 상호작용은 에디터에서 프로세서에 액세스하고 값을 돌려 받는 방식이 되어야지, 반대가 되어서는 안됨
//반대가 될 경우 오디오가 끊길 위험성이 있음.
//예를 들면 PluginProcessor의 class안에 hello라는 변수 또는 함수가 있다면, PluginEditor안에서 audioProcess.hello나 audioProcess.hello()와 같이 접근함

//Granular3DAudioProcessorEditor::TransportState state = Granular3DAudioProcessorEditor::Stopped;

Granular3DAudioProcessorEditor::Granular3DAudioProcessorEditor (Granular3DAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //state = Stopped;
    
    startTimer (20);
    setSize (1024, 768);
    setResizable (true, false);
    setResizeLimits (640, 480, 6000, 6000);
    playButton.setEnabled (true);
    
    addAndMakeVisible (openButton);
    openButton.setButtonText ("Open...");
    openButton.onClick = [this] () { openButtonClicked(); };

    addAndMakeVisible (playButton);
    playButton.setButtonText ("Play");
    playButton.onClick = [this] () { playButtonClicked(); };
    playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled (false);

    addAndMakeVisible (stopButton);
    stopButton.setButtonText ("Stop");
    stopButton.onClick = [this] () { stopButtonClicked(); };
    stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled (false);

    addAndMakeVisible (loopingToggle);
    loopingToggle.setButtonText ("Loop");
    loopingToggle.onClick = [this] () { loopButtonChanged(); };

    addAndMakeVisible (currentPositionLabel);
    currentPositionLabel.setText ("Stopped", juce::dontSendNotification);
    
    formatManager.registerBasicFormats();
}

Granular3DAudioProcessorEditor::~Granular3DAudioProcessorEditor()
{
}

//==============================================================================
void Granular3DAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(50.0f);
    g.drawFittedText ("Hello Philip!, Hi Yonghyun!", getLocalBounds(), juce::Justification::centred, 2);
}

void Granular3DAudioProcessorEditor::resized()
{
    openButton.setBounds (10, 10, getWidth() - 20, 20);
    playButton.setBounds (10, 40, getWidth() - 20, 20);
    stopButton.setBounds (10, 70, getWidth() - 20, 20);
    loopingToggle.setBounds (10, 100, getWidth() - 20, 20);
    currentPositionLabel.setBounds (10, 130, getWidth() - 20, 20);

}

//오픈버튼 시 chooser 인터페이스로 wav파일을 읽어서 juce:File 로 formatManager에 넣어주고, 그 포맷이 맞추어진 데이터로부터 reader를 생성해서
//audioProcessor의 setTransportSource() 펑션을 통해 프로세서 측으로 read정보가 넘어가서, 그곳에서 transportSource(juce의 AudioTransport 클래스로 만든 변수)에 들어가서 .setSource 됨.
void Granular3DAudioProcessorEditor::openButtonClicked()
{
    
    
    chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                   juce::File{},
                                                   "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode
                      | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        juce::File file = fc.getResult();
        
        if (file != juce::File{})
        {
            auto* reader = formatManager.createReaderFor (file);
            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                audioProcessor.transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                playButton.setEnabled (true);
                audioProcessor.readerSource.reset (newSource.release());
            }
        }
    });
    
}

void Granular3DAudioProcessorEditor::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
    {
        changeState (Starting);
        changeState (Playing);
    }
    else if (state == Playing)
    {
        changeState (Pausing);
        changeState (Paused);
    }
}

void Granular3DAudioProcessorEditor::stopButtonClicked()
{
    if (state == Paused)
        changeState (Stopped);
    else
        changeState (Stopping);
}

void Granular3DAudioProcessorEditor::timerCallback() // juce::Timer 안에 있는 timerCallback 펑션. 실시간 업뎃에 필요
{
    if (audioProcessor.transportSource.isPlaying())
    {
        juce::RelativeTime position (audioProcessor.transportSource.getCurrentPosition());

        auto minutes = ((int) position.inMinutes()) % 60;
        auto seconds = ((int) position.inSeconds()) % 60;
        auto millis  = ((int) position.inMilliseconds()) % 1000;

        auto positionString = juce::String::formatted ("%02d:%02d:%03d", minutes, seconds, millis);

        currentPositionLabel.setText (positionString, juce::dontSendNotification);
    }
    else
    {
        currentPositionLabel.setText ("Stopped", juce::dontSendNotification);
    }
}

void Granular3DAudioProcessorEditor::changeState (int newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
            case Stopped:
                playButton.setButtonText ("Play");
                stopButton.setButtonText ("Stop");
                stopButton.setEnabled (false);
                audioProcessor.transportSource.setPosition (0.0); // 음악의 시작부분으로 포인팅
                Granular3DAudioProcessor::myState = 0;
                break;

            case Starting:
                audioProcessor.transportSource.start();
                Granular3DAudioProcessor::myState = 1;
                break;

            case Playing:
                playButton.setButtonText ("Pause");
                stopButton.setButtonText ("Stop");
                stopButton.setEnabled (true);
                Granular3DAudioProcessor::myState = 2;
                break;
                
            case Pausing:
                audioProcessor.transportSource.stop();
                Granular3DAudioProcessor::myState = 3;
                break;

            case Paused:
                playButton.setButtonText ("Resume");
                stopButton.setButtonText ("Return to Zero");
                Granular3DAudioProcessor::myState = 4;
                break;

            case Stopping:
                audioProcessor.transportSource.stop();
                Granular3DAudioProcessor::myState = 5;
                break;
        }
    }
}
 

void Granular3DAudioProcessorEditor::updateLoopState (bool shouldLoop)
   {
       if (audioProcessor.readerSource.get() != nullptr)
           audioProcessor.readerSource->setLooping (shouldLoop);
   }


void Granular3DAudioProcessorEditor::loopButtonChanged()
   {
       updateLoopState (loopingToggle.getToggleState());
   }

void Granular3DAudioProcessorEditor::changeListenerCallback (juce::ChangeBroadcaster* source)
{
//    if (source == &audioProcessor.transportSource)
//    {
//        if (state == Starting)
//            changeState (Playing);
//        else if ((state == Stopping) || (state == Playing))
//            changeState (Stopped);
//        else if (Pausing == state)
//            changeState (Paused);
//    }
}
