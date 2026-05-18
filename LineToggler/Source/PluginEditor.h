/*
  ==============================================================================

    PluginEditor.h
    Created: 16 May 2026 11:53:05am
    Author:  Nardianian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class LineTogglerAudioProcessor;

class LineTogglerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LineTogglerAudioProcessorEditor (LineTogglerAudioProcessor&);
    ~LineTogglerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    LineTogglerAudioProcessor& audioProcessor;

    // Four buttons to activate/deactivate the lines
    juce::ToggleButton toggleButtons[4];
    
    // Attachments that connect the buttons to the processor parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments[4];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LineTogglerAudioProcessorEditor)
};
