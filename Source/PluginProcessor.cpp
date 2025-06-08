/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
  MIDI Note → Carrier Base Frequency
                    ↓
  Modulator Osc → FM Depth → Carrier Osc → ADSR → Output
    ↑                          ↓
  FM Feedback ←────────────────┘
*/




#include "PluginProcessor.h"

// Faust parameter IDs
static juce::ParameterID ampID {"amplitude", 1};
static juce::ParameterID atID {"attack", 1};
static juce::ParameterID dcID {"decay", 1};
static juce::ParameterID stID {"sustain", 1};
static juce::ParameterID rlID {"release", 1};
static juce::ParameterID oscTypeID {"type", 1};
static juce::ParameterID fmEnableID {"fm_enable", 1};
static juce::ParameterID fmRatioID {"fm_ratio", 1};
static juce::ParameterID fmDepthID {"fm_depth", 1};
static juce::ParameterID fmFeedbackID {"fm_feedback", 1};
static juce::ParameterID fmTypeID {"fm_type", 1};

static juce::Identifier oscilloscopeID { "oscilloscope" };

juce::AudioProcessorValueTreeState::ParameterLayout FaustSynthStarter::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto adsrGroup = std::make_unique<juce::AudioProcessorParameterGroup>("adsr", "ADSR","|");
    adsrGroup->addChild(std::make_unique<juce::AudioParameterFloat>(atID, "Attack", juce::NormalisableRange<float>(0, 1.0, 0.01), 0.1));
    adsrGroup->addChild(std::make_unique<juce::AudioParameterFloat>(dcID, "Decay", juce::NormalisableRange<float>(0, 1.0, 0.01), 0.25));
    adsrGroup->addChild(std::make_unique<juce::AudioParameterFloat>(stID, "Sustain", juce::NormalisableRange<float>(0, 1.0, 0.01), 0.5));
    adsrGroup->addChild(std::make_unique<juce::AudioParameterFloat>(rlID, "Release", juce::NormalisableRange<float>(0, 5.0, 0.01), 1.0));
    layout.add(std::move(adsrGroup));
    
    // Carrier Oscillator Parameters
    auto carrierGroup = std::make_unique<juce::AudioProcessorParameterGroup>("carrier", "Carrier", "|");
    carrierGroup->addChild(std::make_unique<juce::AudioParameterChoice>(oscTypeID, "Type", juce::StringArray("Sine", "Square", "Triangle", "Saw"), 0));
    layout.add(std::move(carrierGroup));
    
    // FM Parameters Group
    auto fmGroup = std::make_unique<juce::AudioProcessorParameterGroup>("fm", "FM Synthesis", "|");
    fmGroup->addChild(std::make_unique<juce::AudioParameterBool>(fmEnableID, "FM Enable", false));
    fmGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fmRatioID, "FM Ratio", juce::NormalisableRange<float>(0.01f, 8.0f, 0.01f), 0.1f));
    fmGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fmDepthID, "FM Depth", juce::NormalisableRange<float>(0.0f, 10.0f, 0.1f), 3.0f));
    fmGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fmFeedbackID, "FM Feedback", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    fmGroup->addChild(std::make_unique<juce::AudioParameterChoice>(fmTypeID, "FM Type", juce::StringArray("Sine", "Square", "Triangle", "Saw"), 0));
    layout.add(std::move(fmGroup));
    
    // Global Parameters
    auto ampParam = std::make_unique<juce::AudioParameterFloat>(ampID, "Amplitude", juce::NormalisableRange<float>(0, 1.0, 0.01), 0.1);
    layout.add(std::move(ampParam));
    
    return layout;
}
//==============================================================================
FaustSynthStarter::FaustSynthStarter()
     :
