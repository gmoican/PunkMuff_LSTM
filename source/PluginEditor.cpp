#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkMuffEditor::PunkMuffEditor (PunkMuffProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    juce::ignoreUnused(audioProcessor);
    
    // ================= PARAMETERS ====================
    setSliderComponent(sustainKnob, sustainKnobAttachment, "SUSTAIN", false);
    setSliderComponent(toneKnob, toneKnobAttachment, "TONE", false);
    setSliderComponent(levelKnob, levelKnobAttachment, "LEVEL", false);
    
    setSliderComponent(modeSwitch, modeSwitchAttachment, "MODE", true);
    
    setToggleComponent(onToggle, onToggleAttachment, "ONOFF");

    // ================= ASSETS =======================
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    lightOff = juce::ImageCache::getFromMemory(BinaryData::lightOff_png, BinaryData::lightOff_pngSize);
    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    switchTopImage = juce::ImageCache::getFromMemory(BinaryData::switchTop_png, BinaryData::switchTop_pngSize);
        
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (180, 320);
}

PunkMuffEditor::~PunkMuffEditor()
{
}

//==============================================================================
void PunkMuffEditor::paint (juce::Graphics& g)
{
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
        
    // =========== On/Off state ====================
    if (!onToggle.getToggleState()) {
        juce::AffineTransform t;
        t = t.scaled(0.485f);
        t = t.translated(75.5, 163.5);
        g.drawImageTransformed(lightOff, t);
    }
    
    // ========== Parameter knobs angle in radians ==================
    auto sustainRadians = juce::jmap(sustainKnob.getValue(), 0.0, 10.0, -150.0, 150.0) * DEG2RADS;
    auto toneRadians = juce::jmap(toneKnob.getValue(), 0.0, 10.0, -150.0, 150.0) * DEG2RADS;
    auto levelRadians = juce::jmap(levelKnob.getValue(), -18.0, 18.0, -150.0, 150.0) * DEG2RADS;
    
    // ========== Draw parameter knobs ==================
    g.drawImageTransformed(knobImage, knobRotation(sustainRadians, 12.5, 38.0, 0.48));
    g.drawImageTransformed(knobImage, knobRotation(toneRadians, 67.5, 82.0, 0.48));
    g.drawImageTransformed(knobImage, knobRotation(levelRadians, 122.5, 38.0, 0.48));
    
    // ========= Mode switch position  ==================
    switch ((int) modeSwitch.getValue()) {
        case 0:
            g.drawImageTransformed(switchTopImage, knobRotation(0, 74.0, 29.0, 0.5));
            break;
        case 1:
            g.drawImageTransformed(switchTopImage, knobRotation(0, 81.0, 29.0, 0.5));
            break;
        case 2:
            g.drawImageTransformed(switchTopImage, knobRotation(0, 89.0, 29.0, 0.5));
            break;
            
        default:
            break;
    }
}

void PunkMuffEditor::resized()
{
    // Knobs
    sustainKnob.setBounds(13, 38, 46, 46);
    toneKnob.setBounds(68, 82, 46, 46);
    levelKnob.setBounds(123, 38, 46, 46);
    
    // Mode switch
    modeSwitch.setBounds(74, 31, 32, 12);
        
    // OnOff
    onToggle.setBounds(65, 240, 50, 50);
}

void PunkMuffEditor::setSliderComponent(juce::Slider &slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> &sliderAttachment, juce::String paramName, bool linStyle){
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.state, paramName, slider);
    if (linStyle)
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    else
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(slider);
    slider.setAlpha(0);
}

void PunkMuffEditor::setToggleComponent(juce::ToggleButton& button, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& buttonAttachment, juce::String paramName){
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.state, paramName, button);
    addAndMakeVisible(button);
    button.setAlpha(0);
}

juce::AffineTransform PunkMuffEditor::knobRotation(float radians, float posX, float posY, float scaleFactor){
    juce::AffineTransform t;
    t = t.rotated(radians, 46.0f, 46.0f);
    t = t.scaled(scaleFactor);
    t = t.translated(posX, posY);
    return t;
}
