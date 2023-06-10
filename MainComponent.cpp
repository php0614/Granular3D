#include "MainComponent.h"
#include "RubberBandStretcher.h"

//==============================================================================
MainComponent::MainComponent()
{
    //첫번째 음원용
    addAndMakeVisible (openButton); // "Adds" a child component to this one, and also makes the child "visible" if it isn't already.
    openButton.setButtonText ("Open Source 1");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible (clearButton);
    clearButton.setButtonText ("Clear Source 1");
    clearButton.onClick = [this] { clearButtonClicked(); };
    
    addAndMakeVisible (visualizeButton);
    visualizeButton.setButtonText ("Visualize Source 1");
    visualizeButton.onClick = [this] { visualizeButtonClicked(); };
    
    addAndMakeVisible (grainPositionSlider);
    grainPositionSlider.setRange (0, 34800, 1);
    grainPositionSlider.setValue(24745);
    grainPositionSlider.setTitle("Grain Position");
    grainPositionSlider.onValueChange = [this] {newStartPosition = grainPositionSlider.getValue();};
    
    addAndMakeVisible (grainLengthSlider);
    grainLengthSlider.setRange (10, 8192, 1);
    grainLengthSlider.setValue(5070);
    grainLengthSlider.setTitle("Grain Length");
    grainLengthSlider.onValueChange = [this] { newGrainSize = grainLengthSlider.getValue();};
    
    addAndMakeVisible(grainNumberSlider);
    grainNumberSlider.setRange (0, 512, 1);
    grainNumberSlider.setValue(1);
    grainNumberSlider.setTitle("Grain Number");
    grainNumberSlider.onValueChange = [this] { newGrainNumber = grainNumberSlider.getValue();};

    addAndMakeVisible (panningRandomizeSlider);
    panningRandomizeSlider.setRange (0, 0.5, 0.01);
    panningRandomizeSlider.setValue(0);
    panningRandomizeSlider.setTitle("Panning Randomize");
    
    addAndMakeVisible (panningSlider);
    panningSlider.setRange (0, 1, 0.01);
    panningSlider.setValue(0.5);
    panningSlider.setTitle("Panning Position");
    //panningSlider.onValueChange = [this] {pannings = panningSlider.getValue();};

    setSize (1200, 1200);

    formatManager.registerBasicFormats();
    // formatManager: A class for keeping a list of available audio formats, and for deciding which one to use to open a given file. After creating an AudioFormatManager object, you should call registerFormat() or registerBasicFormats() to give it a list of format types that it can use.
    
    startPosition = 512;
    grainSize = 1024;
    windowPosition = 0;
    
    newStartPosition = startPosition;
    newGrainSize = grainSize;
    newGrainNumber = grainNumber;
    
    startTimer(33);
    
    // 두번째 음원용
    addAndMakeVisible (openButton2);
    openButton2.setButtonText ("Open Source 2");
    openButton2.onClick = [this] { openButtonClicked2(); };

    addAndMakeVisible (clearButton2);
    clearButton2.setButtonText ("Clear Source 2");
    clearButton2.onClick = [this] { clearButtonClicked2(); };
    
    addAndMakeVisible (visualizeButton2);
    visualizeButton2.setButtonText ("Visualize Source 2");
    visualizeButton2.onClick = [this] { visualizeButtonClicked2(); };
    
    addAndMakeVisible (grainPositionSlider2);
    grainPositionSlider2.setRange (0, 34800, 1);
    grainPositionSlider2.setValue(24745);
    grainPositionSlider2.setTitle("Grain Position2");
    grainPositionSlider2.onValueChange = [this] {newStartPosition2 = grainPositionSlider2.getValue();};
    
    addAndMakeVisible (grainLengthSlider2);
    grainLengthSlider2.setRange (10, 8192, 1);
    grainLengthSlider2.setValue(5070);
    grainLengthSlider2.setTitle("Grain Length2");
    grainLengthSlider2.onValueChange = [this] { newGrainSize2 = grainLengthSlider2.getValue();};
    
    addAndMakeVisible(grainNumberSlider2);
    grainNumberSlider2.setRange (0, 512, 1);
    grainNumberSlider2.setValue(1);
    grainNumberSlider2.setTitle("Grain Number2");
    grainNumberSlider2.onValueChange = [this] { newGrainNumber2 = grainNumberSlider2.getValue();};

    addAndMakeVisible (panningRandomizeSlider2);
    panningRandomizeSlider2.setRange (0, 0.5, 0.01);
    panningRandomizeSlider2.setValue(0);
    panningRandomizeSlider2.setTitle("Panning Randomize2");
    
    addAndMakeVisible (panningSlider2);
    panningSlider2.setRange (0, 1, 0.01);
    panningSlider2.setValue(0.5);
    panningSlider2.setTitle("Panning Position2");
    //panningSlider.onValueChange = [this] {pannings = panningSlider.getValue();};
    
    ////////////////////////
    juce::File pathToMyExecutable = juce::File::getSpecialLocation (juce::File::SpecialLocationType::currentExecutableFile);
    auto Newpath = pathToMyExecutable.getParentDirectory().getParentDirectory().getFullPathName();
    juce::File initFile(Newpath+"/"+"ASMR_Voice.wav");
    juce::File initFile2(Newpath+"/"+"ASMR_Voice.wav");
    std::cout<<Newpath<<std::endl;

    
    std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (initFile)); //첫번째 음원
    std::unique_ptr<juce::AudioFormatReader> reader2 (formatManager.createReaderFor (initFile2)); //두번쨰 음원

    if (reader.get() != nullptr) // 첫번째 음원이 존재한다면
    {
        // 해당 음원 파일의 길이를 가져옵니다.
        auto duration = (float) reader->lengthInSamples / reader->sampleRate;

        if (duration < 10) // 만약 음원의 길이가 10초 미만이면
        {
            //각 음원 파일에 대한 버퍼를 생성합니다.
            fileBuffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
            
            reader->read (&fileBuffer,
                          0,
                          (int) reader->lengthInSamples,
                          0,
                          true,
                          true);

            
            // 첫번째 음원 파일에 대한 처리
            position = startPosition; // 음원파일에 대한 시작위치
            setAudioChannels (0, (int) reader->numChannels); // 음원 파일에 대한 오디오 채널을 설정
        }
        else
        {
            // handle the error that the file is 10 seconds or longer..
        }
    }
    if (reader2.get() != nullptr) // 두번째 음원이 존재한다면
    {
        // 각 음원 파일의 길이를 가져옵니다.
        auto duration2 = (float) reader2->lengthInSamples / reader2->sampleRate;

        if (duration2 < 10)
        {
            //각 음원 파일에 대한 버퍼를 생성합니다.
            fileBuffer2.setSize ((int) reader2->numChannels, (int) reader2->lengthInSamples);
            
            reader2->read (&fileBuffer2,
                          0,
                          (int) reader2->lengthInSamples, // 350497 (yonghyun's violin.wav)
                          0,
                          true,
                          true);

            
            // 첫번째 음원 파일에 대한 처리
            position2 = startPosition2; // 음원파일에 대한 시작위치
            setAudioChannels (1, (int) reader2->numChannels); // 음원 파일에 대한 오디오 채널을 설정
        }
        else
        {
            // handle the error that the file is 10 seconds or longer..
        }
    }
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    stopTimer();
}

