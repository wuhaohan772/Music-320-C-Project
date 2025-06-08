/*
 ==============================================================================
 
 FaustVoice.h - Basic JUCE+Faust Synth with MIDI and ADSR
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "MonoSource.h"
class MonoSource;
class MapUI;




//==============================================================================
// This "Sound" Class acts as an ID meaning "any FaustVoice".
// Originally only sampling synthesis was implemented in JUCE,
// so a "Sound" represented some wavetable sounding on some MIDI channel(s)
// over some range of MIDI note numbers.
class FaustSound : public juce::SynthesiserSound
{
public:
  bool appliesToNote(int /*midiNoteNumber*/) {return true;}
  bool appliesToChannel(int /*midiNoteNumber*/) {return true;}
};
//==============================================================================

class FaustVoice : public juce::SynthesiserVoice
{
public:
  FaustVoice();
  ~FaustVoice();
  //==============================================================================
  bool canPlaySound(juce::SynthesiserSound* sound) override {return dynamic_cast<FaustSound*>(sound) != nullptr;}
  void setCurrentPlaybackSampleRate (double newRate) override;
  void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
  void stopNote(float /*velocity*/, bool allowTailOff) override;
  
  //==============================================================================
  void pitchWheelMoved(int newPitchWheelValue) override {}
  void controllerMoved(int controllerNumber, int newControllerValue) override {}
  
  void renderNextBlock( juce::AudioBuffer<float> & buffer, int startSample, int numSample) override;
  //==============================================================================
  // Parameter Control functions
  void prepareToPlay(float sampleRate, int maxBlockSize);
  void setParam(std::string paramID, float newValue);
  float getParam(std::string paramID);
  
private:
  // Faust module we manage:
  std::unique_ptr<::MapUI> fUI;          // User interface
  std::unique_ptr<::MonoSource> fMS;    // DSP module

  // Parameters
  int tailOff = 0; // set to ADSR envelope release time and counted down to zero after noteOff when release begins

  foleys::MagicOscilloscope* oscilloscope;
  std::string oscilloscopeID;
};
