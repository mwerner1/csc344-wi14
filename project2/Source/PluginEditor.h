/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/** This is the editor component that our filter will display.
 */
class Project2AudioProcessorEditor  : public AudioProcessorEditor,
                                      public SliderListener,
                                      public Timer,
                                      public Button::Listener
{
public:
    Project2AudioProcessorEditor (Project2AudioProcessor* ownerFilter);
    ~Project2AudioProcessorEditor();
    
    //==============================================================================
    void timerCallback() override;
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider*) override;
    void buttonClicked (Button *) override;
    
private:
    MidiKeyboardComponent midiKeyboard;
    Label infoLabel, gainLabel, delayLabel, waveTypeLabel;
    Slider gainSlider, delaySlider;
    ToggleButton sineWaveButton, squareWaveButton, sawToothWaveButton;
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    
    AudioPlayHead::CurrentPositionInfo lastDisplayedPosition;
    
    Project2AudioProcessor* getProcessor() const
    {
        return static_cast <Project2AudioProcessor*> (getAudioProcessor());
    }
    
    void displayPositionInfo (const AudioPlayHead::CurrentPositionInfo& pos);
};

#endif  // PLUGINEDITOR_H_INCLUDED
