#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : theWindow(0, juce::dsp::WindowingFunction<float>::hann)
{
    addAndMakeVisible (openButton);
    openButton.setButtonText ("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible (clearButton);
    clearButton.setButtonText ("Clear");
    clearButton.onClick = [this] { clearButtonClicked(); };
    
    addAndMakeVisible (grainPositionSlider);
    grainPositionSlider.setRange (0, 80000, 1);
    grainPositionSlider.setValue(512);
    grainPositionSlider.setTitle("Grain Position");
    grainPositionSlider.onValueChange = [this] {startPosition = grainPositionSlider.getValue(); };
    
    
    addAndMakeVisible (grainLengthSlider);
    grainLengthSlider.setRange (10, 8192, 1);
    grainLengthSlider.setValue(1024);
    grainLengthSlider.setTitle("Grain Length");
    grainLengthSlider.onValueChange = [this] { grainSize = grainLengthSlider.getValue();
    };

    setSize (640, 480);

    formatManager.registerBasicFormats();
    
    startPosition = 512;
    grainSize = 1024;
    windowPosition = 0;
    
    windowArray = new float[grainSize];
    
    theWindow.fillWindowingTables(windowArray, grainSize, juce::dsp::WindowingFunction<float>::hann);
    
    

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto numInputChannels = fileBuffer.getNumChannels();
    auto numOutputChannels = bufferToFill.buffer->getNumChannels();

    //아웃풋 버퍼의 크기를 outputSamplesRemaining에 담음
    auto outputSamplesRemaining = bufferToFill.numSamples;                                  // [8]
    auto outputSamplesOffset = bufferToFill.startSample;                                    // [9]

    //position = startPosition;
    
    while (outputSamplesRemaining > 0)
    {
        auto bufferSamplesRemaining = fileBuffer.getNumSamples() - position;                // [10]
        
        //아웃풋 버퍼(일정한 사이즈) 와 플레이 버퍼의 남은 길이(일정하지 않음. 예를 들면 버퍼가 2초짜리라면 맨 처음엔 2초 분량의 버퍼가 남지만, 끝으로 갈수록 남은-플레이해야할-버퍼가 줄어듬. 마지막엔 아웃풋 버퍼의 크기에 딱 떨어지지 않을 수 있음) 를 비교해서 작은 것을 아웃풋
        //예를 들면 아웃풋 버퍼가 100이고 플레이 해야할 샘플이 22567이라면, 맨 끝에는 67샘플이 남을 것임.
        auto samplesThisTime = juce::jmin (outputSamplesRemaining, bufferSamplesRemaining); // [11]

//        for (auto channel = 0; channel < numOutputChannels; ++channel)
//        {
            /*
             void AudioBuffer< Type >::copyFrom    (
                 int     destChannel,
                 int     destStartSample,
                 const   AudioBuffer< Type > &     source,
                 int     sourceChannel,
                 int     sourceStartSample,
                 int     numSamples
             )
             */
        
        int channel = 0;
            
        //아웃풋 버퍼를 fileBuffer로 부터 일정 부분 가져와서 채움
            bufferToFill.buffer->copyFrom (channel,                                        
                                           outputSamplesOffset,
                                           fileBuffer,
                                           channel % numInputChannels,
                                           position,
                                           samplesThisTime);
            //std::cout<<position<<std::endl;
            
            auto processBuffer = bufferToFill.buffer->getWritePointer(channel);
      


            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
                {
                    //windowPosition은 position(전체 버퍼 재생에 쓰이는 위치 정보)와 다름. position을 그대로 윈도우의 위치를 가져오는데 쓰면, 윈도우가 갑자기 중간에서 0으로 뛰어버리는 일이 일어나 오디오 퀄리티가 안좋아짐
                    //따라서 따로 windowPosition을 써서 별도로 진행되게 해야함
                    processBuffer[sample] *= generateHannWindow(grainSize, windowPosition);
                    windowPosition = (windowPosition+1) %grainSize;
                }


        outputSamplesRemaining -= samplesThisTime;
        outputSamplesOffset += samplesThisTime;
   
        //포지션이 그레인의 사이즈+시작위치를 넘어서면, 버퍼를 비우고 position을 초기(startPosition)으로 돌리며, WindowPosition또한 0으로 돌림
        //그렇지 않으면 position을 앞으로 진전시킴(다음 블록을 위한 position 계산후 변수 저장
        if (position >= (grainSize+startPosition))
        {
            bufferToFill.buffer->clear();
            position = startPosition;
            windowPosition = 0;
        }else{
            position = position+samplesThisTime;
        }
        
      
    }
}



void MainComponent::releaseResources()
{
    fileBuffer.setSize (0, 0);
}

void MainComponent::resized()
{
    openButton .setBounds (10, 10, getWidth() - 20, 20);
    clearButton.setBounds (10, 40, getWidth() - 20, 20);
    auto sliderLeft = 120;
    grainPositionSlider.setBounds (sliderLeft, 200, getWidth() - sliderLeft - 10, 20);
    grainLengthSlider.setBounds (sliderLeft, 300, getWidth() - sliderLeft - 10, 20);
}


void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}



void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}


//==============================================================================



void MainComponent::openButtonClicked()
{
    shutdownAudio();                                                                            // [1]

    chooser = std::make_unique<juce::FileChooser> ("Select a Wave file shorter than 2 seconds to play...",
                                                   juce::File{},
                                                   "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode
                      | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();

        if (file == juce::File{})
            return;

        std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file)); // [2]

        if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;               // [3]

            if (duration < 10)
            {
                fileBuffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);  // [4]
                reader->read (&fileBuffer,                                                      // [5]
                              0,                                                                //  [5.1]
                              (int) reader->lengthInSamples,                                    //  [5.2]
                              0,                                                                //  [5.3]
                              true,                                                             //  [5.4]
                              true);                                                            //  [5.5]
                position = startPosition;                                                                   // [6]
                setAudioChannels (0, (int) reader->numChannels);                                // [7]
            }
            else
            {
                // handle the error that the file is 2 seconds or longer..
            }
        }
    });
}

void MainComponent::clearButtonClicked()
{
    shutdownAudio();
}

float MainComponent::generateHannWindow(int size, int pos){
        return 0.5 * (1 - cos(2*juce::MathConstants<float>::pi*pos/size));
        
}
