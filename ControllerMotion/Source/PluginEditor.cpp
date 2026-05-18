/*
  ==============================================================================

    PluginEditor.cpp
    Created: 17 May 2026 11:36:01am
    Author:  Nardianian

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

MIDIControllerMotionAudioProcessorEditor::MIDIControllerMotionAudioProcessorEditor (MIDIControllerMotionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configure General Controls
    phraseLengthLabel.setText ("Phrase Length", juce::dontSendNotification);
    addAndMakeVisible (phraseLengthLabel);
    phraseLengthBox.addItemList ({"1 beat", "4 beats", "8 beats", "16 beats", "32 beats", "64 beats"}, 1);
    addAndMakeVisible (phraseLengthBox);
    phraseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.parameters, "phraseBeats", phraseLengthBox);

    firstCCLabel.setText ("First CC Num", juce::dontSendNotification);
    addAndMakeVisible (firstCCLabel);
    firstCCSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    firstCCSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 40, 20);
    addAndMakeVisible (firstCCSlider);
    firstCCAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "firstCCNumber", firstCCSlider);

    channelLabel.setText ("MIDI Channel", juce::dontSendNotification);
    addAndMakeVisible (channelLabel);
    channelSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    channelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 40, 20);
    addAndMakeVisible (channelSlider);
    channelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "channelNumber", channelSlider);

    // Configure the four Slider Targets
    for (int i = 0; i < CBR_CCMOTION_NUM_PARAMS; ++i)
    {
        juce::String id = "target" + juce::String (i + 1);
        targetLabels[i].setText ("Target " + juce::String (i + 1), juce::dontSendNotification);
        addAndMakeVisible (targetLabels[i]);

        targetSliders[i].setSliderStyle (juce::Slider::LinearHorizontal);
        targetSliders[i].setTextBoxStyle (juce::Slider::TextBoxRight, false, 40, 20);
        addAndMakeVisible (targetSliders[i]);

        targetAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            audioProcessor.parameters, id, targetSliders[i]);
    }

    setSize (420, 340);
}

MIDIControllerMotionAudioProcessorEditor::~MIDIControllerMotionAudioProcessorEditor() {}

void MIDIControllerMotionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText ("MIDI Controller Motion", 15, 15, 350, 25, juce::Justification::centredLeft);

    g.setColour (juce::Colours::grey);
    g.setFont (juce::Font (12.0f));
    g.drawText ("Quantized CC Automator & Interpolator", 15, 38, 350, 20, juce::Justification::centredLeft);
    
    // Linea divisoria estetica tra controlli globali e i target
    g.setColour (juce::Colours::darkgrey);
    g.drawHorizontalLine (165, 15, 405);
}

void MIDIControllerMotionAudioProcessorEditor::resized()
{
    auto startY = 70;
    auto rowHeight = 26;
    auto spacing = 5;

    // Global Configuration Section
    phraseLengthLabel.setBounds (15, startY, 120, rowHeight);
    phraseLengthBox.setBounds (140, startY, 120, rowHeight);

    firstCCLabel.setBounds (15, startY + rowHeight + spacing, 120, rowHeight);
    firstCCSlider.setBounds (140, startY + rowHeight + spacing, 260, rowHeight);

    channelLabel.setBounds (15, startY + (rowHeight + spacing) * 2, 120, rowHeight);
    channelSlider.setBounds (140, startY + (rowHeight + spacing) * 2, 260, rowHeight);

    // Target Modulators Section
    auto targetStartY = 185;
    for (int i = 0; i < CBR_CCMOTION_NUM_PARAMS; ++i)
    {
        targetLabels[i].setBounds (15, targetStartY + (rowHeight + spacing) * i, 120, rowHeight);
        targetSliders[i].setBounds (140, targetStartY + (rowHeight + spacing) * i, 260, rowHeight);
    }
}
