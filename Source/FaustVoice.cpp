#include "FaustVoice.h"
#include <vector>

FaustVoice::FaustVoice()
{
  // Initialize Faust
  fMS = std::make_unique<::MonoSource>();
  fUI = std::make_unique<::MapUI>();
  fMS->buildUserInterface(fUI.get());
  fMS->init(getSampleRate());
}

FaustVoice::~FaustVoice() = default;

void FaustVoice::setCurrentPlaybackSampleRate (double newRate) {
  // Cannot call fMS->init(newRate) because it expands as below:
  fMS->classInit(newRate); // This is ok, and needed
  // Cannot call fMS->instanceInit(newRate) because it expands as below:
  fMS->instanceConstants(newRate); // This is ok, and needed
  // Requires resetting parameters we don't have cached:
  // fMS->instanceResetUserInterface(); // sets to defaults - SHOULD BE A UI METHOD NOT DSP
  fMS->instanceClear(); // This is ok, and cleaner to do
}

void FaustVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/)
{
  // Set midi note and velocity based off of incoming message
  setParam("midiNote", midiNoteNumber);
  setParam("velocity", velocity);
  // Turn the note on (Attack phase)
  setParam("gate", 1);
  // Clear tail off
  tailOff = 0;
}

void FaustVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
  // Turn note off
  setParam("gate", 0);
  // Allow for release
  if (allowTailOff)
  {
    // Compute how many samples we need to tail off
    // Based on release setting
    float releaseTimeSec = fUI->getParamValue("release");
    tailOff = int(std::ceil(releaseTimeSec * getSampleRate()));
  }
  // If no global tail off, just remove the note
  else
    clearCurrentNote();
}

void FaustVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
  // Get our number of output channels and number of outputs from Faust
  int nChannelsOut = outputBuffer.getNumChannels();
  int nChannelsFaust = fMS->getNumOutputs();
  // Create a temporary audio buffer to store our Faust outputs
  juce::AudioBuffer<float> tmpBuffer = juce::AudioBuffer<float>(nChannelsFaust, numSamples);
  // Call compute
#if 1
  // The "safer way":
  float*const* writePointers = tmpBuffer.getArrayOfWritePointers();
  std::vector<float*> tmpBuffersFaust(nChannelsFaust);
  for (int i=0; i<nChannelsFaust; i++)
    tmpBuffersFaust[i] = writePointers[i];
  fMS->compute(numSamples, NULL, tmpBuffersFaust.data());
#else
  // But if you're in a hurry:
  fMS->compute(numSamples, NULL, const_cast<float**>(tmpBuffer.getArrayOfWritePointers()));
#endif

  // Add Faust output channels to main buffer
  for (int dstChan = 0; dstChan < nChannelsOut; ++dstChan)
  {
    int srcChan = std::min(nChannelsFaust-1,dstChan);
    outputBuffer.addFrom(dstChan, startSample, tmpBuffer,
                         srcChan, 0, numSamples, 1.0);
  }

  // If we're in tail-off mode
  if (tailOff > 0)
  {
    // Decrement number of samples
    tailOff -= numSamples;
    // Once we're out of samples, clear to indicate
    // the voice is free
    if (tailOff <= 0)
    {
      clearCurrentNote();
      tailOff = 0;
    }
  }
}

// Generic getter and setter wrappers for our Faust class
// We use these internally in noteOn and noteOff
// And externally when our global plugin parameters change
void FaustVoice::setParam(std::string paramID, float newValue)
{
  fUI->setParamValue(paramID, newValue);
}

float FaustVoice::getParam(std::string paramID)
{
  float value = fUI->getParamValue(paramID);
  return value;
}
