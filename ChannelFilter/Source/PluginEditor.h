/*
  ==============================================================================

    PluginEditor.h
    Created: 16 May 2026 10:56:03pm
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

    // GUI Elements
    juce::Slider channelSlider;
    juce::ComboBox phraseLengthBox;

    juce::Label channelLabel;
    juce::Label phraseLengthLabel;

    // Attachment per collegare la GUI all'APVTS del Processor
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> phraseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIClipVariationsAudioProcessorEditor)
};
