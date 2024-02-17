#pragma once

// #include "BinaryData.h"
#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // Add the sliderValueChanged override here
    void sliderValueChanged (juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    // std::unique_ptr<melatonin::Inspector> inspector;
    // juce::TextButton inspectButton { "Inspect the UI" };

    juce::Slider distortionAmountSlider;
    juce::Label distortionAmountLabel;

    juce::Slider filtBlendSlider;
    juce::Label filtBlendLabel;

    juce::Slider compBlendSlider;
    juce::Label compBlendLabel;

    juce::Slider filtCompSlider;
    juce::Label filtCompLabel;

    juce::Slider dirtyReverbSlider;
    juce::Label dirtyReverbLabel;

    juce::Slider lowOomphSlider;
    juce::Label lowOomphLabel;

    juce::Slider lightCompSlider;
    juce::Label lightCompLabel;

    juce::Slider medCompSlider;
    juce::Label medCompLabel;

    juce::Slider outputLevel;
    juce::Label outputLevelLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distortionAmountSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filtBlendSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compBlendSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filtCompSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dirtyReverbSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowOomphSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lightCompSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> medCompSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputLevelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
