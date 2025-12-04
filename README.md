# PeakSynth Audio Plugin

<img width="900" alt="Screenshot 2023-07-08 at 3 44 10 PM" src="https://github.com/owennjpr/JucePeakSynth/assets/123135948/d4031d3a-0dfd-43bf-8c78-fa4c92a39e17">

video demo link: https://drive.google.com/file/d/1MyZLQpser3z00eg5mtKh2x4wodKL4FYz/view?usp=sharing

## What this Plugin Does

PeakSynth is a mix between a synthesizer and a sampler that instead of creating sound from scratch, uses IIR filtering to "play" an audio file with midi. User Midi input is converted into a series of peak filters at frequencies corresponding to the notes being played. Depending on the character of the audio file being used, this can lead to lots of interesting outcomes. While this plugin is intended to be used for the most part with atonal audio like percussion or ambient noise, (rain sounds, footsteps, wind, etc.) but any kind of audio input can generate some interesting results. This plugin is early in development and therefore may have some bugs or issues. If you have any issues with its use or suggestions for potential plugin features, feel free to email me at owenprendergast8@gmail.com.

## Demos

Here are a couple examples of what this plugin can sound like. The first is a series of notes played with PeakSynth over a recording of ambient sounds outside. The second is a chord progression played over vinyl dust and scratching sounds generated in the iZotope Vinyl plugin.


https://github.com/owennjpr/PeakSynth-Audio-Plugin/assets/123135948/ed55c5b7-b28d-4285-b7e5-9677def9c8ba


https://github.com/owennjpr/PeakSynth-Audio-Plugin/assets/123135948/5ab5ae59-a3a8-486a-9c42-284c435fb824


## How to Use this Plugin

The File Player Module:

For this plugin to function you need to open a .wav, .aif, or aiff file for the audio player module. Once a file is selected it will loop repeatedly. The volume of the audio is controlled in decibels from -12 to 12 by the slider in this module, and the playback is controlled by the pause/play button. Additionally, the "Pause with DAW" button when toggled on will override the normal pause/play functionality, instead matching the playback state to the playhead within your DAW.

The Synth Module:

Once a file is playing this is the module that allows you to use midi input to control the tonal nature of the sound. The gain slider controls the maximum gain level of the peak filter whenever you play a note. The Q slider controls the Q of the filter, with higher values creating a thinner peak. the Attack, Decay, Sustain, and Release knobs are standard ADSR controls which control the time it takes for the filter to reach its maximum gain and how long it takes to tail off in seconds. There is also a number of voices dropdown which allows you to determine whether the plugin is monophonic or 4-8 note polyphonic. 

## How to Download this Plugin

You can download the plugin as VST3, AU, or LV2 from the release builds folder or through [Google Drive](https://drive.google.com/drive/folders/1ghIPKx0kRZkyaEUP4X4Qwysitcfxb0YX?usp=sharing)

Follow the link and select which version of the plugin you would like to use. You will need to manually move this plugin to the correct VST3 or Components folder on your device for it to appear in your DAW.


## Resources Used For Reference

Official Juce Resources:

[JUCE Class Index](https://docs.juce.com/master/index.html)

[JUCE Tutorial: Building an Audio Player](https://docs.juce.com/master/tutorial_playing_sound_files.html)

[JUCE Tutorial: Draw Audio Waveforms](https://docs.juce.com/master/tutorial_audio_thumbnail.html)

[JUCE Tutorial: Customize the Look and Feel of Your App](https://docs.juce.com/master/tutorial_look_and_feel_customisation.html)


Other Resources:

[JUCE Step by Step](https://jucestepbystep.wordpress.com/)

[Learn Modern C++ by Building an Audio Plugin (w/ JUCE Framework) - Full Course](https://www.youtube.com/watch?v=i_Iq4_Kd7Rc)

[Let's Build a Synth VST Plug-in with JUCE (2020)](https://www.youtube.com/playlist?list=PLLgJJsrdwhPwJimt5vtHtNmu63OucmPck)

[JUCE Audio Plugin Intro Essentials](https://www.youtube.com/playlist?list=PL7Ysno2nO-6JVw5dTSYCmxdvxFS42kuve)
