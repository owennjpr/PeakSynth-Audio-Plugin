/*
  ==============================================================================

    AudioPlayerData.cpp
    Created: 25 Jun 2023 10:58:11am
    Author:  Owen Prendergast

  ==============================================================================
*/

#include "AudioPlayerData.h"

AudioPlayerData::AudioPlayerData() : thumbnailCache(3), thumbnail(512, formatManager, thumbnailCache) {
//    DBG("Constructing!!!!!!!!");
    formatManager.registerBasicFormats();
    fileFound = false;
    filePaused = false;

}

AudioPlayerData::~AudioPlayerData() {
    transportSource.stop();
    transportSource.setSource(nullptr);
//    DBG("Deconstructing!!!!!!!!");

}



void AudioPlayerData::prepareToPlay(double sampleRate, int samplesPerBlock) {
//    DBG("preparing!!!!!!!!");
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
    //transportSource.stop();

}

void AudioPlayerData::releaseResources() {
//    DBG("releasing!!!!!!!!");
    transportSource.releaseResources();
}

void AudioPlayerData::processNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (readerSource.get() == nullptr) {
        //DBG("BLEK");
        buffer.clear(0, buffer.getNumSamples());
        //DBG("Should be first");
        return;
        
    }

    if (fileFound && transportSource.hasStreamFinished()) {
        transportSource.stop();
        transportSource.setPosition(0.0);
        transportSource.start();
//        DBG("LOOPING");
    }

//    if (!filePaused) {
//        DBG("hi");
        transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
//    }
    
    
}

void AudioPlayerData::getNewFile()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    fileFound = false;
    currFile = juce::File();
    
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
            } else {
            }
        }
    });
}

void AudioPlayerData::setFileState(juce::File newFile) {
    transportSource.stop();
    transportSource.setSource(nullptr);
    fileFound = false;
    filePaused = true;

    auto* reader = formatManager.createReaderFor(newFile);
    if (reader != nullptr) {
        currFile = newFile;
        jassert(currFile != juce::File());
        //                DBG("GOT A WAV");
        fileFound = true;
        auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        thumbnail.setSource(new juce::FileInputSource (newFile));
        readerSource.reset (newSource.release());
    }
}

void AudioPlayerData::checkPlayhead(bool isPlaying) {
    
    if (pauseWithDAW && fileFound) {
        if (isPlaying && filePaused) {
            filePaused = false;
            transportSource.start();
//            DBG("Play");
        } else if (!isPlaying && !filePaused){
            filePaused = true;
            transportSource.stop();
//            DBG("Pause");
            
        }
    }
}


void AudioPlayerData::pauseOrPlay() {
    if (filePaused) {
        transportSource.start();
        filePaused = false;
    } else {
        transportSource.stop();
        filePaused = true;
    }
}

bool AudioPlayerData::getFileState(int index) {
    
    
    if (index == 0) {
        return fileFound;
    } else if (index == 1) {
        return filePaused;
    } else {
        jassert(false);
        return false;
    }
}

juce::AudioThumbnail* AudioPlayerData::getThumbnail() {
    return &thumbnail;
}

juce::AudioTransportSource* AudioPlayerData::getTSource() {
    return &transportSource;
}

juce::File* AudioPlayerData::getCurrFile() {
    return &currFile;
}
