#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkMuffProcessor::PunkMuffProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "parameters", createParams())
#endif
{
}

PunkMuffProcessor::~PunkMuffProcessor()
{
}

//==============================================================================
const juce::String PunkMuffProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PunkMuffProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PunkMuffProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PunkMuffProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PunkMuffProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PunkMuffProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PunkMuffProcessor::getCurrentProgram()
{
    return 0;
}

void PunkMuffProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String PunkMuffProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void PunkMuffProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout PunkMuffProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
        
    params.push_back(std::make_unique<juce::AudioParameterBool>("ONOFF", "On/Off", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), DEFAULT_SUSTAIN, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TONE", "Tone", juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), DEFAULT_TONE, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LEVEL", "Output level", juce::NormalisableRange<float>(-18.0f, 18.0f, 0.01f), DEFAULT_LEVEL, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterInt>("MODE", "Mode", 0, 2, DEFAULT_MODE));
        
    return { params.begin(), params.end() };
}

// ============ VALUE UPDATERS =====================
void PunkMuffProcessor::updateOnOff()
{
    auto ONOFF = state.getRawParameterValue("ONOFF");
    on = ONOFF->load();
}

void PunkMuffProcessor::updateSustain()
{
    auto SUS = state.getRawParameterValue("SUSTAIN")->load();
    
    float susLvl = juce::jmap(SUS, 0.f, 10.f, 15.f, 45.f);
    float susCompLvl = juce::jmap(SUS, 0.f, 10.f, -10.f, -30.f);
    
    sustainLevel.setGainDecibels(susLvl);
    sustainCompLevel.setGainDecibels(susCompLvl);
}

void PunkMuffProcessor::updateTone()
{
    auto TONE = state.getRawParameterValue("TONE")->load();
    auto MODE = (int) state.getRawParameterValue("MODE")->load();
    
    float lowPassFrec, lowShelfDip, highShelfBoost, highShelfFrec, midDipFrec, midDipQ, midDipGain;
    switch (MODE) {
        // Big Muff Pi
        case 0:
            lowPassFrec = juce::jmap(TONE, 0.f, 10.f, 800.f, 16000.f);
            lowShelfDip = juce::jmap(TONE, 0.f, 10.f, 1.f, 0.4f);
            highShelfBoost = juce::jmap(TONE, 0.f, 10.f, 0.7f, 1.5f);
            highShelfFrec = 2400.f;
            midDipFrec = 1000.f;
            midDipQ = juce::jmap(abs(TONE - 5.f), 0.f, 5.f, 1.2f, 0.9f);
            midDipGain = juce::jmap(abs(TONE - 5.f), 0.f, 5.f, 0.5f, 0.9f);
            break;
        // Elk Sustainer
        case 1:
            lowPassFrec = juce::jmap(TONE, 0.f, 10.f, 800.f, 16000.f);
            lowShelfDip = juce::jmap(TONE, 0.f, 10.f, 1.f, 0.4f);
            highShelfBoost = juce::jmap(TONE, 0.f, 10.f, 0.7f, 1.5f);
            highShelfFrec = 3000.f;
            midDipFrec = juce::jmap(TONE, 0.f, 10.f, 6000.f, 1200.f);
            midDipQ = juce::jmap(TONE, 0.f, 10.f, 1.f, 3.f);
            midDipGain = juce::jmap(TONE, 0.f, 10.f, 0.6f, 0.3f);
            break;
        // Another pedal ???
        case 2:
            lowPassFrec = juce::jmap(TONE, 0.f, 10.f, 800.f, 16000.f);
            lowShelfDip = juce::jmap(TONE, 0.f, 10.f, 1.f, 0.4f);
            highShelfBoost = juce::jmap(TONE, 0.f, 10.f, 0.7f, 1.5f);
            highShelfFrec = 3000.f;
            midDipFrec = juce::jmap(TONE, 0.f, 10.f, 6000.f, 1200.f);
            midDipQ = juce::jmap(TONE, 0.f, 10.f, 1.f, 3.f);
            midDipGain = juce::jmap(TONE, 0.f, 10.f, 0.6f, 0.3f);
            break;
            
        default:
            break;
    }
    
    
    double sampleRate = getSampleRate();
    
    *toneEq.get<0>().state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassFrec);
    *toneEq.get<1>().state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 630.f, 1.f, lowShelfDip);
    *toneEq.get<2>().state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, highShelfFrec, 1.4f, highShelfBoost);
    *toneEq.get<3>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midDipFrec, midDipQ, midDipGain);
}

void PunkMuffProcessor::updateLevel()
{
    auto LVL = state.getRawParameterValue("LEVEL")->load();
        
    outputLevel.setGainDecibels(LVL);
}

void PunkMuffProcessor::updateState()
{
    updateOnOff();
    updateSustain();
    updateTone();
    updateLevel();
}

//==============================================================================
void PunkMuffProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    sustainLevel.prepare(spec);
    sustainLevel.setRampDurationSeconds(0.1f);
    sustainCompLevel.prepare(spec);
    sustainCompLevel.setRampDurationSeconds(0.1f);
    
    preEq.prepare(spec);
    preEq.reset();
    *preEq.get<0>().state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 1200.f);
    *preEq.get<1>().state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 30.f);
    
    clipper.prepare(spec);
    clipper.reset();
    clipper.functionToUse = doubleClipper;

    toneEq.prepare(spec);
    toneEq.reset();
    
    outputLevel.prepare(spec);
    outputLevel.setRampDurationSeconds(0.1f);
}

void PunkMuffProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PunkMuffProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PunkMuffProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateState();
    if(on)
    {
        juce::dsp::AudioBlock<float> audioBlock = juce::dsp::AudioBlock<float>(buffer);
        
        preEq.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
        sustainLevel.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        sustainCompLevel.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
        // TODO: Add oversampling ??
        clipper.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
                
        toneEq.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
        outputLevel.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }
}

//==============================================================================
bool PunkMuffProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PunkMuffProcessor::createEditor()
{
    return new PunkMuffEditor (*this);
}

//==============================================================================
void PunkMuffProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::ignoreUnused(destData);
}

void PunkMuffProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PunkMuffProcessor();
}