//==============================================================================
// the appropriate number of samples is read from our fileBuffer AudioSampleBuffer member and written out the AudioSampleBuffer object in the AudioSourceChannelInfo struct.
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    iteration ++;
    
    auto numInputChannels = bufferToFill.buffer -> getNumChannels();
    auto numOutputChannels = bufferToFill.buffer -> getNumChannels(); // 2
    
    // stores the total number of samples
    auto outputSamplesRemaining = bufferToFill.numSamples;                                  // [8]
    //std::cout << "bufferToFill.numSamples: " << bufferToFill.numSamples; // 512
    
    auto outputSamplesOffset = bufferToFill.startSample;                                    // [9]
    //std::cout << "bufferToFill.startSample: " << bufferToFill.startSample; Always 0
    
    while (outputSamplesRemaining > 0)
    {
        auto bufferSamplesRemaining = fileBuffer.getNumSamples() - position; // [10] calculate how many samples are left in the buffer from which we are reading.
        
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
            
            //  For each output channel we use the AudioSampleBuffer::copyFrom() function to copy sections of data from one channel of one buffer to a channel of another buffer. 아웃풋 버퍼를 fileBuffer로 부터 일정 부분 가져와서 채움
            bufferToFill.buffer->copyFrom (channel, // Destination Channel index
                                           outputSamplesOffset, // Sample offset within the destination buffer.
                                           fileBuffer, // Source AudioSampleBuffer object from which to copy.
                                           channel % numInputChannels, // Channel index of the source buffer
                                           position, // position to start reading from in the source buffer.
                                           samplesThisTime); // The number of samples to read that we calculated earlier.
            
            //std::cout<<position<<std::endl;
            //processBuffer를 아웃풋으로 만드는 부분(getWritePointer)
            float* processBuffer = bufferToFill.buffer->getWritePointer(channel);
            //  "processBuffer" is a juce::AudioBuffer object containing the audio samples you want to apply the window to, "channel" is the index of the audio channel you want to process (0 for the first channel).
            
            
//            auto rb = new RubberBand::RubberBandStretcher(16000, 2);
//            rb -> setMaxProcessSize(512);
//            rb->process((const float* const*)fileBuffer.getArrayOfReadPointers(), fileBuffer.getNumSamples(), false);
//            rb->retrieve((float *const *)fileBuffer.getArrayOfWritePointers(), fileBuffer.getNumSamples());

            if(channel == 0)
            {
                // Write to Ring Buffer: Do it only when Channel 0
                ringBuffer->writeSamples (*bufferToFill.buffer, bufferToFill.startSample, bufferToFill.numSamples);
            }
            
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                //windowPosition은 position(전체 버퍼 재생에 쓰이는 위치 정보)와 다름. position을 그대로 윈도우의 위치를 가져오는데 쓰면, 윈도우가 갑자기 중간에서 0으로 뛰어버리는 일이 일어나 오디오 퀄리티가 안좋아짐
                //따라서 따로 windowPosition을 써서 별도로 진행되게 해야함
                
                //processBuffer가 곧 아웃풋 (1 sample)
                processBuffer[sample] *= generateHannWindow(windowSize, windowPosition);
            
                // 다양한 Sound를 만들기 위해 시도하기 (1) 음원과 Knocking Sound가 함께 들림
//                if (iteration % 10 == 0)
//                {processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*iteration/40);}
                
                // 다양한 Sound를 만들기 위해 시도하기 (2) 깜빡이 (gap을 키우면 Grain 출현 빈도를 늦추기)
                //if (gap>0 && iteration % gap != 0 ){processBuffer[sample] = 0;}
                
                // 다양한 Sound를 만들기 위해 시도하기 (3) 음원과 또다른 사운드가 함께 들림
//                if (iteration % 2 == 0)
//                {processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*sample/130);} // 이 130 값을 낮추면 음이 높아지고 값을 높이면 음이 낮아짐
                
                // 다양한 Sound를 만들기 위해 시도하기 (4) 음원과 또다른 사운드 2개까지 해서 3개가 번갈이 들림
                if (iteration % 3 == 0)
                {
                    processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*sample/130); //
                    
                }
                else if (iteration % 3 == 1)
                {
                    processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*sample/100);
                }
                
                // 스테레오 패닝 (왼쪽 오른쪽의 비율 조정)
                if(channel == 0) // 왼쪽
                    processBuffer[sample] *= (1-currentGrainPan);
                if(channel == 1) // 오른쪽
                    processBuffer[sample] *= currentGrainPan;
                
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
                        
                        //grain panning 업데이트 - 하나의 grain은 각자 하나의 고정된 panning값을 지님. grain하나가 재생될 동안에는 panning position은 바뀌지 않음.
                        currentGrainPan = pannings;
                    }
                    else
                        windowPosition = (windowPosition+1) % grainSize;
                }
            }
            
