/*
  ==============================================================================

    PluginEditor.cpp
    Created: 16 May 2026 10:57:14pm
    Author:  Eugenio

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

MIDIClipVariationsAudioProcessorEditor::MIDIClipVariationsAudioProcessorEditor (MIDIClipVariationsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // 1. Configura Label e Slider per il Canale
    channelLabel.setText ("Channel", juce::dontSendNotification);
    channelLabel.setFont (juce::Font (14.0f));
    channelLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (channelLabel);

    channelSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    channelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 40, 20);
    addAndMakeVisible (channelSlider);

    // 2. Configura Label e ComboBox per la lunghezza della frase
    phraseLengthLabel.setText ("Phrase Length", juce::dontSendNotification);
    phraseLengthLabel.setFont (juce::Font (14.0f));
    phraseLengthLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (phraseLengthLabel);

    // Riempiamo il menu con le stesse voci del processore
    phraseLengthBox.addItemList ({"1 beat", "4 beats", "8 beats", "16 beats", "32 beats", "64 beats"}, 1);
    addAndMakeVisible (phraseLengthBox);

    // 3. Collega i componenti ai parametri del processore (APVTS)
    channelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "channel", channelSlider);

    phraseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.parameters, "phraseBeats", phraseLengthBox);

    // Imposta le dimensioni fisse del rettangolo, identiche a Line Toggler
    setSize (350, 180);
}

MIDIClipVariationsAudioProcessorEditor::~MIDIClipVariationsAudioProcessorEditor()
{
}

void MIDIClipVariationsAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Sfondo nero, pulito e minimale
    g.fillAll (juce::Colours::black);

    // Titoli in alto
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText("MIDI Channel Filter", 15, 15, 300, 25, juce::Justification::centredLeft);

    g.setColour(juce::Colours::grey);
    g.setFont(juce::Font(12.0f));
    g.drawText("Clip Variations / Phrase Quantized", 15, 38, 300, 20, juce::Justification::centredLeft);
}  

void MIDIClipVariationsAudioProcessorEditor::resized()
{
    // Organizziamo i componenti nello spazio della finestra
    auto startY = 75;
    auto rowHeight = 30;
    auto spacing = 15;

    // Riga 1: Canale (Label + Slider)
    channelLabel.setBounds (15, startY, 100, rowHeight);
    channelSlider.setBounds (120, startY, 210, rowHeight);

    // Riga 2: Lunghezza Frase (Label + ComboBox)
    phraseLengthLabel.setBounds (15, startY + rowHeight + spacing, 100, rowHeight);
    phraseLengthBox.setBounds (120, startY + rowHeight + spacing, 120, rowHeight);
}
