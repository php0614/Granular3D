#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (openButton);
    openButton.setButtonText ("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible (clearButton);
    clearButton.setButtonText ("Clear");
    clearButton.onClick = [this] { clearButtonClicked(); };
    
    addAndMakeVisible (visualizeButton);
    visualizeButton.setButtonText ("Visualize");
    visualizeButton.onClick = [this] { visualizeButtonClicked(); };
    
    addAndMakeVisible (grainPositionSlider);
    grainPositionSlider.setRange (0, 80000, 1);
    grainPositionSlider.setValue(512);
    grainPositionSlider.setTitle("Grain Position");
    grainPositionSlider.onValueChange = [this] {newStartPosition = grainPositionSlider.getValue();};
    
    addAndMakeVisible (panningSlider);
    panningSlider.setRange (0, 1, 0.01);
    panningSlider.setValue(0.5);
    panningSlider.setTitle("Panning Position");
    panningSlider.onValueChange = [this] {pannings = panningSlider.getValue()+currentRand;
        repaint(); };
    
    
    addAndMakeVisible (grainLengthSlider);
    grainLengthSlider.setRange (10, 8192, 1);
    grainLengthSlider.setValue(1024);
    grainLengthSlider.setTitle("Grain Length");
    grainLengthSlider.onValueChange = [this] { newGrainSize = grainLengthSlider.getValue();
    };

    setSize (1024, 768);

    formatManager.registerBasicFormats();
    
    startPosition = 512;
    grainSize = 1024;
    windowPosition = 0;
    
    newStartPosition = startPosition;
    newGrainSize = grainSize;
    
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

        for (auto channel = 0; channel < numOutputChannels; ++channel)
        {
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
            
        //아웃풋 버퍼를 fileBuffer로 부터 일정 부분 가져와서 채움
            bufferToFill.buffer->copyFrom (channel,                                        
                                           outputSamplesOffset,
                                           fileBuffer,
                                           channel % numInputChannels,
                                           position,
                                           samplesThisTime);
            //std::cout<<position<<std::endl;
            //processBuffer를 아웃풋으로 만드는 부분(getWritePointer)
            auto processBuffer = bufferToFill.buffer->getWritePointer(channel);
            
            if(channel == 0)
            {
            // Write to Ring Buffer: Do it only when Channel 0
            ringBuffer->writeSamples (*bufferToFill.buffer, bufferToFill.startSample, bufferToFill.numSamples);
            }
            
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
                {
                    //windowPosition은 position(전체 버퍼 재생에 쓰이는 위치 정보)와 다름. position을 그대로 윈도우의 위치를 가져오는데 쓰면, 윈도우가 갑자기 중간에서 0으로 뛰어버리는 일이 일어나 오디오 퀄리티가 안좋아짐
                    //따라서 따로 windowPosition을 써서 별도로 진행되게 해야함
                        
                    //processBuffer가 곧 아웃풋
                    processBuffer[sample] *= generateHannWindow(windowSize, windowPosition);
                    
                     currentRand = abs(random.nextFloat()) * 0.1;
                    if(channel == 0)
                        processBuffer[sample] *= (1-(pannings * currentRand));
                    if(channel == 1)
                        processBuffer[sample] *= (pannings* currentRand);
                    
                    if(channel == 0)
                    {
                        //부드럽게 window가 0위치일때 새로운 값 입력을 반영
                        if(windowPosition == 0)
                        {
                            grainSize = newGrainSize;
                            startPosition = newStartPosition;
                            windowPosition = 1;
                            
                            //원래는 grainSize로 window크기를 만들지만, 재생해야할 버퍼가 grainSize보다 작으면, 윈도우 사이즈 길이로 만듬
                            if(bufferSamplesRemaining < grainSize)
                                windowSize = bufferSamplesRemaining;
                            else
                                windowSize = grainSize;
                        }
                        else
                        {
                            windowPosition = (windowPosition+1) %grainSize;
                        }
                    }
                }
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
    if (oscilloscope2D != nullptr)
           {
               oscilloscope2D->stop();
               removeChildComponent (oscilloscope2D);
               delete oscilloscope2D;
           }
    
    delete ringBuffer;
}

void MainComponent::resized()
{
    openButton .setBounds (10, 10, getWidth() - 20, 20);
    clearButton.setBounds (10, 50, getWidth() - 20, 20);
    visualizeButton.setBounds (10, 90, getWidth() - 20, 20);
    auto sliderLeft = 120;
    grainPositionSlider.setBounds (sliderLeft, 200, getWidth() - sliderLeft - 10, 20);
    grainLengthSlider.setBounds (sliderLeft, 300, getWidth() - sliderLeft - 10, 20);
    
    if (oscilloscope2D != nullptr)
        oscilloscope2D->setBounds (10, 400, getWidth() - 20, getHeight()-50);
    
    panningSlider.setBounds (10, 110, getWidth() - 20, 20);
}


void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Setup Ring Buffer of GLfloat's for the visualizer to use
            // Uses two channels
            
    ringBuffer = new RingBuffer<GLfloat> (2, samplesPerBlockExpected * 10);
    oscilloscope2D = new Oscilloscope2D (ringBuffer);
    addChildComponent (oscilloscope2D);

            
}



void MainComponent::paint (juce::Graphics& g)
{
    
    g.setColour (juce::Colours::sandybrown);
    float a = pannings;
    g.drawEllipse(getWidth() * a, getHeight() * 0.5, 30, 30, 3);
}

void MainComponent::update()
{
    
}

//==============================================================================



void MainComponent::openButtonClicked()
{
    shutdownAudio();

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

        std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file));

        if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;

            if (duration < 10)
            {
                fileBuffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
                reader->read (&fileBuffer,
                              0,
                              (int) reader->lengthInSamples,
                              0,
                              true,
                              true);
                position = startPosition;
                setAudioChannels (0, (int) reader->numChannels);
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
    if (oscilloscope2D != nullptr)
           {
               oscilloscope2D->stop();
               removeChildComponent (oscilloscope2D);
               delete oscilloscope2D;
           }
    
}

void MainComponent::visualizeButtonClicked()
{
    oscilloscope2D->start();

    oscilloscope2D->setVisible(true);
    
    if (oscilloscope2D != nullptr)
        oscilloscope2D->setBounds (10, 400, getWidth()/2, getHeight()/2);
}


double MainComponent::generateHannWindow(int size, int pos){
        return 0.5 * (1 - cos(2*juce::MathConstants<float>::pi*pos/size));
        
}
