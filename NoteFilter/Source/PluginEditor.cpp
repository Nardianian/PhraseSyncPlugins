/*
  ==============================================================================

    PluginEditor.cpp
    Created: 17 May 2026 12:19:15am
    Author:  Nardianian

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

MIDIClipVariationsAudioProcessorEditor::MIDIClipVariationsAudioProcessorEditor (MIDIClipVariationsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // 1. Variation Slider
    variationLabel.setText ("Variation", juce::dontSendNotification);
    variationLabel.setFont (juce::Font (14.0f));
    addAndMakeVisible (variationLabel);

    variationSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    variationSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 40, 20);
    addAndMakeVisible (variationSlider);

    // 2. Phrase Length menu
    phraseLengthLabel.setText ("Phrase Length", juce::dontSendNotification);
    phraseLengthLabel.setFont (juce::Font (14.0f));
    addAndMakeVisible (phraseLengthLabel);

    phraseLengthBox.addItemList ({"1 beat", "4 beats", "8 beats", "16 beats", "32 beats", "64 beats"}, 1);
    addAndMakeVisible (phraseLengthBox);

    // 3. Height Variation menu
    variationHeightLabel.setText ("Var. Height", juce::dontSendNotification);
    variationHeightLabel.setFont (juce::Font (14.0f));
    addAndMakeVisible (variationHeightLabel);

    variationHeightBox.addItemList ({"6 semitones", "1 octave", "2 octaves", "3 octaves"}, 1);
    addAndMakeVisible (variationHeightBox);

    // APVTS Connections
    variationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "variation", variationSlider);

    phraseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.parameters, "phraseBeats", phraseLengthBox);

    heightAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.parameters, "notesPerVariation", variationHeightBox);

    setSize (350, 210); // Slightly taller to accommodate the third control
}

MIDIClipVariationsAudioProcessorEditor::~MIDIClipVariationsAudioProcessorEditor() {}

void MIDIClipVariationsAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText ("MIDI Note Filter", 15, 15, 300, 25, juce::Justification::centredLeft);

    g.setColour (juce::Colours::grey);
    g.setFont (juce::Font (12.0f));
    g.drawText ("Octave Windows / Phrase Quantized", 15, 38, 300, 20, juce::Justification::centredLeft);
}

void MIDIClipVariationsAudioProcessorEditor::resized()
{
    auto startY = 75;
    auto rowHeight = 30;
    auto spacing = 12;

    variationLabel.setBounds (15, startY, 100, rowHeight);
    variationSlider.setBounds (120, startY, 210, rowHeight);

    phraseLengthLabel.setBounds (15, startY + rowHeight + spacing, 100, rowHeight);
    phraseLengthBox.setBounds (120, startY + rowHeight + spacing, 140, rowHeight);

    variationHeightLabel.setBounds (15, startY + (rowHeight + spacing) * 2, 100, rowHeight);
    variationHeightBox.setBounds (120, startY + (rowHeight + spacing) * 2, 140, rowHeight);
}
