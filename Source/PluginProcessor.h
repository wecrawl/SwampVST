/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 */

class SwampVSTAudioProcessor  : public AudioProcessor, public Timer
{
public:
    //==============================================================================
     SwampVSTAudioProcessor();
    ~SwampVSTAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void updateAngleDelta();
    
    double currentLevel = 0.0f, frequency = 0.0f;
    
    void setCurrentLevel(double cl);
    
    double freq= 0.0f;
    double getFreq(){ return freq; };
    
    dsp::Oscillator<float> oscillators[5] =  //setting up the oscillators using the DSP oscillator class
    {
        // Approximated by a wave-table
        {[] (float x) { return std::sin (x); }, 100},                 // sine wave
        {[] (float x) { return x / MathConstants<float>::pi; }, 100}, // saw wave
        {[] (float x) { return x < 0.0f ? -1.0f : 1.0f; }, 100},      //square wave
    };
    
    dsp::Gain<float> gain;
    
    float fact[3] = {0.5f, 0.0f, 0.0f}; //setting up three facts to control the oscillator gain, setting their default gain values. These values could change, depending on how loud you would want them. I am currently happy with this balance
    
    void updateFilter(); //this function will update the parameters of the filter
    void updateParameters (); //updating the parameters of the filter for low, high and band pass
    
    AudioProcessorValueTreeState tree; //value tree to manage free form data associated with the filter parameters 
    
private:
    dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> stateVariableFilter; //setting up the state varible filters using DSP, declaring the filter itself, and then the parameters of the filter
    
    double currentSampleRate, currentAngle = 0.0, angleDelta = 0.0;
    void timerCallback() override;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwampVSTAudioProcessor)
};
