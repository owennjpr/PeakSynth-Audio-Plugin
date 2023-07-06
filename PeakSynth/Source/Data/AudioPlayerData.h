/*
  ==============================================================================

    AudioPlayerData.h
    Created: 25 Jun 2023 10:58:11am
    Author:  Owen Prendergast

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioPlayerData
{
public:
    AudioPlayerData();
    ~AudioPlayerData();
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void getNewFile();
    void pauseOrPlay();
    

    bool getFileState(int index);
    juce::AudioThumbnail* getThumbnail();
    juce::AudioTransportSource* getTSource();
    juce::File* getCurrFile();
    void setFileState(juce::File newFile);
    void checkPlayhead(bool isPlaying);

    bool pauseWithDAW { false };

private:
    enum stateIndex { found, paused };

    std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::AudioFormatManager formatManager;
    
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;

    juce::File currFile;
    
    bool fileFound;
    bool filePaused;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayerData);
};
