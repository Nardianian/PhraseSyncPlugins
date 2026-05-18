/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const int LineTogglerAudioProcessor::notesPerLine[] = { 2, 2, 4, 4 };

// 2 octaves below first line note (36). In future might shift to MIDI zero.
const int LineTogglerAudioProcessor::lineControlNotes[] = { 12, 13, 14, 15 };

//==============================================================================
LineTogglerAudioProcessor::LineTogglerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    :
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        parameters (*this, nullptr, juce::Identifier (JucePlugin_Name),
            {
            std::make_unique<juce::AudioParameterBool> (
                "lineEnable1", // parameterID
                "Enable line 1", // parameter name
                true
            ),
            std::make_unique<juce::AudioParameterBool> (
                "lineEnable2", // parameterID
                "Enable line 2", // parameter name
                true
            ),
            std::make_unique<juce::AudioParameterBool> (
                "lineEnable3", // parameterID
                "Enable line 3", // parameter name
                true
            ),
            std::make_unique<juce::AudioParameterBool> (
                "lineEnable4", // parameterID
                "Enable line 4", // parameter name
                true
            ),

            } )
#endif
{
    for (int i=0; i<CBR_TOGGLELINES_NUM_LINES; i++) {
        int lineNumber = i + 1;

        std::ostringstream paramIdentifier;
        paramIdentifier << "lineEnable" << lineNumber;

        allowLinePlayback[i] = (juce::AudioParameterBool*)parameters.getParameter(paramIdentifier.str());

        lineGate[i] = true;
    }

}

LineTogglerAudioProcessor::~LineTogglerAudioProcessor()
{
}

//==============================================================================
const juce::String LineTogglerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LineTogglerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LineTogglerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LineTogglerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LineTogglerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LineTogglerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LineTogglerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LineTogglerAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String LineTogglerAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void LineTogglerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void LineTogglerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void LineTogglerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LineTogglerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Come Synth, non abbiamo ingressi audio ma solo uscite stereo o mono
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}
#endif

/**
 * Determine if a MIDI note is a synth note for a line.
 *
 * @param midiNoteNumber The MIDI note number to check.
 * @return The slot index for the note, or -1 if the note is not in a slot.
*/
int LineTogglerAudioProcessor::getSlotIndexForNote(const int midiNoteNumber) {
    int slotStartNote = CBR_TOGGLELINES_FIRST_MIDI_NOTE;

    // Iterate over each line, and check if the note is in the line's range.
    for (int i = 0; i < CBR_TOGGLELINES_NUM_LINES; i++)
    {
        int nextSlotStart = slotStartNote + notesPerLine[i];
        if ( midiNoteNumber >= slotStartNote && midiNoteNumber < nextSlotStart ) {
            return  i;
        }
        slotStartNote = nextSlotStart;
    }

    // Not in a slot!
    return -1;
}

/**
 * Determine if a MIDI note is a control note for a line.
 *
 * @param midiNoteNumber The MIDI note number to check.
 * @return The slot index that the note controls, or -1.
*/
int LineTogglerAudioProcessor::getSlotIndexForControlNote(const int midiNoteNumber) {
    for (int i = 0; i < CBR_TOGGLELINES_NUM_LINES; i++)
    {
        if ( midiNoteNumber == lineControlNotes[i] ) {
            return  i;
        }
    }

    return -1;
}

const juce::MidiMessageMetadata* findNoteOnEvent(
    juce::MidiBuffer& midiMessages,
    int afterTimestamp,
    int midiNoteValue
) {

    for (const juce::MidiMessageMetadata metadata : midiMessages) {
       const juce:: MidiMessage event = metadata.getMessage();
        if (event.isNoteOn()) {
            if (event.getNoteNumber() == midiNoteValue &&
                metadata.samplePosition >= afterTimestamp) {

                // THIS IS A POINTER – we should avoid this and use a reference or a copy. (Compile warning)
                return &metadata;
            }
        }
    }

    return nullptr;
}

void LineTogglerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(buffer);

    outputMidiBuffer.clear();

    // 1. Controlla prima i cambi di parametri o eventi di controllo immediati all'inizio del blocco
    // Aggiorniamo lo stato dei gate in base ai messaggi MIDI in ingresso in questo blocco
    for (const auto metadata : midiMessages)
    {
        auto m = metadata.getMessage();
        if (m.isNoteOn())
        {
            int ctrlSlot = getSlotIndexForControlNote(m.getNoteNumber());
            if (ctrlSlot != -1)
            {
                // Quando arriva la nota di controllo, il gate copia lo stato del parametro GUI
                lineGate[ctrlSlot] = allowLinePlayback[ctrlSlot]->get();
            }
        }
    }

    // 2. Filtra i messaggi MIDI
    for (const auto metadata : midiMessages)
    {
        auto m = metadata.getMessage();
        int samplePos = metadata.samplePosition;

        if (m.isNoteOn())
        {
            int slot = getSlotIndexForNote(m.getNoteNumber());
            int ctrlSlot = getSlotIndexForControlNote(m.getNoteNumber());

            if (slot != -1)
            {
                // È una nota musicale appartenente a una linea: passa solo se il gate è aperto
                if (lineGate[slot])
                {
                    outputMidiBuffer.addEvent(m, samplePos);
                }
            }
            else if (ctrlSlot != -1)
            {
                // È una nota di controllo: la consumiamo qui (non la mandiamo all'output)
                // per evitare che suoni nel synth a valle, come da tuo commento (bonus)
            }
            else
            {
                // Note musicali fuori dal range del plugin: passano sempre
                outputMidiBuffer.addEvent(m, samplePos);
            }
        }
        else if (m.isNoteOff())
        {
            int slot = getSlotIndexForNote(m.getNoteNumber());
            int ctrlSlot = getSlotIndexForControlNote(m.getNoteNumber());

            if (slot != -1)
            {
                // NOTA OFF CRITICA: Passa SEMPRE i Note Off per evitare note bloccate,
                // anche se il gate nel frattempo si è chiuso!
                outputMidiBuffer.addEvent(m, samplePos);
            }
            else if (ctrlSlot != -1)
            {
                // Consumiamo il Note Off della nota di controllo
            }
            else
            {
                outputMidiBuffer.addEvent(m, samplePos);
            }
        }
        else
        {
            // Pass-through per CC, Pitch Bend, Aftertouch, ecc.
            outputMidiBuffer.addEvent(m, samplePos);
        }
    }

    midiMessages.swapWith(outputMidiBuffer);
}

//==============================================================================
bool LineTogglerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* LineTogglerAudioProcessor::createEditor()
{
    return new ::LineTogglerAudioProcessorEditor (*this);
}

//==============================================================================
void LineTogglerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LineTogglerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LineTogglerAudioProcessor();
}

