/*
  ==============================================================================

    PluginEditor.cpp
    Created: 16 May 2026 10:57:14pm
    Author:  Nardianian

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

MIDIClipVariationsAudioProcessorEditor::MIDIClipVariationsAudioProcessorEditor (MIDIClipVariationsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // 1. Configure Channel Label & Slider
    channelLabel.setText ("Channel", juce::dontSendNotification);
    channelLabel.setFont (juce::Font (14.0f));
    channelLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (channelLabel);

    channelSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    channelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 40, 20);
    addAndMakeVisible (channelSlider);

    // 2. Configure Phrase Lenght Label & ComboBox
    phraseLengthLabel.setText ("Phrase Length", juce::dontSendNotification);
    phraseLengthLabel.setFont (juce::Font (14.0f));
    phraseLengthLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (phraseLengthLabel);

    // We fill the menu with the same entries as the processor
    phraseLengthBox.addItemList ({"1 beat", "4 beats", "8 beats", "16 beats", "32 beats", "64 beats"}, 1);
    addAndMakeVisible (phraseLengthBox);

    // 3. Connect components to processor parameters (APVTS)
    channelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "channel", channelSlider);

    phraseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.parameters, "phraseBeats", phraseLengthBox);

    // Sets the rectangle to a fixed size, identical to Line Toggler
    setSize (350, 180);
}

MIDIClipVariationsAudioProcessorEditor::~MIDIClipVariationsAudioProcessorEditor()
{
}

void MIDIClipVariationsAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Black background, clean and minimal
    g.fillAll (juce::Colours::black);

    // Headlines at the top
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText("MIDI Channel Filter", 15, 15, 300, 25, juce::Justification::centredLeft);

    g.setColour(juce::Colours::grey);
    g.setFont(juce::Font(12.0f));
    g.drawText("Clip Variations / Phrase Quantized", 15, 38, 300, 20, juce::Justification::centredLeft);
}  

void MIDIClipVariationsAudioProcessorEditor::resized()
{
    // We organize the components in the window space
    auto startY = 75;
    auto rowHeight = 30;
    auto spacing = 15;

    // Line 1: Channel (Label + Slider)
    channelLabel.setBounds (15, startY, 100, rowHeight);
    channelSlider.setBounds (120, startY, 210, rowHeight);

    // Line 2: Sentence Length (Label + ComboBox)
    phraseLengthLabel.setBounds (15, startY + rowHeight + spacing, 100, rowHeight);
    phraseLengthBox.setBounds (120, startY + rowHeight + spacing, 120, rowHeight);
}
