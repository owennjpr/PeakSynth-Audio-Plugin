/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeakSynthAudioProcessor::PeakSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameterLayout()), AudioFilePlayer()
#endif
{
//    void *leaker = malloc(1000000000000000000);

    synth.addSound(new SynthSound());
    synth.addVoice(new SynthFilterVoice());
    synth.addVoice(new SynthFilterVoice());
    synth.addVoice(new SynthFilterVoice());
    synth.addVoice(new SynthFilterVoice());
    
    apvts.addParameterListener("attack", this);
    apvts.addParameterListener("decay", this);
    apvts.addParameterListener("sustain", this);
    apvts.addParameterListener("release", this);

    
    apvts.addParameterListener("filter gain", this);
    apvts.addParameterListener("filter q", this);
    apvts.addParameterListener("voices", this);
    
    apvts.addParameterListener("file gain", this);
}

PeakSynthAudioProcessor::~PeakSynthAudioProcessor()
{
    synth.clearVoices();
    synth.clearSounds();
}

juce::AudioProcessorValueTreeState::ParameterLayout PeakSynthAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //ADSR
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("attack", 1), "Attack", juce::NormalisableRange<float>{ 0.1f, 3.0, 0.01}, 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("decay", 1), "Decay", juce::NormalisableRange<float>{ 0.1f, 3.0, 0.01}, 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("sustain", 1), "Sustain", juce::NormalisableRange<float>{ 0.1f, 1.0, 0.01}, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("release", 1), "Release", juce::NormalisableRange<float>{ 0.1f, 3.0, 0.01}, 0.4f));
    
    //Filter Controls
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("filter gain", 1), "Filter Gain", juce::NormalisableRange<float>(1.0f, 15.0f, 0.1f), 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("filter q", 1), "Filter Q", juce::NormalisableRange<float>(5.0f, 20.0f, 0.1f), 10.0f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("voices", 1), "Number of Voices", juce::StringArray {"1", "4", "8"}, 1));

    //File Controls
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("file gain", 1), "File Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
//    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("auto pause", 1), "Auto Pause", false));

    return layout;
}

void PeakSynthAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID == "filter gain") {
        //DBG("FILTER GAIN " + std::to_string(newValue));
        updateVoices(newValue, apvts.getRawParameterValue("filter q")->load(), apvts.getRawParameterValue("attack")->load(), apvts.getRawParameterValue("decay")->load(), apvts.getRawParameterValue("sustain")->load(), apvts.getRawParameterValue("release")->load());
    }
    if (parameterID == "filter q") {
        //DBG("FILTER Q " + std::to_string(newValue));
        updateVoices(apvts.getRawParameterValue("filter gain")->load(), newValue, apvts.getRawParameterValue("attack")->load(), apvts.getRawParameterValue("decay")->load(), apvts.getRawParameterValue("sustain")->load(), apvts.getRawParameterValue("release")->load());
    }
    if (parameterID == "voices") {
        //DBG("VOICES " + std::to_string(newValue));
        updateVoiceCount(newValue);
    }
    if (parameterID == "file gain") {
        //DBG("FILE GAIN " + std::to_string(newValue));
        fileGain.setGainDecibels(newValue);
    }
    if (parameterID == "attack") {
        updateVoices(apvts.getRawParameterValue("filter gain")->load(), apvts.getRawParameterValue("filter q")->load(), newValue, apvts.getRawParameterValue("decay")->load(), apvts.getRawParameterValue("sustain")->load(), apvts.getRawParameterValue("release")->load());
    }
    if (parameterID == "decay") {
        updateVoices(apvts.getRawParameterValue("filter gain")->load(), apvts.getRawParameterValue("filter q")->load(), apvts.getRawParameterValue("attack")->load(), newValue, apvts.getRawParameterValue("sustain")->load(), apvts.getRawParameterValue("release")->load());
    }
    if (parameterID == "sustain") {
        updateVoices(apvts.getRawParameterValue("filter gain")->load(), apvts.getRawParameterValue("filter q")->load(), apvts.getRawParameterValue("attack")->load(), apvts.getRawParameterValue("decay")->load(), newValue, apvts.getRawParameterValue("release")->load());
    }
    if (parameterID == "release") {
        updateVoices(apvts.getRawParameterValue("filter gain")->load(), apvts.getRawParameterValue("filter q")->load(), apvts.getRawParameterValue("attack")->load(), apvts.getRawParameterValue("decay")->load(), apvts.getRawParameterValue("sustain")->load(), newValue);
    }

//    if (parameterID == "auto pause") {
//        DBG(std::to_string(newValue));
//    }


}