#ifndef JucePlugin_PreferredChannelConfigurations
       MagicProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
      treeState (*this, nullptr, JucePlugin_Name, createParameterLayout())
{
    // Initialize synthesizer here:
    synth.clearSounds();
    synth.addSound(new FaustSound());
    for (int nVoices = 0; nVoices < NUM_VOICES; ++nVoices)
    {
        synth.addVoice(new FaustVoice());
    }

    // Add listeners to our JUCE parameters
    treeState.addParameterListener(ampID.getParamID(), this);

    // Faust parameters use parameterChanged():
    treeState.addParameterListener(atID.getParamID(), this);
    treeState.addParameterListener(dcID.getParamID(), this);
    treeState.addParameterListener(stID.getParamID(), this);
    treeState.addParameterListener(rlID.getParamID(), this);
    treeState.addParameterListener(oscTypeID.getParamID(), this);
    treeState.addParameterListener(fmEnableID.getParamID(), this);
    treeState.addParameterListener(fmRatioID.getParamID(), this);
    treeState.addParameterListener(fmDepthID.getParamID(), this);
    treeState.addParameterListener(fmFeedbackID.getParamID(), this);
    treeState.addParameterListener(fmTypeID.getParamID(), this);

    // Get amplitude initial value
    amplitude = treeState.getParameter(ampID.getParamID())->getValue();
    
    // Initialize all Faust parameters on startup
    for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
    {
        FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
        if (synthVoice != nullptr)
        {
            // Initialize ADSR parameters
            synthVoice->setParam("attack", treeState.getParameter(atID.getParamID())->getValue());
            synthVoice->setParam("decay", treeState.getParameter(dcID.getParamID())->getValue());
            synthVoice->setParam("sustain", treeState.getParameter(stID.getParamID())->getValue());
            synthVoice->setParam("release", treeState.getParameter(rlID.getParamID())->getValue());
            synthVoice->setParam("type", treeState.getParameter(oscTypeID.getParamID())->getValue());
            
            // Initialize FM parameters
            synthVoice->setParam("fm_enable", treeState.getParameter(fmEnableID.getParamID())->getValue());
            synthVoice->setParam("fm_ratio", treeState.getParameter(fmRatioID.getParamID())->getValue());
            synthVoice->setParam("fm_depth", treeState.getParameter(fmDepthID.getParamID())->getValue());
            synthVoice->setParam("fm_feedback", treeState.getParameter(fmFeedbackID.getParamID())->getValue());
            synthVoice->setParam("fm_type", treeState.getParameter(fmTypeID.getParamID())->getValue());
        }
    }

    // Add PGM oscilloscope to our synthesizer
    oscilloscope = magicState.createAndAddObject<foleys::MagicOscilloscope>(oscilloscopeID, 0);

    // Load GUI
    magicState.setGuiValueTree(BinaryData::Layout_HW3_xml, BinaryData::Layout_HW3_xmlSize);
}

FaustSynthStarter::~FaustSynthStarter()
{
}

//==============================================================================
const juce::String FaustSynthStarter::getName() const
{
    return JucePlugin_Name;
}

bool FaustSynthStarter::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FaustSynthStarter::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FaustSynthStarter::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FaustSynthStarter::getTailLengthSeconds() const
{
    return treeState.getParameter(rlID.getParamID())->getValue();
}

int FaustSynthStarter::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FaustSynthStarter::getCurrentProgram()
{
    return 0;
}

void FaustSynthStarter::setCurrentProgram (int index)
{
}

const juce::String FaustSynthStarter::getProgramName (int index)
{
    return {};
}

void FaustSynthStarter::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FaustSynthStarter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Don't forget to sample rate initialize your synthesizer!
    magicState.prepareToPlay (sampleRate, samplesPerBlock); // required for MagicPlotSource and such
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void FaustSynthStarter::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FaustSynthStarter::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FaustSynthStarter::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    int nSamples = buffer.getNumSamples();

    buffer.clear();                                    // 1. Start with silence
    magicState.processMidiBuffer(midiMessages, nSamples); // 2. Handle MIDI
    synth.renderNextBlock(buffer, midiMessages, 0, nSamples); // 3. Generate audio
    buffer.applyGain(amplitude);                       // 4. Apply volume
    oscilloscope->pushSamples(buffer);                 // 5. Send to display
}

void FaustSynthStarter::parameterChanged (const juce::String& param, float value)
{
    if (param == ampID.getParamID())
    {
      amplitude = value;
    }

    if (param == atID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("attack", value);
        }
    }
    if (param == dcID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("decay", value);
        }
    }
    if (param == stID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("sustain", value);
        }
    }
    if (param == rlID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("release", value);
        }
    }
    if (param == oscTypeID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("type", value);
        }
    }
    if (param == fmEnableID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("fm_enable", value);
        }
    }
    if (param == fmRatioID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("fm_ratio", value);
        }
    }
    if (param == fmDepthID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("fm_depth", value);
        }
    }
    if (param == fmFeedbackID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("fm_feedback", value);
        }
    }
    if (param == fmTypeID.getParamID())
    {
        for(int nVoice=0; nVoice < NUM_VOICES; ++nVoice)
        {
            FaustVoice* synthVoice = dynamic_cast<FaustVoice *>(synth.getVoice(nVoice));
            if (synthVoice != nullptr)
                synthVoice->setParam("fm_type", value);
        }
    }

    // Add Faust Synthesizer Parameter Handling
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FaustSynthStarter();
}
