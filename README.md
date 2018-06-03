# SwampVST

![swampVST](Resources/swampVST.png)

## About
The SwampVST is intended to be used as a compositional tool within a DAW. The name stemmed from the idea of combining various oscillators (additive synthesis) to sonically craft a swamp of noise. The SwampVST aims to enable present people with a simple, minimalistic workspace to produce audio with, whether it be for music production or sound designing. 

## How It Works
The VST was written in C++ using Projucer. In order for this project to run, you will need to open it as a stand alone VST. 
As with analog circuitry, signals need to be amplified (or processed in this instance) so that they can be analysed and manipulated, through converting the signal into the appropriate format in order to access the information that it contains. I used the DSP module in Juce to generate the sound of the oscillators within the VST through using algorithms to generate each wave form, which is then converted into audible sound for the user to hear and craft through interacting with the GUI of the VST. 

## GUI Design Considerations 
I wanted to keep the GUI rather minimal and clean, so that it is easy to use. 
simplistic, clean GUI to enable the user to easily and creatively interact with the VST, as creativity is often piqued by the simplest of designs 
why its simple and why i decided to make it look the way i did

## Bugs
The current version of the SwampVST is unable to be opened in Ableton Live — it crashes when placing the VST on an empty midi track. This only happened when the state variable filter was added to the VST, so that is a major issue that needs to be solved in order to use the VST within Ableton Live to create music with it. Upon launching the standalone program, the VST automatically plays a 440Hz sine wave — I am yet to figure out how to disable this. The window is resizable, but the oscillator sliders will move with the enlarged window, everything else stays put. There is also an error that pops up when quitting the VST, claiming that the program "quit unexpectedly."

## Future Considerations
Note on/off would be a feature that I'd like to add to the frequency slider, which would be implemented when the slider is clicked and released, turning a note on/off. In conjunction with this, implementing ADSR into the VST would give more control over the envelope of each note, allowing the user to craft the audio more meticulously. A pitch scaling feature, much like what is seen on a lot of other VST's and hardware synth/MIDI controllers, would also be great as it again allows the user to change the range of notes that they are working within. Implementing the possibility of MIDI control for the VST would be something that I'd like to add to the VST.
