/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
 */
class SwampVSTProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    SwampVSTProcessorEditor (SwampVSTAudioProcessor&);
    ~SwampVSTProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue; //setting up value trees
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResValue;
    
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment>
    filterMenuChoice;
    
    void sliderValueChanged(Slider* slider) override;
    
    Slider oscSlider[3]; //declaring an array of three rotary sliders for the oscillator mix/gain
    Slider freqSlider; //declaring the frequency slider for the oscillators 
    Slider filterCutoffDial; //declaring filter cutoff dial
    Slider filterResDial;   //declaring filter resonsance dial
    ComboBox filterMenu;    //declaring the selector menu for the state variable filters
    
private:
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SwampVSTAudioProcessor& processor;
    
    Label freqLabel;    //labels for the sliders
    Label oscLabel1;
    Label oscLabel2;
    Label oscLabel3;
    //Image background = ImageCache::getFromMemory(BinaryData::IMG_9412_jpg, BinaryData::IMG_9412_jpgSize);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwampVSTProcessorEditor)
};

