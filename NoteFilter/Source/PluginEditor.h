/*
  ==============================================================================

    PluginEditor.h
    Created: 17 May 2026 12:18:22am
    Author:  Nardianian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MIDIClipVariationsAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MIDIClipVariationsAudioProcessorEditor (MIDIClipVariationsAudioProcessor&);
    ~MIDIClipVariationsAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MIDIClipVariationsAudioProcessor& audioProcessor;

    juce::Slider variationSlider;
    juce::ComboBox phraseLengthBox;
    juce::ComboBox variationHeightBox;

    juce::Label variationLabel;
    juce::Label phraseLengthLabel;
    juce::Label variationHeightLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> variationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> phraseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> heightAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIClipVariationsAudioProcessorEditor)
};