#pragma once

#include "PluginProcessor.h"
#include "BinaryData.h"

#define DEG2RADS 0.0174533f

//==============================================================================
/**
*/
class PunkMuffEditor  : public juce::AudioProcessorEditor
{
public:
    PunkMuffEditor (PunkMuffProcessor&);
    ~PunkMuffEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //=================== PARAMETER MANIPULATION ===================================
    void setSliderComponent(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, juce::String paramName, bool linStyle);
    void setToggleComponent(juce::ToggleButton& button, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& buttonAttachment, juce::String paramName);
    juce::AffineTransform knobRotation(float radians, float posX, float posY, float scaleFactor);
    
private:
    // Parameters
    juce::Slider sustainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainKnobAttachment;
    
    juce::Slider toneKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneKnobAttachment;
    
    juce::Slider levelKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelKnobAttachment;
    
    juce::Slider modeSwitch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modeSwitchAttachment;
        
    juce::ToggleButton onToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onToggleAttachment;
    
    // Assets
    juce::Image background;
    juce::Image lightOff;
    juce::Image knobImage;
    juce::Image switchTopImage;
        
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PunkMuffProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkMuffEditor)
};
