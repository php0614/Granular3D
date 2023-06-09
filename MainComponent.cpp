#include "MainComponent.h"
#include "RubberBandStretcher.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    addAndMakeVisible (grainVisualizeButton); // grain 가시화 On/Off
    grainVisualizeButton.setButtonText ("VIS ON/OFF");
    grainVisualizeButton.onClick = [this] { grainVisualizeButtonClicked(); };
    
    addAndMakeVisible (windowTypeButton); // windowType 변경
    windowTypeButton.setButtonText ("Window Type:\n"+windowName(windowType));
    windowTypeButton.onClick = [this] { windowTypeButtonClicked(); };
    
    //첫번째 음원용
    addAndMakeVisible (openButton); // "Adds" a child component to this one, and also makes the child "visible" if it isn't already.
    openButton.setButtonText ("Open Source 1");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible (clearButton);
    clearButton.setButtonText ("Clear Source 1");
    clearButton.onClick = [this] { clearButtonClicked(); };
    // 1: buttonColourid, 2: buttonOnColourid, 3: textColourOffid, 4: textColourOnid
    
    addAndMakeVisible (visualizeButton);
    visualizeButton.setButtonText ("Visualize Source 1");
    visualizeButton.onClick = [this] { visualizeButtonClicked(); };
    
    addAndMakeVisible (masterVolumeSlider); // 마스터볼륨(게인) 슬라이더; Master Volume(Gain) Slider
    masterVolumeSlider.setRange (0, 100, 1);
    masterVolumeSlider.setValue(80);
    masterVolumeSlider.setTitle("Master Volume");
    masterVolumeSlider.onValueChange = [this] {newStartPosition = masterVolumeSlider.getValue();};
    masterVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (masterVolumeLabel);
    masterVolumeLabel.setText ("Master\nVolume", juce::dontSendNotification);
    masterVolumeLabel.attachToComponent (&masterVolumeSlider, true);
    masterVolumeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    masterVolumeSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    masterVolumeSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    masterVolumeSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    addAndMakeVisible (grainNumberSlider);
    grainNumberSlider.setRange (0, 10, 1);
    grainNumberSlider.setValue(1);
    grainNumberSlider.setTitle("Grain Number");
    grainNumberSlider.onValueChange = [this] {newStartPosition = grainNumberSlider.getValue();};
    addAndMakeVisible (grainNumberLabel);
    grainNumberLabel.setText ("Grain Num", juce::dontSendNotification);
    grainNumberLabel.attachToComponent (&grainNumberSlider, true);
    grainNumberLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    grainNumberSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    grainNumberSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    grainNumberSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    
    addAndMakeVisible (grainPositionSlider);
    grainPositionSlider.setRange (0, 34800, 1);
    grainPositionSlider.setValue(24745);
    grainPositionSlider.setTitle("Grain Position");
    grainPositionSlider.onValueChange = [this] {newStartPosition = grainPositionSlider.getValue();};
    addAndMakeVisible (grainPositionLabel);
    grainPositionLabel.setText ("Grain Pos", juce::dontSendNotification);
    grainPositionLabel.attachToComponent (&grainPositionSlider, true);
    grainPositionLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    grainPositionSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    grainPositionSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    grainPositionSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    addAndMakeVisible (grainLengthSlider);
    grainLengthSlider.setRange (10, 8192, 1);
    grainLengthSlider.setValue(5070);
    grainLengthSlider.setTitle("Grain Length");
    grainLengthSlider.onValueChange = [this] { newGrainSize = grainLengthSlider.getValue();};
    addAndMakeVisible (grainLengthLabel);
    grainLengthLabel.setText ("Grain Len", juce::dontSendNotification);
    grainLengthLabel.attachToComponent (&grainLengthSlider, true);
    grainLengthLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    grainLengthSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    grainLengthSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    grainLengthSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    addAndMakeVisible(grainFrequencySlider);
    grainFrequencySlider.setRange (0, 100, 1);
    grainFrequencySlider.setValue(100);
    grainFrequencySlider.setTitle("Grain Frequency");
    grainFrequencySlider.setTextValueSuffix(" %");
    grainFrequencySlider.onValueChange = [this] { newGrainNumber = grainFrequencySlider.getValue();};
    addAndMakeVisible (grainNumberLabel);
    grainFrequencyLabel.setText ("Grain Frq", juce::dontSendNotification);
    grainFrequencyLabel.attachToComponent (&grainFrequencySlider, true);
    grainFrequencyLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    grainFrequencySlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    grainFrequencySlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    grainFrequencySlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));

    addAndMakeVisible (panningRandomizeSlider);
    panningRandomizeSlider.setRange (0, 0.5, 0.01);
    panningRandomizeSlider.setValue(0);
    panningRandomizeSlider.setTitle("Panning Randomize");
    addAndMakeVisible (panningRandomizeLabel);
    panningRandomizeLabel.setText ("Pan Rand", juce::dontSendNotification);
    panningRandomizeLabel.attachToComponent (&panningRandomizeSlider, true);
    panningRandomizeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    panningRandomizeSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    panningRandomizeSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    panningRandomizeSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    addAndMakeVisible (xPanningSlider);
    xPanningSlider.setRange (0, 1, 0.01);
    xPanningSlider.setValue(0.5);
    xPanningSlider.setTitle("x Panning Position");
    addAndMakeVisible (xPanningLabel);
    xPanningLabel.setText ("x/y/z Pan", juce::dontSendNotification);
    xPanningLabel.attachToComponent (&xPanningSlider, true);
    xPanningLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    xPanningSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    xPanningSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    xPanningSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    addAndMakeVisible (yPanningSlider);
    yPanningSlider.setRange (0, 1, 0.01);
    yPanningSlider.setValue(0.5);
    yPanningSlider.setTitle("y Panning Position");
    addAndMakeVisible (yPanningLabel);
    //yPanningLabel.setText ("y", juce::dontSendNotification);
    //yPanningLabel.attachToComponent (&yPanningSlider, true);
    //yPanningLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    yPanningSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    yPanningSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    yPanningSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));
    
    addAndMakeVisible (zPanningSlider);
    zPanningSlider.setRange (0, 1, 0.01);
    zPanningSlider.setValue(0.5);
    zPanningSlider.setTitle("z Panning Position");
    addAndMakeVisible (zPanningLabel);
    //zPanningLabel.setText ("z", juce::dontSendNotification);
    //zPanningLabel.attachToComponent (&yPanningSlider, true);
    //zPanningLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    zPanningSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::grey);
    zPanningSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour::fromHSV(0.13f, 0.88f, 0.96f, 1.0f));
    zPanningSlider.setColour(juce::Slider::ColourIds::thumbColourId ,juce::Colour::fromHSV(0.13f, 0.88f, 0.90f, 1.0f));

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
    
    int grain_number = grainNumberSlider.getValue();
    
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
            
            //processBuffer를 아웃풋으로 만드는 부분(getWritePointer)
            float* processBuffer = bufferToFill.buffer->getWritePointer(channel);


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
                //processBuffer[sample] *= generateHannWindow(windowSize, windowPosition);
                switch (windowType)
                {
                    // Rectangular window
                    case 0: continue;
                    
                    // Hanning window (Hannwindow)
                    case 1: processBuffer[sample] *= generateHannWindow(windowSize, windowPosition);
                    
                    // Hamming window (Hammwindow)
                    case 2: processBuffer[sample] *= generateHammWindow(windowSize, windowPosition);
                    
                    // Blackman window
                    case 3: processBuffer[sample] *= generateBlackmanWindow(windowSize, windowPosition);
                    
                }
                
                // 다양한 Sound를 만들기 위해 시도하기 (1) 음원과 Knocking Sound가 함께 들림
