/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SwampVSTAudioProcessor::SwampVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: currentAngle(0), angleDelta(0), AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                                                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                                                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                                                  ), tree(*this, nullptr) 
#endif
{
    //setting the range for the filter cutoff, resonance and the drop-down selective menu
    NormalisableRange<float> cutoffRange (20.0f, 20000.0f);
    NormalisableRange<float> resRange (1.0f, 5.0f);
    NormalisableRange<float> filterMenuRange (0, 2);
    
    //setting relevant filter data to be stored within the value tree, which will be called later
    tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0f, nullptr, nullptr);
    tree.createAndAddParameter("resonance", "Resonance", "resonance", resRange, 1.0f, nullptr, nullptr);
    tree.createAndAddParameter("filterMenu", "FilterMenu", "filterMenu", filterMenuRange, 0, nullptr, nullptr);
}

SwampVSTAudioProcessor::~SwampVSTAudioProcessor()
{
}

//==============================================================================
const String SwampVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SwampVSTAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SwampVSTAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SwampVSTAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SwampVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SwampVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SwampVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SwampVSTAudioProcessor::setCurrentProgram (int index)
{
}

const String SwampVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void SwampVSTAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SwampVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate; //holds the current sample rate
    String message;
   
    //allows us to pass on information into the dsp module
    dsp::ProcessSpec spec;
    spec.sampleRate = currentSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    stateVariableFilter.reset(); //clears the parameters of the filter if audio stops
    stateVariableFilter.prepare(spec); //initialises the filter
    
    if (oscillators[0].isInitialised()) //checking to see if the oscillators are initialised and ready to go
    {
        message << "yes!" << newLine;
    }
}

void SwampVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SwampVSTAudioProcessor::updateFilter() //updating the state/type of filter in here
{
    int menuChoice = *tree.getRawParameterValue("filterMenu"); //accessing free form data from the value trees
    int cutoff = *tree.getRawParameterValue("cutoff");
    int res = *tree.getRawParameterValue("resonance");
    
    if (menuChoice == 0) //if menuChoice is 0, set it to lowpass
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass; //accessing the parameters of the filter in the process duplicator, telling it what sort of filter it should be
        stateVariableFilter.state->setCutOffFrequency (currentSampleRate, cutoff, res); //relay the parameters
        
    }
    
    if (menuChoice == 1) //if menuChoice is 1, set it to bandpass
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency (currentSampleRate, cutoff, res);
    }
    
    if (menuChoice == 2) //if menuChoice is 2, set it to highpass
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency (currentSampleRate, cutoff, res);
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SwampVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void SwampVSTAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels(); //getting the number of audio input channels
    auto totalNumOutputChannels = getTotalNumOutputChannels(); //getting the numnber of audio output channels
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    dsp::AudioBlock<float> block(buffer); //audio block is equal to the buffer
    AudioBuffer<float> temp(buffer.getNumChannels(), buffer.getNumSamples());
    
    for (int i = 0; i < 3; i++)
        //this for loop is where the oscillators are given information about the buffer to suit the data requirements of the dsp module as a type float
    {
        dsp::AudioBlock<float> tempBlock(temp); //information stored in a temp block
        oscillators[i].process(dsp::ProcessContextReplacing<float>(tempBlock));
        block.addWithMultiply(tempBlock, fact[i]); //applying the data stored in the buffer to the facts, which will be used to control the gain of 
    }
    
    gain.setGainDecibels(-24.0f); //setting the gain in decibels so that the audio won't clip the master fader
    gain.process(dsp::ProcessContextReplacing <float> (block)); //presenting the data appropriately to the dsp module to control the gain
   
    updateFilter(); //update the state of the filter
    stateVariableFilter.process(dsp::ProcessContextReplacing <float> (block));  //filter accessing the sample rate of the buffer here
}

//==============================================================================
bool SwampVSTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SwampVSTAudioProcessor::createEditor()
{
    return new SwampVSTProcessorEditor (*this);
}

//==============================================================================
void SwampVSTAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SwampVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SwampVSTAudioProcessor();
}

void SwampVSTAudioProcessor::timerCallback()
{
    
}

void SwampVSTAudioProcessor::updateAngleDelta()
{
    
}

void SwampVSTAudioProcessor::setCurrentLevel(double cl)
{
    
}
