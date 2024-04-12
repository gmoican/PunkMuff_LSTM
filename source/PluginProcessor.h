#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#if (MSVC)
#include "ipps.h"
#endif

#define DEFAULT_MODE 0
#define DEFAULT_SUSTAIN 5.0f
#define DEFAULT_TONE 5.0f
#define DEFAULT_LEVEL 0.0f

//==============================================================================
/**
*/

class PunkMuffProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PunkMuffProcessor();
    ~PunkMuffProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //=============== MY STUFF =====================================================
    juce::AudioProcessorValueTreeState state;
    
    // Updaters
    void updateOnOff();
    void updateSustain();
    void updateTone();
    void updateLevel();
    void updateState();
    
    void process(float* samples, int numSamples);

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    using FilterBand = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using WaveShaper = juce::dsp::WaveShaper<float>;
    using Bias = juce::dsp::Bias<float>;
    using Gain = juce::dsp::Gain<float>;
    
    Gain sustainLevel, sustainCompLevel, outputLevel;
    
    juce::dsp::ProcessorChain<FilterBand, FilterBand> preEq;
    juce::dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand> toneEq;
    juce::dsp::ProcessorChain<Bias, WaveShaper> clipper;
    
    bool on;
    
    // Drive functions
    static float doubleClipper(float sample)
    {
        return softClipper( softClipper(sample) );
    }
    
    static float softClipper(float sample)
    {
        return sample / (abs(sample) + 1.f);
    }
    
    static float tanhClipper(float sample)
    {
        return 2.f / juce::MathConstants<float>::pi * juce::dsp::FastMathApproximations::tanh(sample);
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkMuffProcessor)
};
