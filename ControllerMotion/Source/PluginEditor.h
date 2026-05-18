/*
  ==============================================================================

    PluginEditor.h
    Created: 17 May 2026 11:32:35am
    Author:  Nardianian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MIDIControllerMotionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MIDIControllerMotionAudioProcessorEditor (MIDIControllerMotionAudioProcessor&);
    ~MIDIControllerMotionAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MIDIControllerMotionAudioProcessor& audioProcessor;

    // Main GUI elements
    juce::ComboBox phraseLengthBox;
    juce::Slider firstCCSlider;
    juce::Slider channelSlider;

    juce::Slider targetSliders[CBR_CCMOTION_NUM_PARAMS];

    // Labels
    juce::Label phraseLengthLabel;
    juce::Label firstCCLabel;
    juce::Label channelLabel;
    juce::Label targetLabels[CBR_CCMOTION_NUM_PARAMS];

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> phraseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> firstCCAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> targetAttachments[CBR_CCMOTION_NUM_PARAMS];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIControllerMotionAudioProcessorEditor)
};