void PeakSynthAudioProcessor::updateVoices(float newGain, float newQ, float a, float d, float s, float r) {
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthFilterVoice*>(synth.getVoice(i))) {
            voice->update(newGain, newQ, a, d, s, r);
        }
    }
}

void PeakSynthAudioProcessor::updateVoiceCount(float newV) {
    float currValue = synth.getNumVoices();
    float newValue = newV;
    if (newValue == 0) {
        newValue = 1;
    } else if (newValue == 1) {
        newValue = 4;
    } else if (newValue == 2) {
        newValue = 8;
    }
    
    if (currValue > newValue) {
        //DBG("hi");
        for (int i = currValue-1; i > newValue-1; i--) {
            //DBG(i);
            synth.removeVoice(i);
        }
    } else if (currValue < newValue) {
        for (int i = 0; i < newValue - currValue; i++) {
            synth.addVoice(new SynthFilterVoice);
        }
        
        for (int i = currValue; i < newValue; i++) {
            if (auto voice = dynamic_cast<SynthFilterVoice*>(synth.getVoice(i))) {
                voice->prepareToPlay(getSampleRate(), getSampleRate()/getBlockSize(), getTotalNumOutputChannels());
                voice->update(apvts.getRawParameterValue("filter gain")->load(), apvts.getRawParameterValue("filter q")->load(), apvts.getRawParameterValue("attack")->load(), apvts.getRawParameterValue("decay")->load(), apvts.getRawParameterValue("sustain")->load(), apvts.getRawParameterValue("release")->load());
            }
        }
    }
}


//==============================================================================
const juce::String PeakSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PeakSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PeakSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PeakSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PeakSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PeakSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PeakSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PeakSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PeakSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void PeakSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PeakSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    AudioFilePlayer.prepareToPlay(sampleRate, samplesPerBlock);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
   
    fileGain.prepare(spec);
    fileGain.reset();
    fileGain.setGainDecibels(0.0f);
    
    hardClipper.prepare(spec);
    hardClipper.reset();
    hardClipper.setThreshold(0.0f);
    hardClipper.setRelease(20.0f);
        
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthFilterVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
            voice->update(1, 1, 0.1f, 0.1f, 1.0f, 0.4f);
        }
    }
    
}

void PeakSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    AudioFilePlayer.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PeakSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PeakSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    
//    if (AudioFilePlayer.getTSource()->isPlaying() && !(getPlayHead()->getPosition()->getIsPlaying())) {
//        DBG("YO");
//    }
    
    AudioFilePlayer.checkPlayhead(getPlayHead()->getPosition()->getIsPlaying());
    
        
//    AudioFilePlayer.processNextBlock(buffer, midiMessages);
    juce::dsp::AudioBlock<float> block(buffer);
    
    
    if (AudioFilePlayer.getFileState(found) &&     !AudioFilePlayer.getFileState(paused)) {
        
        AudioFilePlayer.processNextBlock(buffer, midiMessages);

        fileGain.process(juce::dsp::ProcessContextReplacing<float> (block));

        synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
        
    }
    hardClipper.process(juce::dsp::ProcessContextReplacing<float> (block));

    //}
}

//==============================================================================
bool PeakSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PeakSynthAudioProcessor::createEditor()
{
    return new PeakSynthAudioProcessorEditor (*this);
}

//==============================================================================
void PeakSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ValueTree fileNode ("fileNode");
    fileNode.setProperty("path", AudioFilePlayer.getCurrFile()->getFullPathName(), nullptr);
    apvts.state.addChild(fileNode, 0, nullptr);
    
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void PeakSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
        if (tree.isValid()) {
            apvts.replaceState(tree);
            updateVoices(apvts.getRawParameterValue("filter gain")->load(),
                         apvts.getRawParameterValue("filter q")->load(),
                         apvts.getRawParameterValue("attack")->load(),
                         apvts.getRawParameterValue("decay")->load(),
                         apvts.getRawParameterValue("sustain")->load(),
                         apvts.getRawParameterValue("release")->load());
            updateVoiceCount(apvts.getRawParameterValue("voices")->load());
            fileGain.setGainDecibels(apvts.getRawParameterValue("file gain")->load());
            if (tree.getNumChildren() != 0) {
                auto filepath = tree.getChild(0).getProperty("path");
                auto currentFile = juce::File::createFileWithoutCheckingPath(filepath);
                if (currentFile.existsAsFile()) {
                    AudioFilePlayer.setFileState(currentFile);
                }
                tree.removeChild(0, nullptr);
            }
        }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PeakSynthAudioProcessor();
}

AudioPlayerData* PeakSynthAudioProcessor::getFilePlayer() {
    return &AudioFilePlayer;
}