// Another way to Multiply the Windowing Function
//            juce::dsp::WindowingFunction<float> window(samplesThisTime, juce::dsp::WindowingFunction<float>::hamming);
//            window.multiplyWithWindowingTable(processBuffer, samplesThisTime);
            
        }
            
        
        outputSamplesRemaining -= samplesThisTime; // deduct the number of samples we just processed from the outputSamplesRemaining variable.
        outputSamplesOffset += samplesThisTime; // Increment the outputSamplesOffset by the same amount in case we have another pass of the while() loop.
        
        //포지션이 그레인의 사이즈+시작위치를 넘어서면, 버퍼를 비우고 position을 초기(startPosition)으로 돌리며, WindowPosition또한 0으로 돌림
        //그렇지 않으면 position을 앞으로 진전시킴(다음 블록을 위한 position 계산후 변수 저장
        if (position >= (grainSize+startPosition))
        {
            bufferToFill.buffer->clear();
            position = startPosition;
            windowPosition = 0;
        }
        else
            position = position+samplesThisTime; // Offset our position member
    }
 
}

void MainComponent::releaseResources()
{
    fileBuffer.setSize (0, 0);
    //fileBuffer2.setSize (0, 0);
    
    if (oscilloscope2D != nullptr)
    {
       oscilloscope2D->stop();
       removeChildComponent (oscilloscope2D);
       delete oscilloscope2D;
    }
    
//    if (oscilloscope2D2 != nullptr)
//    {
//       oscilloscope2D2->stop();
//       removeChildComponent (oscilloscope2D2);
//       delete oscilloscope2D2;
//    }
    
    delete ringBuffer;
//    delete ringBuffer2;
}

