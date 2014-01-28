/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter();


//==============================================================================
// Basic sine wave synth sound
class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound() {}
    
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

// Square wave synth sound
class SquareWaveSound : public SynthesiserSound
{
public:
    SquareWaveSound() {}
    
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

// Sawtooth wave synth sound
class SawToothWaveSound : public SynthesiserSound
{
public:
    SawToothWaveSound() {}
    
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

//==============================================================================
// Synth voice that plays either a sine wave, square wave, or sawtooth wave
class SineWaveVoice  : public SynthesiserVoice
{
public:
    SineWaveVoice()
    : angleDelta (0.0),
    lfoAngleDelta(0.0),
    tailOff (0.0)
    {
    }
    
    // Determines if sound is a sawtooth, square, or sine wave sound
    bool canPlaySound (SynthesiserSound* sound)
    {
        // Determine if sound is a sine wave, square wave, or sawtooth by casting the sound
        // and checking to see if its value is non-zero
        if (dynamic_cast<SineWaveSound*>(sound) != 0)
        {
            return true;
        }
        else if (dynamic_cast<SquareWaveSound*>(sound) != 0)
        {
            return true;
        }
        else if (dynamic_cast<SawToothWaveSound*>(sound) != 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    // Caculates the angle change for both the sound wave and the LFO wave using the midi
    // note in hertz and an arbitrary 6 hertz frequency for the LFO
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/)
    {
        currentAngle = 0.0;
        lfoCurrentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        
        double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        double lfoFrequency = 8.0;
        
        double cyclesPerSample = cyclesPerSecond / getSampleRate();
        double lfoCyclesPerSample = lfoFrequency / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * double_Pi;
        lfoAngleDelta = lfoCyclesPerSample * 2.0 * double_Pi;
        
    }
    
    // Begins a tail off or stops playing immediatedly and resets angle delta values for the sound
    // and the LFO
    void stopNote (bool allowTailOff)
    {
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.
            
            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                // stopNote method could be called more than once.
                tailOff = 1.0;
        }
        else
        {
            // we're being told to stop playing immediately, so reset everything..
            
            clearCurrentNote();
            angleDelta = 0.0;
            lfoAngleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved (int /*newValue*/)
    {
        // can't be bothered implementing this for the demo!
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/)
    {
        // not interested in controllers in this case.
    }
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                SynthesiserSound *sound = getCurrentlyPlayingSound();
                
                while (--numSamples >= 0)
                {
                    float currentSample;
                    
                    // LFO used to create tremolo
                    const float lfoCurrentSample = (float) ((4.0 * sin (lfoCurrentAngle)) * level * tailOff) + 0.7;
                    
                    currentSample = calculateSample(sound, currentSample, lfoCurrentSample, true);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += (currentSample);
                    
                    currentAngle += angleDelta;
                    lfoCurrentAngle += lfoAngleDelta;
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        lfoAngleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                SynthesiserSound *sound = getCurrentlyPlayingSound();
                
                while (--numSamples >= 0)
                {
                    float currentSample;
                    
                    // LFO used to create tremolo
                    const float lfoCurrentSample = (float) ((4.0 * sin (lfoCurrentAngle)) * level) + 0.7;
                    
                    currentSample = calculateSample(sound, currentSample, lfoCurrentSample, false);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += (currentSample);
                    
                    currentAngle += angleDelta;
                    lfoCurrentAngle += lfoAngleDelta;
                    ++startSample;
                }
            }
        }
    }
    
