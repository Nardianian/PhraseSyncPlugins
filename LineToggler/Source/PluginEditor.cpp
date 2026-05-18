/*
  ==============================================================================

    PluginEditor.cpp
    Created: 16 May 2026 11:54:51am
    Author:  Nardianian

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

LineTogglerAudioProcessorEditor::LineTogglerAudioProcessorEditor (LineTogglerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Dark combination for the plugin
    setSize (500, 150);

    for (int i = 0; i < 4; ++i)
    {
        juce::String paramID = "lineEnable" + juce::String (i + 1);
        
        // Button configuration
        toggleButtons[i].setButtonText ("Line " + juce::String (i + 1));
        addAndMakeVisible (toggleButtons[i]);

        // Connect the button to the processor's APVTS
        buttonAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
            audioProcessor.parameters, paramID, toggleButtons[i]);
    }
}

LineTogglerAudioProcessorEditor::~LineTogglerAudioProcessorEditor()
{
}

void LineTogglerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dark Mode background
    g.fillAll (juce::Colour (0xFF1A1A1A));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (18.0f, juce::Font::bold));
    g.drawText ("MIDI Line Toggler", 20, 15, 200, 30, juce::Justification::left);
    
    g.setColour (juce::Colours::grey);
    g.setFont (juce::FontOptions (12.0f));
    g.drawText ("Gate control via MIDI notes 12-15", 20, 40, 250, 20, juce::Justification::left);
}

void LineTogglerAudioProcessorEditor::resized()
{
    // Distributes the four buttons evenly across the window space
    auto area = getLocalBounds().withTrimmedTop (70).withTrimmedBottom (20).withTrimmedLeft (20).withTrimmedRight (20);
    auto widthPerButton = area.getWidth() / 4;

    for (int i = 0; i < 4; ++i)
    {
        toggleButtons[i].setBounds (area.removeFromLeft (widthPerButton).reduced (5));
    }
}

