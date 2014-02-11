/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <complex>
#include <math.h>

AudioProcessor* JUCE_CALLTYPE createPluginFilter();

const float defaultGain = 1.0f;
const float defaultDelay = 0.0f;
const float defaultCutoff = 1.0f;
const std::complex<double> imag = std::sqrt(std::complex<double>(-1));
std::complex<double> arr[4];
std::complex<double> zPoles[4];
std::complex<double> coefficients[4];


//==============================================================================
Project3AudioProcessor::Project3AudioProcessor()
    : delayBuffer(2, 12000), lowPassBuffer(2, 4)
{
    // Set up some default values..
    gain = defaultGain;
    delay = defaultDelay;
    cutoff = defaultCutoff;
    
    lastUIWidth = 400;
    lastUIHeight = 200;
    
    lastPosInfo.resetToDefault();
    delayPosition = 0;
    
    //compute chebyshev poles
    for (int i=0; i<4; i++)
    {
        arr[i] = imag * cos((1.0 / 4.0) * acos((imag/0.5)) + (((i) * double_Pi) / 4));
        std::cout << "S Pole " << i+1 << ": " << arr[i] << "\n";
    }
}

Project3AudioProcessor::~Project3AudioProcessor()
{
}

//==============================================================================
const String Project3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int Project3AudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float Project3AudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     return gain;
        case delayParam:    return delay;
        case cutoffParam:   return cutoff;
        default:            return 0.0f;
    }
}

void Project3AudioProcessor::setParameter (int index, float newValue)
{
    std::complex<double> scaledValue;
    
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     gain = newValue;  break;
        case delayParam:    delay = newValue;  break;
        case cutoffParam:
            cutoff = (newValue / getSampleRate()) * double_Pi;
            
            
            // iterate through poles in s-space scale them and translate to z-space
            for (int i=0; i<4; i++)
            {
                scaledValue = arr[i] * (double)cutoff;
                zPoles[i] = (1.0 + (scaledValue / 2.0)) / (1.0 - (scaledValue / 2.0));
                
                std::cout << "Z Pole " << i+1 << ": " << zPoles[i] << "\n";
            }
            
            // calculate coefficients
            coefficients[0] = -1.0 * (zPoles[0] + zPoles[1] + zPoles[2] + zPoles[3]);
            coefficients[1] = (zPoles[0] * zPoles[1]) + (zPoles[0] * zPoles[2]) + (zPoles[0] * zPoles[3]) +
                (zPoles[1] * zPoles[2]) + (zPoles[1] * zPoles[3]) + (zPoles[2] * zPoles[3]);
            coefficients[2] = -1.0 * ((zPoles[0] * zPoles[1] * zPoles[2]) + (zPoles[0] * zPoles[1] * zPoles[3]) +
                (zPoles[0] * zPoles[2] * zPoles[3]) + (zPoles[1] * zPoles[2] * zPoles[3]));
            coefficients[3] = zPoles[0] * zPoles[1] * zPoles[2] * zPoles[3];
            
            for (int i=0; i<4; i++)
            {
                std::cout << "Coefficient " << i+1 << ": " << coefficients[i] << "\n";
            }
            
            break;
        default:            break;
    }
}

float Project3AudioProcessor::getParameterDefaultValue (int index)
{
    switch (index)
    {
        case gainParam:     return defaultGain;
        case delayParam:    return defaultDelay;
        case cutoffParam:   return defaultCutoff;
        default:            break;
    }
    
    return 0.0f;
}

const String Project3AudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
        case delayParam:    return "delay";
        case cutoffParam:   return "cutoff";
        default:            break;
    }
    
    return String::empty;
}

const String Project3AudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String Project3AudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String Project3AudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool Project3AudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool Project3AudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool Project3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Project3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Project3AudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double Project3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Project3AudioProcessor::getNumPrograms()
{
    return 0;
}

int Project3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Project3AudioProcessor::setCurrentProgram (int index)
{
}

const String Project3AudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void Project3AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Project3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    keyboardState.reset();
    delayBuffer.clear();
    lowPassBuffer.clear();
}

void Project3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void Project3AudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBuffer.clear();
    lowPassBuffer.clear();
}

void Project3AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    int channel, dp = 0;
    
    // Go through the incoming data, and apply our gain to it...
    for (channel = 0; channel < getNumInputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
    
    // Apply our delay effect to the new output..
    for (channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData (channel);
        float* delayData = delayBuffer.getSampleData (jmin (channel, delayBuffer.getNumChannels() - 1));
        dp = delayPosition;
        
        for (int i = 0; i < numSamples; ++i)
        {
            const float in = channelData[i];
            channelData[i] += delayData[dp];
            delayData[dp] = (delayData[dp] + in) * delay;
            if (++dp >= delayBuffer.getNumSamples())
                dp = 0;
        }
    }
    
    // Iterate over all channels
    for (channel = 0; channel < getNumInputChannels(); channel++)
    {
        // Raw sample values
        float *channelData = buffer.getSampleData(channel);
        
        // Buffer of previous 4 outputs initially set to 0s
        float *lowPassData = lowPassBuffer.getSampleData(jmin(channel, lowPassBuffer.getNumChannels() - 1));
        
        // Iterate over all samples
        for (int i=0; i < numSamples; i++)
        {
            // Iterate through previous outputs and perform calculation for next output
            for (int j=0; j<lowPassBuffer.getNumSamples(); j++)
            {
                channelData[i] += -1.0 * coefficients[j].real() * lowPassData[j];
            }
            
            for (int j=lowPassBuffer.getNumSamples()-1; j>=0; j--)
            {
                if (j == 0)
                {
                    lowPassData[0] = channelData[i];
                }
                else
                {
                    lowPassData[j] = lowPassData[j-1];
                }
            }
            
        }
    }
    
    delayPosition = dp;

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
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
bool Project3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Project3AudioProcessor::createEditor()
{
    return new Project3AudioProcessorEditor (this);
}

//==============================================================================
void Project3AudioProcessor::getStateInformation (MemoryBlock& destData)
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

void Project3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new Project3AudioProcessor();
}