    float calculateSample(SynthesiserSound *sound, float currentSample, float lfo, bool hasTailOff)
    {
        if (hasTailOff)
        {
            if (dynamic_cast<SineWaveSound*>(sound) != NULL)
            {
                currentSample = (float) ((lfo * sin (currentAngle)) * level * tailOff);
            }
            else if (dynamic_cast<SquareWaveSound*>(sound) != NULL)
            {
                float sum = 0.0;
                
                for (int i=0; i<10; i++)
                {
                    sum += lfo * sin(currentAngle * (2 * i + 1)) * level * tailOff /
                    (2 * i + 1);
                }
                
                currentSample = sum;
            }
            else if (dynamic_cast<SawToothWaveSound*>(sound) != NULL)
            {
                float sum = 0.0;
                
                for (int i=1; i<10; i++)
                {
                    sum += lfo * (pow(-1.0, (double)i+1) / i) * (sin(currentAngle * i)) *
                    level * tailOff;
                }
                currentSample = sum;
            }
            else
            {
                // Default to a sine wave
                currentSample = (float) ((lfo * sin (currentAngle)) * level * tailOff);
            }
        }
        else
        {
            if (dynamic_cast<SineWaveSound*>(sound) != NULL)
            {
                currentSample = (float) ((lfo * sin (currentAngle)) * level);
            }
            else if (dynamic_cast<SquareWaveSound*>(sound) != NULL)
            {
                float sum = 0.0;
                
                for (int i=0; i<10; i++)
                {
                    sum += lfo * sin(currentAngle * (2 * i + 1)) * level / (2 * i + 1);
                }
                
                currentSample = sum;
            }
            else if (dynamic_cast<SawToothWaveSound*>(sound) != NULL)
            {
                float sum = 0.0;
                
                for (int i=1; i<10; i++)
                {
                    sum += lfo * (pow(-1.0, (double)i+1) / i) * (sin(currentAngle * i)) * level;
                }
                
                currentSample = sum;
            }
            else
            {
                //Default to a sine wave
                currentSample = (float) ((lfo * sin (currentAngle)) * level);
            }
        }
        
        return currentSample;
    }
    
private:
    double currentAngle, lfoCurrentAngle, angleDelta, lfoAngleDelta, level, tailOff;
};

const float defaultGain = 1.0f;
const float defaultDelay = 0.5f;
const Project2AudioProcessor::Wave defaultWaveType = Project2AudioProcessor::sine;

//==============================================================================
Project2AudioProcessor::Project2AudioProcessor()
    : delayBuffer (2, 12000)
{
    // Set up some default values..
    gain = defaultGain;
    delay = defaultDelay;
    waveType = defaultWaveType;
    
    lastUIWidth = 400;
    lastUIHeight = 200;
    
    lastPosInfo.resetToDefault();
    delayPosition = 0;
    
    // Initialise the synth...
    for (int i = 4; --i >= 0;)
        synth.addVoice (new SineWaveVoice());   // These voices will play our custom sine-wave sounds..
    
    synth.addSound (new SineWaveSound());
}

Project2AudioProcessor::~Project2AudioProcessor()
{
}

//==============================================================================
int Project2AudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float Project2AudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     return gain;
        case delayParam:    return delay;
        default:            return 0.0f;
    }
}

void Project2AudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:
            gain = newValue;
            break;
        case delayParam:
            delay = newValue;
            break;
        case waveParam:
            waveType = (Wave)newValue;
        
            synth.clearSounds();
            
            if (waveType == sine)
            {
                synth.addSound(new SineWaveSound());
                
            }
            else if (waveType == square)
            {
                synth.addSound(new SquareWaveSound());
            }
            else if (waveType == sawTooth)
            {
                synth.addSound(new SawToothWaveSound());
            }
            break;
        default:
            break;
    }
}

const String Project2AudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
        case delayParam:    return "delay";
        default:            break;
    }
    
    return String::empty;
}

const String Project2AudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String Project2AudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String Project2AudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool Project2AudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool Project2AudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool Project2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Project2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Project2AudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double Project2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//int Project2AudioProcessor::getNumPrograms()
//{
//    return 0;
//}
//
//int Project2AudioProcessor::getCurrentProgram()
//{
//    return 0;
//}
//
//void Project2AudioProcessor::setCurrentProgram (int index)
//{
//}
//
//const String Project2AudioProcessor::getProgramName (int index)
//{
//    return String::empty;
//}
//
//void Project2AudioProcessor::changeProgramName (int index, const String& newName)
//{
//}

//==============================================================================
void Project2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate (sampleRate);
    keyboardState.reset();
    delayBuffer.clear();
}

void Project2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void Project2AudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBuffer.clear();
}

void Project2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    int channel, dp = 0;
    
    // Go through the incoming data, and apply our gain to it...
    for (channel = 0; channel < getNumInputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
    
    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
    
    // and now get the synth to process these midi events and generate its output.
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    // **Commented out this code because we don't want delay effect for assgn 2**
    // Apply our delay effect to the new output..
//    for (channel = 0; channel < getNumInputChannels(); ++channel)
//    {
//        float* channelData = buffer.getSampleData (channel);
//        float* delayData = delayBuffer.getSampleData (jmin (channel, delayBuffer.getNumChannels() - 1));
//        dp = delayPosition;
//        
//        for (int i = 0; i < numSamples; ++i)
//        {
//            const float in = channelData[i];
//            channelData[i] += delayData[dp];
//            delayData[dp] = (delayData[dp] + in) * delay;
//            if (++dp >= delayBuffer.getNumSamples())
//                dp = 0;
//        }
//    }
//    
//    delayPosition = dp;
    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // ask the host for the current time so we can display it...
    AudioPlayHead::CurrentPositionInfo newTime;
    
    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (newTime))
    {
        // Successfully got the current time from the host..
        lastPosInfo = newTime;
    }
    else
    {
        // If the host fails to fill-in the current time, we'll just clear it to a default..
        lastPosInfo.resetToDefault();
    }
}

//==============================================================================
//bool Project2AudioProcessor::hasEditor() const
//{
//    return true; // (change this to false if you choose to not supply an editor)
//}

AudioProcessorEditor* Project2AudioProcessor::createEditor()
{
    return new Project2AudioProcessorEditor (this);
}

//==============================================================================
void Project2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:
    
    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain);
    xml.setAttribute ("delay", delay);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void Project2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            
            gain  = (float) xmlState->getDoubleAttribute ("gain", gain);
            delay = (float) xmlState->getDoubleAttribute ("delay", delay);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Project2AudioProcessor();
}
