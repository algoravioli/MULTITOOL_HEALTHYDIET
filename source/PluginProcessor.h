#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#if (MSVC)
    #include "ipps.h"
#endif

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    /** Member Variables for Distortion */

    void setDistortion (float newAmount)
    {
        distortionAmount = newAmount;
    }

    float getDistortionAmount() const
    {
        return distortionAmount;
    }

    void setFiltBlend (float newAmount)
    {
        filtBlend = newAmount;
    }

    float getFiltBlend() const
    {
        return filtBlend;
    }
    float calculateGainFactor (float dAmount);

    void setCompBlend (float newAmount)
    {
        compBlend = newAmount;
    }

    float getCompBlend() const
    {
        return compBlend;
    }

    void setFiltComp (float newAmount)
    {
        filtComp = newAmount;
    }

    float getFiltComp() const
    {
        return filtComp;
    }

    void setDirtyReverb (float newAmount)
    {
        dirtyReverb = newAmount;
    }

    float getDirtyReverb() const
    {
        return dirtyReverb;
    }

    void setLowOomph (float newAmount)
    {
        lowOomph = newAmount;
    }

    float getLowOomph() const
    {
        return lowOomph;
    }

    void setLightComp (float newAmount)
    {
        lightCompression = newAmount;
    }

    float getLightComp() const
    {
        return lightCompression;
    }

    void setMedComp (float newAmount)
    {
        mediumCompression = newAmount;
    }

    float getMedComp() const
    {
        return mediumCompression;
    }

    void setOutputLevel (float newAmount)
    {
        outputLevel = newAmount;
    }

    float getOutputLevel() const
    {
        return outputLevel;
    }

    juce::dsp::IIR::Filter<float> iir1Filter1L, iir1Filter2L, iir1Filter3L;
    juce::dsp::IIR::Filter<float> iir1Filter1R, iir1Filter2R, iir1Filter3R;

    juce::dsp::IIR::Filter<float> iir2Filter1L, iir2Filter2L, iir2Filter3L;
    juce::dsp::IIR::Filter<float> iir2Filter1R, iir2Filter2R, iir2Filter3R;

    juce::dsp::IIR::Filter<float> iir3Filter1L, iir3Filter2L, iir3Filter3L;
    juce::dsp::IIR::Filter<float> iir3Filter1R, iir3Filter2R, iir3Filter3R;

    juce::dsp::Compressor<float> compressorL, compressorR;
    juce::dsp::Compressor<float> compressor2L, compressor2R;
    juce::dsp::Compressor<float> compressor3L, compressor3R;
    juce::dsp::Compressor<float> compressor4L, compressor4R;
    juce::dsp::Compressor<float> compressor5L, compressor5R;

    juce::dsp::Reverb reverb;

    juce::dsp::Gain<float> gain;

    juce::AudioProcessorValueTreeState parameters;

private:
    float distortionAmount = 1.0f; // Initial distortion level
    float filtBlend = 0.0f; // Initial filter blend level
    float compBlend = 0.0f;
    float filtComp = 0.0f;
    float dirtyReverb = 0.0f;
    float lowOomph = 0.0f;
    float lightCompression = 0.0f;
    float mediumCompression = 0.0f;
    float outputLevel = 1.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