//                if (iteration % 10 == 0)
//                {processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*iteration/40);}
                
                // 다양한 Sound를 만들기 위해 시도하기 (2) 깜빡이 (newGrainNumber가 커지면 빈도수가 높아짐)
                if (newGrainNumber == 0 || iteration % (100/newGrainNumber) != 0 ){processBuffer[sample] = 0;}
                
                // 다양한 Sound를 만들기 위해 시도하기 (3) 음원과 또다른 사운드가 함께 들림
//                if (iteration % 2 == 0)
//                {processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*sample/130);} // 이 130 값을 낮추면 음이 높아지고 값을 높이면 음이 낮아짐
                
                // 다양한 Sound를 만들기 위해 시도하기 (4) 음원과 또다른 사운드 2개까지 해서 3개가 번갈이 들림
//                if (iteration % 3 == 0)
//                {
//                    processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*sample/130); //
//
//                }
//                else if (iteration % 3 == 1)
//                {
//                    processBuffer[sample] = 0.1 * std::sin(2*juce::MathConstants<float>::pi*sample/100);
//                }
                
                // 다양한 Sound를 만들기 위해 시도하기 (5) 기존 음원을 사인파에 올리기 - FM Synthesis..!?
                if (grain_number == 0)
                    processBuffer[sample] *= 0;
                else if (iteration % grain_number != 0)
                {
                    processBuffer[sample] *= std::sin(2*juce::MathConstants<float>::pi*sample/(160 - (iteration % grain_number)*5));
                    // 이 160 값이 음정을 조절함
                }
                
                // 마스터 볼륨 조절
                processBuffer[sample] *= (0.01 * masterVolumeSlider.getValue());
                
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
                        currentGrainPan = xPannings;
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
    // Painting 방식을 화면의 비율로 모두 맞췄습니다. :)
    
    int width = getWidth();
    int height = getHeight();

    int buttonLayoutX   = int(width*0.01);
    int buttonLayoutY   = int(height*0.8);
    int buttonWidth     = int(width*0.25);
    int buttonHeight    = int(height*0.05);
    int buttonMargin    = int(height*0.065);
    
    int labelLayoutX    = int(width*0.26);
    int labelLayoutY    = buttonLayoutY;
    int labelWidth      = int(width*0.09);
    int labelHeight     = int(height*0.025);
    int labelMargin     = int(height*0.03);
    
    int sliderLayoutX   = int(width*0.35);
    int sliderLayoutY   = buttonLayoutY;
    int sliderWidth     = int(width*0.40);
    int sliderHeight    = labelHeight;
    int sliderMargin    = labelMargin;
    
    int oscil2DLayoutX  = int(width*0.77);
    int oscil2DLayoutY  = buttonLayoutY;
    int oscil2DWidth    = int(width*0.2);
    int oscil2DHeight   = int(height*0.15);
    
    int masterVolumeLabelLayoutX    = int(width*0.89);
    int masterVolumeLabelLayoutY    = int(height* 0.15);
    int masterVolumeLabelWidth      = int(width*0.10);
    int masterVolumeLabelHeight     = int(height*0.05);
    
    int masterVolumeSliderLayoutX   = int(width*0.89);
    int masterVolumeSliderLayoutY   = int(height* 0.2);
    int masterVolumeSliderWidth     = int(width*0.10);
    int masterVolumeSliderHeight    = int(height*0.20);

    // Grain Visualize 버튼
    grainVisualizeButton.setTopLeftPosition (buttonLayoutX, buttonLayoutY - buttonMargin);
    grainVisualizeButton.setSize (width*0.1, buttonHeight);
    
    // windowType 버튼
    windowTypeButton.setTopLeftPosition (masterVolumeSliderLayoutX, buttonLayoutY - buttonMargin*2);
    windowTypeButton.setSize (width*0.1, buttonHeight);
    
    // 첫번째 음원용 버튼
    openButton.setTopLeftPosition (buttonLayoutX,buttonLayoutY);
    openButton.setSize(buttonWidth, buttonHeight);
    clearButton.setTopLeftPosition (buttonLayoutX,buttonLayoutY + buttonMargin);
    clearButton.setSize(buttonWidth, buttonHeight);
    visualizeButton.setTopLeftPosition (buttonLayoutX,buttonLayoutY + buttonMargin * 2);
    visualizeButton.setSize(buttonWidth, buttonHeight);
   
    // 마스터 볼륨 슬라이더
    masterVolumeSlider.setTopLeftPosition(masterVolumeSliderLayoutX,masterVolumeSliderLayoutY);
    masterVolumeSlider.setSize(masterVolumeSliderWidth,masterVolumeSliderHeight);
    masterVolumeLabel.setTopLeftPosition(masterVolumeLabelLayoutX, masterVolumeLabelLayoutY);
    masterVolumeLabel.setSize(masterVolumeLabelWidth, masterVolumeLabelHeight);
    masterVolumeLabel.setJustificationType(juce::Justification::centred);
    
    // 첫번째 음원용 슬라이더
    grainNumberSlider.setTopLeftPosition(sliderLayoutX,sliderLayoutY);
    grainNumberSlider.setSize(sliderWidth,sliderHeight);
    grainNumberLabel.setTopLeftPosition(labelLayoutX, labelLayoutY);
    grainNumberLabel.setSize(labelWidth, labelHeight);
    grainNumberLabel.setJustificationType(juce::Justification::right);
    grainPositionSlider.setTopLeftPosition(sliderLayoutX,sliderLayoutY + sliderMargin);
    grainPositionSlider.setSize(sliderWidth,sliderHeight);
    grainPositionLabel.setTopLeftPosition(labelLayoutX, labelLayoutY + labelMargin);
    grainPositionLabel.setSize(labelWidth, labelHeight);
    grainPositionLabel.setJustificationType(juce::Justification::right);
    grainLengthSlider.setTopLeftPosition(sliderLayoutX,sliderLayoutY + sliderMargin*2);
    grainLengthSlider.setSize(sliderWidth,sliderHeight);
    grainLengthLabel.setTopLeftPosition(labelLayoutX, labelLayoutY + labelMargin*2);
    grainLengthLabel.setSize(labelWidth, labelHeight);
    grainLengthLabel.setJustificationType(juce::Justification::right);
    grainFrequencySlider.setTopLeftPosition(sliderLayoutX,sliderLayoutY + sliderMargin*3);
    grainFrequencySlider.setSize(sliderWidth,sliderHeight);
    grainFrequencyLabel.setTopLeftPosition(labelLayoutX, labelLayoutY + labelMargin*3);
    grainFrequencyLabel.setSize(labelWidth, labelHeight);
    grainFrequencyLabel.setJustificationType(juce::Justification::right);
    panningRandomizeSlider.setTopLeftPosition(sliderLayoutX, sliderLayoutY + sliderMargin*4);
    panningRandomizeSlider.setSize(sliderWidth,sliderHeight);
    panningRandomizeLabel.setTopLeftPosition(labelLayoutX, labelLayoutY + labelMargin*4);
    panningRandomizeLabel.setSize(labelWidth, labelHeight);
    panningRandomizeLabel.setJustificationType(juce::Justification::right);
    xPanningSlider.setTopLeftPosition(sliderLayoutX, sliderLayoutY + sliderMargin*5);
    xPanningSlider.setSize(sliderWidth*0.33,sliderHeight);
    xPanningLabel.setTopLeftPosition(labelLayoutX, labelLayoutY + labelMargin*5);
    xPanningLabel.setSize(labelWidth, labelHeight);
    xPanningLabel.setJustificationType(juce::Justification::right);
    yPanningSlider.setTopLeftPosition(sliderLayoutX+sliderWidth*0.33, sliderLayoutY + sliderMargin*5);
    yPanningSlider.setSize(sliderWidth*0.33,sliderHeight);
    zPanningSlider.setTopLeftPosition(sliderLayoutX+sliderWidth*0.66, sliderLayoutY + sliderMargin*5);
    zPanningSlider.setSize(sliderWidth*0.33,sliderHeight);
    
    if (oscilloscope2D != nullptr)
    {
        oscilloscope2D->setTopLeftPosition(oscil2DLayoutX, oscil2DLayoutY);
        oscilloscope2D->setSize(oscil2DWidth, oscil2DHeight);
    }
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
    
    g.fillAll(juce::Colour::fromHSV(0.13f, 0.02f, 0.99f, 1.0f)); // 배경화면
    
    float width = getWidth() * width_ratio;
    float height = getHeight() * height_ratio;
    float ratio = 0.05;
    
    // Revised 3D grid
    g.setColour(juce::Colours::black);
    g.drawLine(0, 0, 5*width*ratio, 5*height*ratio);
    
    for (int i=0; i<6; i++)
    {
        if (i==0)
            g.drawRect(0.0,0.0, width, height, 2.0);
        else
            g.drawRect(i*width*ratio,i*height*ratio, width*(1-2*i*ratio), height*(1-2*i*ratio), 1.0);
        if (i<5)
        {
            g.drawLine((i+1)*0.2*width, 0.0, (0.35+i*0.1)*width,0.25*height);
            g.drawLine(0.25*width,(0.35+i*0.1)*height, 0.75*width,(0.35+i*0.1)*height);
            g.drawLine((0.35+i*0.1)*width,0.25*height, (0.35+i*0.1)*width,0.75*height);
            g.drawLine((0.35+i*0.1)*width, 0.75*height, (i+1)*0.2*width,height);
            g.drawLine(0.0, (i+1)*0.2*height, 0.25*width, (0.35+i*0.1)*height);
            g.drawLine(0.75*width, (0.35+i*0.1)*height, width, (i+1)*0.2*height);
        }
    }
    
    //슬라이드바 배경
    g.setColour(juce::Colour::fromHSV(0.13f, 0.02f, 0.99f, 1.0f));
    g.juce::Graphics::fillRect((float)(getWidth() * (0.31)), (float)(getHeight() * (0.79)), float(getWidth() * (0.4)), float(getHeight() * (0.22)));
    g.drawImageWithin(coordinate, getWidth()*0.87, getHeight()* 0.45, getWidth()*0.14, getHeight()*0.14, 0);
    
    g.drawImageWithin(front_lr, width * 0.25, height * 0.51, width*0.15, height* 0.3, 0);
    g.drawImageWithin(front_m, width * 0.43, height * 0.55, width*0.15, height* 0.3, 0);
    g.drawImageWithin(front_lr, width * 0.61, height * 0.51, width*0.15, height* 0.3, 0);
    
    g.drawImageWithin(person, width * 0.425, height * 0.61, width*0.15, height* 0.3, 0);

    g.drawImageWithin(rear_l, width * 0.33, height * 0.71, width*0.15, height* 0.3, 0);
    g.drawImageWithin(rear_r, width * 0.53, height * 0.71, width*0.15, height* 0.3, 0);

    
    
    if (grainVisualize == true)
    {
        int num_ellipse = grainNumberSlider.getValue();
        
        xCurrentRand = (random.nextFloat() -0.5) * 2 * panningRandomizeSlider.getValue();
            
        for (int i=0; i<num_ellipse; i++)
        {
            xCurrentRand = (random.nextFloat() -0.5) * 2 * panningRandomizeSlider.getValue();
            yCurrentRand = (random.nextFloat() -0.5) * 2 * panningRandomizeSlider.getValue();
            zCurrentRand = (random.nextFloat() -0.5) * 2 * panningRandomizeSlider.getValue();

            xPannings = xPanningSlider.getValue()+xCurrentRand;
            yPannings = yPanningSlider.getValue()+yCurrentRand;
            zPannings = zPanningSlider.getValue()+zCurrentRand;
            g.drawImageWithin(sphere,-0.02*width*xPannings*(1-zPannings)+(width*(xPannings+0.16)*0.74)-0.5*width*(zPannings-0.5)*(xPannings-0.5), height*(yPannings+0.12)*0.75, width*0.05*(1-zPannings*0.5), height*0.05*(1-zPannings*0.5), 0);
            g.drawImageWithin(grain_sketch, getWidth()*0.91+0.04*getWidth()*(xPannings+0.5*zPannings), getHeight()* 0.53-0.023*getHeight()*(zPannings)+0.06*getHeight()*(yPannings-0.5), getWidth()*0.01, getHeight()*0.01, 0);
        }


    }
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
    int width = getWidth();
    int height = getHeight();
    int oscil2DLayoutX  = int(width*0.77);
    int oscil2DLayoutY  = int(height*0.8);
    int oscil2DWidth    = int(width*0.2);
    int oscil2DHeight   = int(height*0.18);
    
    oscilloscope2D->start();

    oscilloscope2D->setVisible(true);
    
    if (oscilloscope2D != nullptr)
    {
        oscilloscope2D->setTopLeftPosition(oscil2DLayoutX, oscil2DLayoutY);
        oscilloscope2D->setSize(oscil2DWidth, oscil2DHeight);
    }
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


// Applying Different Window functions
// 0 <= pos <= size-1
double MainComponent::generateHannWindow(int size, int pos)
{
        return 0.5 * (1 - cos(2*juce::MathConstants<float>::pi*pos/size));
}

double MainComponent::generateHammWindow(int size, int pos)
{
        return 0.54 - 0.46 * cos(2*juce::MathConstants<float>::pi*pos/size);
}

double MainComponent::generateBlackmanWindow(int size, int pos)
{
        return 0.42 -  0.5 * cos(2*juce::MathConstants<float>::pi*pos/size) + 0.08 * cos(4*juce::MathConstants<float>::pi*pos/size);
}


void MainComponent::timerCallback()
{
    repaint();
}