void MainComponent::resized()
{
    int margin = 30;
    int leftMargin = 10;
    int rightMargin = int((getWidth()-20)/2) + leftMargin;
    
    // 첫번째 음원용 버튼
    openButton.setBounds (leftMargin, 10, int((getWidth() - 20)/2), 20);
    clearButton.setBounds (leftMargin, margin+10, int((getWidth() - 20)/2), 20);
    visualizeButton.setBounds (leftMargin, margin*2+10, int((getWidth() - 20)/2), 20);
   
    // 첫번째 음원용 슬라이더
    grainPositionSlider.setBounds (leftMargin, margin*3+10, int((getWidth() - 20)/2), 20);
    grainLengthSlider.setBounds (leftMargin, margin*4+10, int((getWidth() - 20)/2), 20);
    grainNumberSlider.setBounds (leftMargin, margin*5+10, int((getWidth() - 20)/2), 20);
    panningRandomizeSlider.setBounds (leftMargin, margin*6+10, int((getWidth() - 20)/2), 20);
    panningSlider.setBounds (leftMargin, margin*7+10, int((getWidth() - 20)/2), 20);
    
    if (oscilloscope2D != nullptr)
        oscilloscope2D->setBounds (10, 350, getWidth() - 20, getHeight()-50);
    
    if (oscilloscope2D2 != nullptr)
        oscilloscope2D2->setBounds (10, 450, getWidth() - 20, getHeight()-50);
    
    // 두번째 음원용 버튼
    openButton2.setBounds (rightMargin, 10, int((getWidth() - 20)/2), 20);
    clearButton2.setBounds (rightMargin, margin+10, int((getWidth() - 20)/2), 20);
    visualizeButton2.setBounds (rightMargin, margin*2+10, int((getWidth() - 20)/2), 20);
    
    // 두번째 음원용 슬라이더
    grainPositionSlider2.setBounds (rightMargin, margin*3+10, int((getWidth() - 20)/2), 20);
    grainLengthSlider2.setBounds (rightMargin, margin*4+10, int((getWidth() - 20)/2), 20);
    grainNumberSlider2.setBounds (rightMargin, margin*5+10, int((getWidth() - 20)/2), 20);
    panningRandomizeSlider2.setBounds (rightMargin, margin*6+10, int((getWidth() - 20)/2), 20);
    panningSlider2.setBounds (rightMargin, margin*7+10, int((getWidth() - 20)/2), 20);
 
}


void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Setup Ring Buffer of GLfloat's for the visualizer to use
    // Uses two channels
    ringBuffer = new RingBuffer<GLfloat> (2, samplesPerBlockExpected * 10); // numChannels = 2, bufferSize = samplesPerBlockExpected * 10
    oscilloscope2D = new Oscilloscope2D (ringBuffer);
    addChildComponent (oscilloscope2D);
    
    ringBuffer2 = new RingBuffer<GLfloat> (2, samplesPerBlockExpected * 10); // numChannels = 2, bufferSize = samplesPerBlockExpected *
    oscilloscope2D2 = new Oscilloscope2D (ringBuffer2);
    addChildComponent (oscilloscope2D2);
}



void MainComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::sandybrown);
    
    float a = pannings;
    float b = pannings2;
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::orange);
    g.fillEllipse(getWidth() * a, getHeight() * 0.40, 40, 40);
    
    g.setColour(juce::Colours::green);
    g.fillEllipse(getWidth() * b, getHeight() * 0.45, 40, 40);
}

