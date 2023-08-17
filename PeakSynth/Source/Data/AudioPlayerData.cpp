/*
  ==============================================================================

    AudioPlayerData.cpp
    Created: 25 Jun 2023 10:58:11am
    Author:  Owen Prendergast
    Description: Handles the audio file selection and state management, as well as
                 the playing of that file through the transport source

  ==============================================================================
*/

#include "AudioPlayerData.h"

AudioPlayerData::AudioPlayerData() : thumbnailCache(3), thumbnail(512, formatManager, thumbnailCache)
{
    //registering  audio formats to use
    formatManager.registerBasicFormats();
    fileFound = false;
    filePaused = false;

}

AudioPlayerData::~AudioPlayerData()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
}



void AudioPlayerData::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void AudioPlayerData::releaseResources()
{
    transportSource.releaseResources();
}

void AudioPlayerData::processNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (readerSource.get() == nullptr) {
        buffer.clear(0, buffer.getNumSamples());
        return;
    }

    // loop the file if the end has been reached
    if (fileFound && transportSource.hasStreamFinished()) {
        transportSource.stop();
        transportSource.setPosition(0.0);
        transportSource.start();
    }

    // process the audio
    transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
}

/* handles the process of creating a file chooser object to get a new audio file from the user's computer */
void AudioPlayerData::getNewFile()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    fileFound = false;
    currFile = juce::File();
    
    // initializing a file chooser
    chooser = std::make_unique<juce::FileChooser> ("Select a .wav, .aif, or .aiff file", juce::File{}, "*.wav;*.aif;*.aiff");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();
        if (file != juce::File()) {
            auto* reader = formatManager.createReaderFor(file);
            
            if (reader != nullptr) {
                currFile = file;
                jassert(currFile != juce::File());
                fileFound = true;
                
                auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                thumbnail.setSource(new juce::FileInputSource (file));
                readerSource.reset (newSource.release());
                
                transportSource.start();
                filePaused = false;
            }
        }
    });
}

/* updates the current state of the audio file when re opening the plugin*/
void AudioPlayerData::setFileState(juce::File newFile)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    fileFound = false;
    filePaused = true;

    auto* reader = formatManager.createReaderFor(newFile);
    if (reader != nullptr) {
        currFile = newFile;
        jassert(currFile != juce::File());
        fileFound = true;
        auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        thumbnail.setSource(new juce::FileInputSource (newFile));
        readerSource.reset (newSource.release());
    }
}

/* determining whether to pause or play based on the playhead's playing status */
void AudioPlayerData::checkPlayhead(bool isPlaying, bool *shouldPlay)
{
    if (pauseWithDAW && fileFound) {
        *shouldPlay = isPlaying;
        if (isPlaying && filePaused) {
            filePaused = false;
            transportSource.start();
        } else if (!isPlaying && !filePaused){
            filePaused = true;
            transportSource.stop();
        }
    }
}

/* handling pause/play button pressing */
void AudioPlayerData::pauseOrPlay()
{
    if (filePaused) {
        transportSource.start();
        filePaused = false;
    } else {
        transportSource.stop();
        filePaused = true;
    }
}

/* public function for other objects to access important state information about this class */
bool AudioPlayerData::getFileState(int index)
{
    if (index == 0) {
        return fileFound;
    } else if (index == 1) {
        return filePaused;
    } else {
        jassert(false);
        return false;
    }
}

/* functions for giving private data to other objects */

juce::AudioThumbnail* AudioPlayerData::getThumbnail()
{
    return &thumbnail;
}

juce::AudioTransportSource* AudioPlayerData::getTSource()
{
    return &transportSource;
}

juce::File* AudioPlayerData::getCurrFile()
{
    return &currFile;
}
