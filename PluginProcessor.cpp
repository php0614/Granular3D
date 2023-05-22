/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Granular3DAudioProcessor::Granular3DAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    transportSource.addChangeListener (this);
}

Granular3DAudioProcessor::~Granular3DAudioProcessor()
{
}

//==============================================================================
const juce::String Granular3DAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Granular3DAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Granular3DAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Granular3DAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Granular3DAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Granular3DAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Granular3DAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Granular3DAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Granular3DAudioProcessor::getProgramName (int index)
{
    return {};
}

void Granular3DAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Granular3DAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void Granular3DAudioProcessor::releaseResources()
{
    transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Granular3DAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//buferToFill이라는 이름의 AudioSourceChannelInfo는 standalone프로그램 일 경우 흔히 쓰이는 변수명임
//플러그인의 processBlock에서는 그것이 기본 제공되지 않으며 아래를 통해 똑같은 것을 생성해 줄 수 있음.
//여기서 이것을 만드는 이유는 아래 transportSource가 AudioSourceChannelInfo를 입력형으로 받기 때문
juce::AudioSourceChannelInfo bufferToFill;

void Granular3DAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //bufferToFill의 정보를 채우는 과정
    bufferToFill.buffer      = &buffer;
    bufferToFill.startSample = 0;
    bufferToFill.numSamples  = buffer.getNumSamples();
    //////

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //transportSource(juce의 AudioTransport 클래스로 만든 변수)는 프로세서에서 processBlock()을 통해 최종적으로 버퍼를 채워 소리가 플레이 되게 만듬
    transportSource.getNextAudioBlock(bufferToFill);
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        
        // ..do something to the data...
    }
}

//==============================================================================
bool Granular3DAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Granular3DAudioProcessor::createEditor()
{
    return new Granular3DAudioProcessorEditor (*this);
}

//==============================================================================
void Granular3DAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Granular3DAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void Granular3DAudioProcessor::audioFileLoader(juce::File& file){
    
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Granular3DAudioProcessor();
}



//일단 현재는 미사용
void Granular3DAudioProcessor::changeListenerCallback (juce::ChangeBroadcaster* source)
    {
        if (source == &transportSource)
        {
           // if (transportSource.isPlaying())
                //changeState (Playing);
            //else
               // changeState (Stopped);
        }
    }

//에디터로부터 트리거 되는 펑션이며, 최종적으로 transportSource(juce의 AudioTransport 클래스로 만든 변수)에 오디오파일을 로드함
void Granular3DAudioProcessor::setTransportSource(juce::AudioFormatReader* reader){
    transportSource.stop();
    transportSource.setSource(nullptr);
    
    auto currentAudioFileSource = new juce::AudioFormatReaderSource(reader, true);
    
    transportSource.setSource(
        currentAudioFileSource,
        0,                   // tells it to buffer this many samples ahead
        nullptr,        // this is the background thread to use for reading-ahead
        reader->sampleRate);     // allows for sample rate correction
    
};