void MainComponent::mouseMove (const juce::MouseEvent& event)
{
//    mouseX = event.position.getX() /getWidth();
//    panningSlider.setValue(mouseX);
//    mouseY = event.position.getY() /getHeight() * 8000;
//    grainPositionSlider.setValue(mouseY);
}

//==============================================================================
//첫번째 음원용 - The whole file is then read into an AudioSampleBuffer member 'fileBuffer' in our MainContentComponent class.
void MainComponent::openButtonClicked()
{
    shutdownAudio(); // To avoid some of the multithreading issues.

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

        // We create the AudioFormatReader object using the AudioFormatManager object. Notice that we store this in a std::unique_ptr object as we need to manage this object ourselves.
        std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file));

        if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;

            if (duration < 10)
            {
                fileBuffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples); // we resize the AudioSampleBuffer object by calling the AudioSampleBuffer::setSize() function using the number of channels and length from the AudioFormatReader object.
                
                // This reads the audio data from the AudioFormatReader object into our AudioSampleBuffer fileBuffer member using the AudioFormatReader::read() function.
                reader->read (&fileBuffer,
                              0, // The destination start sample in the AudioSampleBuffer object where the data will start to be written.
                              (int) reader->lengthInSamples, // The number of samples to read.
                              0, // The start samples in the AudioFormatReader object where reading will start.
                              true, // For stereo (or other two-channel) files this flag indicates whether to read the "left" channel.
                              true); // For stereo files this flag indicates whether to read the "right" channel.
                
                position = startPosition; // We need to store the most recent read position within our buffer as we play it. (1024)
                setAudioChannels (0, (int) reader->numChannels); // This starts the audio system back up again. Here we have an opportunity to use the number of channels in the sound file to try and configure our audio device with the same number of channels.
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

// 두번째 음원용
void MainComponent::openButtonClicked2()
{
    shutdownAudio();

    chooser2 = std::make_unique<juce::FileChooser> ("Select a Wave file shorter than 2 seconds to play...",
                                                   juce::File{},
                                                   "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode
                      | juce::FileBrowserComponent::canSelectFiles;

    chooser2->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();

        if (file == juce::File{})
            return;

        std::unique_ptr<juce::AudioFormatReader> reader2 (formatManager.createReaderFor (file));

        if (reader2.get() != nullptr)
        {
            auto duration = (float) reader2->lengthInSamples / reader2->sampleRate;
            std::cout << "reader2->lengthInSamples: " << reader2->lengthInSamples; // 360497
            std::cout << "reader2->sampleRate: " << reader2->sampleRate; // 44100
            std::cout << "reader2->numChannels: " << reader2->numChannels << std::endl; // 2
            
            
            if (duration < 10)
            {
                fileBuffer2.setSize ((int) reader2->numChannels, (int) reader2->lengthInSamples);
                reader2->read(&fileBuffer2,
                              0,
                              (int) reader2->lengthInSamples,
                              0,
                              true,
                              true);
                position2 = startPosition2;
                setAudioChannels (0, (int) reader2->numChannels);
            }
            else
            {
                // handle the error that the file is 2 seconds or longer..
            }
        }
    });

}

void MainComponent::clearButtonClicked2()
{
//    shutdownAudio();
//
//    if (oscilloscope2D2 != nullptr)
//    {
//       oscilloscope2D2->stop();
//       removeChildComponent (oscilloscope2D2);
//       delete oscilloscope2D2;
//    }
    
}

void MainComponent::visualizeButtonClicked2()
{
//    oscilloscope2D2->start();
//
//    oscilloscope2D2->setVisible(true);
//
//    if (oscilloscope2D2 != nullptr)
//        oscilloscope2D2->setBounds (10, 400, getWidth()/2, getHeight()/2);
}

double MainComponent::generateHannWindow(int size, int pos)
{
        return 0.5 * (1 - cos(2*juce::MathConstants<float>::pi*pos/size));
}


void MainComponent::timerCallback()
{
    currentRand = (random.nextFloat() -0.5) * 2 * panningRandomizeSlider.getValue();
    pannings = panningSlider.getValue()+currentRand;
    
    currentRand2 = (random.nextFloat() -0.5) * 2 * panningRandomizeSlider2.getValue();
    pannings2 = panningSlider2.getValue()+currentRand2;
    repaint();
}
