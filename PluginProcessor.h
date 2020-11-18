/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class A3AudioProcessor  : public juce::AudioProcessor
{
public:
    juce::File userfile;
    juce::File desktop;
    juce::File irfile, lastir;
    int reverbT, emir, lemir;
    bool rtochange;
    float irLength,  dSpeed;
    juce::AudioBuffer<float> outbuffer;
    juce::AudioBuffer<float> irBuffer;

    //==============================================================================
    A3AudioProcessor();
    ~A3AudioProcessor() override;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void decreasing(juce::AudioBuffer<float>& buffer );
    void updateFX();
    void updateParameters ();
    void delayline(juce::AudioBuffer<float>& buffer, juce::dsp::ProcessContextNonReplacing<float>);
    juce::AudioProcessorValueTreeState apvts;

    void gainup(float* data, int numsamples);


private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::dsp::StateVariableTPTFilter<float> stateVariableFilter;
    juce::dsp::AudioBlock<float> tmpblock;
    bool bypassFilter = false;
    int sRate, sBlock;

    int WritePosition{ 0 };
    enum { phaserIndex, gainIndex, reverbIndex, convoIndex};
    juce::dsp::ProcessorChain<juce::dsp::Phaser<float>,juce::dsp::Gain<float>, juce::dsp::Reverb, juce::dsp::Convolution> fxChain;
    enum { ccIndex };
    juce::dsp::ProcessorChain<juce::dsp::Convolution> constructedIR;
    bool bypassPhaser = false;
    juce::dsp::ProcessSpec gspec;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (A3AudioProcessor)
};
