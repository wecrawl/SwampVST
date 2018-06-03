/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SwampVSTProcessorEditor::SwampVSTProcessorEditor (SwampVSTAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    setResizable(true,true);
    for (int i = 0; i < 3; i++)
    {
        //setting the properties of the array of oscillator sliders
        addAndMakeVisible(oscSlider[i]);
        oscSlider[i].setRange(0.0f, 1.0f);
        oscSlider[i].setValue(processor.currentLevel);
        oscSlider[i].setSliderStyle(juce::Slider::SliderStyle::Rotary);
        oscSlider[i].setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        oscSlider[i].setPopupDisplayEnabled(true, true, this);
        oscSlider[i].addListener(this);
        oscSlider[i].setColour (Slider::thumbColourId, Colours::lightsteelblue);
    }
    
     //setting the properties of the frequency slider
     addAndMakeVisible(freqSlider);
     freqSlider.setRange(20.0f, 500.0f);
     freqSlider.setValue(processor.frequency);
     freqSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
     freqSlider.setPopupDisplayEnabled(true, true, this);
     freqSlider.addListener(this);
     freqSlider.setColour (Slider::thumbColourId, Colours::steelblue);
    
     //setting the properties of the filter cutoff dial
     filterCutoffDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     filterCutoffDial.setRange(20.0f, 20000.0f);
     filterCutoffDial.setValue(600.0f);
     filterCutoffDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
     filterCutoffDial.setPopupDisplayEnabled(true, true, this);
     filterCutoffDial.setColour (Slider::thumbColourId, Colours::steelblue);
     addAndMakeVisible(&filterCutoffDial);
    
     //setting the properties for the filter resonance dial
     filterResDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     filterResDial.setRange(20.0f, 20000.0f);
     filterResDial.setValue(600.0f);
     filterResDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
     filterResDial.setPopupDisplayEnabled(true, true, this);
     filterResDial.setColour (Slider::thumbColourId, Colours::steelblue);
     addAndMakeVisible(&filterResDial);
    
     filterMenu.setJustificationType(Justification::centredTop);
     filterMenu.addItem("Low Pass", 1);
     filterMenu.addItem("Band Pass", 2);
     filterMenu.addItem("High Pass", 3);
     addAndMakeVisible(&filterMenu);
    
    //setting up value trees to manage data
    filterCutoffValue = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "cutoff", filterCutoffDial);
    filterResValue = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "resonance", filterResDial);
    filterMenuChoice = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "filterMenu", filterMenu);
    
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f); //setting up a logarithmic mid point of 1000Hz
    
    setSize (500, 500);
}

SwampVSTProcessorEditor::~SwampVSTProcessorEditor()
{
}

//==============================================================================
void SwampVSTProcessorEditor::paint (Graphics& g)
{
    //setting colours and laying out the GUI
    g.fillAll(Colours::darkseagreen);

    g.drawText("CUTOFF", 175, 70, 50, 25, Justification::centred);
    g.drawText("RESONANCE", 265, 70, 70, 25, Justification::centred);
  
    g.setColour(Colours:: lightseagreen);
    g.fillRoundedRectangle(80, 175, 350, 125, 25.0f);
    g.setColour(Colours::silver);
    g.fillRoundedRectangle(50, 395, 410, 60, 10);
    
    g.setColour(Colours::darkgrey);
    g.fillRect(60, 395, 380, 25);
    
    g.setColour(Colours::lightslategrey);
    g.fillRect(105, 395, 15, 25);
    g.fillRect(125, 395, 15, 25);
    g.fillRect(205, 395, 15, 25);
    g.fillRect(225, 395, 15, 25);
    g.fillRect(270, 395, 15, 25);
    g.fillRect(290, 395, 15, 25);
    g.fillRect(360, 395, 15, 25);
    g.fillRect(380, 395, 15, 25);
}

void SwampVSTProcessorEditor::resized()
{
    FlexBox oscillatorSliders;  //flexbox to space the oscillator sliders out equally together
    oscillatorSliders.flexWrap=FlexBox::Wrap::wrap;
    oscillatorSliders.justifyContent=FlexBox::JustifyContent::center;
    oscillatorSliders.alignContent=FlexBox::AlignContent::center;
   
   
    for (int i = 0; i < 3; i++)
    {
        oscillatorSliders.items.add(FlexItem(oscSlider[i]).withMinWidth(100).withMinHeight(100));
    }
    
    oscillatorSliders.performLayout(getLocalBounds());
    
    freqSlider.setBounds(50, 390, 400, 80); //setting the bounds for slider labels
    freqLabel.setBounds(210, 330, 390, 80);
    oscLabel1.setBounds(120, 150, 400, 80);
    oscLabel2.setBounds(220, 150, 400, 80);
    oscLabel3.setBounds(310, 150, 400, 80);
    
    freqLabel.setText("FREQUENCY", dontSendNotification);   //setting the text for the slider labels
    oscLabel1.setText("SINE MIX", dontSendNotification);
    oscLabel2.setText("SAW MIX", dontSendNotification);
    oscLabel3.setText("SQUARE MIX", dontSendNotification);
    
    
    addAndMakeVisible(&freqLabel);  //making all labels visible
    addAndMakeVisible(&oscLabel1);
    addAndMakeVisible(&oscLabel2);
    addAndMakeVisible(&oscLabel3);
    
    Rectangle<int> area = getLocalBounds().reduced(25);
    filterMenu.setBounds(area.removeFromTop(20));
    filterCutoffDial.setBounds(165, 90, 70, 70);
    filterResDial.setBounds(265, 90, 70, 70);
}

void SwampVSTProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &freqSlider) //if statements that map the gain fact of each oscillator to their corresponding slider
    {
        for (int i = 0; i < 3; i++)
        {
            processor.oscillators[i].setFrequency(freqSlider.getValue());
        }
            if  (slider == &oscSlider[0])
            {
                processor.fact[0] = oscSlider[0].getValue();
            }
        
            if (slider == &oscSlider[1])
            {
                processor.fact[1] = oscSlider[1].getValue();
            }
        
            if (slider == &oscSlider[2])
            {
                processor.fact[2] = oscSlider[2].getValue();
            }
    }
}
