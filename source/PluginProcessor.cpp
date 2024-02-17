#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>
#include <cmath>
//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
#if !JucePlugin_IsMidiEffect
    #if !JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
    #endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
            ),
      parameters (*this, nullptr, "PARAMETERS", {
                                                    std::make_unique<juce::AudioParameterFloat> ("distortionAmount", "Distortion Amount", 0.0f, 20.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("filtBlend", "Filter Blend", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("compBlend", "Comp Blend", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("filtComp", "Filter Comp", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("dirtyReverb", "Dirty Reverb", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("lowOomph", "Low Oomph", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("lightCompression", "Light Compression", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("mediumCompression", "Medium Compression", 0.0f, 1.0f, 0.0f),
                                                    std::make_unique<juce::AudioParameterFloat> ("outputLevel", "Output Level", 0.0f, 2.0f, 1.0f),
                                                })
{
    /** Initialize stuff here*/
    distortionAmount = 1.0f; // Default Value
    filtBlend = 0.0f; // Default Value
    compBlend = 0.0f; // Default Value
    filtComp = 0.0f; // Default Value
    dirtyReverb = 0.0f; // Default Value
    lowOomph = 0.0f; // Default Value
    lightCompression = 0.0f; // Default Value
    mediumCompression = 0.0f; // Default Value
    outputLevel = 1.0f; // Default Value
    /** Changed to APVTS*/
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // juce::ignoreUnused (sampleRate, samplesPerBlock);
    auto coefficients1 = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, 5000.0f, 1.45f);
    auto coefficients2 = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 100.0f, 2.0f);
    auto coefficients3 = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, 1900.0f, 1.3f);
    iir1Filter1L.reset();
    iir1Filter1L.coefficients = coefficients1;

    iir1Filter2L.reset();
    iir1Filter2L.coefficients = coefficients2;

    iir1Filter3L.reset();
    iir1Filter3L.coefficients = coefficients3;

    iir1Filter1R.reset();
    iir1Filter1R.coefficients = coefficients1;

    iir1Filter2R.reset();
    iir1Filter2R.coefficients = coefficients2;

    iir1Filter3R.reset();
    iir1Filter3R.coefficients = coefficients3;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = 1;

    compressorL.prepare (spec);
    compressorR.prepare (spec);

    compressorL.setThreshold (-25.0f);
    compressorL.setRatio (20.0f);
    compressorL.setAttack (5.0f);
    compressorL.setRelease (100.0f);

    compressorR.setThreshold (-25.0f);
    compressorR.setRatio (20.0f);
    compressorR.setAttack (5.0f);
    compressorR.setRelease (100.0f);

    auto coefficientsGroup2_1 = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 800.0f, 1.4f);
    auto coefficientsGroup2_2 = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 2000.0f, 1.7f);
    auto coefficientsGroup2_3 = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 5000.0f, 2.3f);

    iir2Filter1L.reset();
    iir2Filter1L.coefficients = coefficientsGroup2_1;

    iir2Filter2L.reset();
    iir2Filter2L.coefficients = coefficientsGroup2_2;

    iir2Filter3L.reset();
    iir2Filter3L.coefficients = coefficientsGroup2_3;

    iir2Filter1R.reset();
    iir2Filter1R.coefficients = coefficientsGroup2_1;

    iir2Filter2R.reset();
    iir2Filter2R.coefficients = coefficientsGroup2_2;

    iir2Filter3R.reset();
    iir2Filter3R.coefficients = coefficientsGroup2_3;

    compressor2L.prepare (spec);
    compressor2R.prepare (spec);

    compressor2L.setThreshold (-10.0f);
    compressor2L.setRatio (8.0f);
    compressor2L.setAttack (80.0f);
    compressor2L.setRelease (30.0f);

    compressor2R.setThreshold (-10.0f);
    compressor2R.setRatio (8.0f);
    compressor2R.setAttack (80.0f);
    compressor2R.setRelease (30.0f);

    juce::dsp::ProcessSpec reverbSpec;
    reverbSpec.sampleRate = sampleRate;
    reverbSpec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    reverbSpec.numChannels = static_cast<juce::uint32> (getTotalNumInputChannels());
    reverb.reset();
    reverb.prepare (reverbSpec);
    juce::dsp::Reverb::Parameters reverbParams;
    reverbParams.damping = 0.9f;
    reverbParams.roomSize = 0.1f;
    reverbParams.width = 1.0f;
    reverb.setParameters (reverbParams);

    auto coefficientsGroup3_1 = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 60.0f, 3.0f);
    iir3Filter1L.reset();
    iir3Filter1L.coefficients = coefficientsGroup3_1;
    iir3Filter2L.reset();
    iir3Filter2L.coefficients = coefficientsGroup3_1;
    iir3Filter3L.reset();
    iir3Filter3L.coefficients = coefficientsGroup3_1;
    iir3Filter1R.reset();
    iir3Filter1R.coefficients = coefficientsGroup3_1;
    iir3Filter2R.reset();
    iir3Filter2R.coefficients = coefficientsGroup3_1;
    iir3Filter3R.reset();
    iir3Filter3R.coefficients = coefficientsGroup3_1;

    compressor3L.prepare (spec);
    compressor3R.prepare (spec);

    compressor3L.setThreshold (-20.0f);
    compressor3L.setRatio (10.0f);
    compressor3L.setAttack (10.0f);
    compressor3L.setRelease (100.0f);

    compressor3R.setThreshold (-20.0f);
    compressor3R.setRatio (10.0f);
    compressor3R.setAttack (10.0f);
    compressor3R.setRelease (100.0f);

    compressor4L.prepare (spec);
    compressor4R.prepare (spec);

    compressor4L.setThreshold (-40.0f);
    compressor4L.setRatio (1.5f);
    compressor4L.setAttack (10.0f);
    compressor4L.setRelease (10.0f);

    compressor4R.setThreshold (-40.0f);
    compressor4R.setRatio (1.5f);
    compressor4R.setAttack (10.0f);
    compressor4R.setRelease (10.0f);

    compressor5L.prepare (spec);
    compressor5R.prepare (spec);

    compressor5L.setThreshold (-20.0f);
    compressor5L.setRatio (4.0f);
    compressor5L.setAttack (130.0f);
    compressor5L.setRelease (50.0f);

    compressor5R.setThreshold (-20.0f);
    compressor5R.setRatio (4.0f);
    compressor5R.setAttack (130.0f);
    compressor5R.setRelease (50.0f);

    //get parameter values
    distortionAmount = *parameters.getRawParameterValue ("distortionAmount");
    filtBlend = *parameters.getRawParameterValue ("filtBlend");
    compBlend = *parameters.getRawParameterValue ("compBlend");
    filtComp = *parameters.getRawParameterValue ("filtComp");
    dirtyReverb = *parameters.getRawParameterValue ("dirtyReverb");
    lowOomph = *parameters.getRawParameterValue ("lowOomph");
    lightCompression = *parameters.getRawParameterValue ("lightCompression");
    mediumCompression = *parameters.getRawParameterValue ("mediumCompression");
    outputLevel = *parameters.getRawParameterValue ("outputLevel");
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
    #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    #endif

    return true;
#endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        /** FILTER BLEND STUFF */
        juce::AudioBuffer<float> processedBuffer (1, buffer.getNumSamples());
        processedBuffer.copyFrom (0, 0, channelData, buffer.getNumSamples());
        juce::dsp::AudioBlock<float> block1 (processedBuffer); // Prepare the block from the duplicate
        juce::dsp::ProcessContextReplacing<float> context1 (block1); // Create the context
        if (channel == 0)
        { // Left channel processing
            iir1Filter1L.process (context1);
            iir1Filter2L.process (context1);
            iir1Filter3L.process (context1);
        }
        else if (channel == 1 && totalNumInputChannels > 1)
        { // Right channel processing
            iir1Filter1R.process (context1);
            iir1Filter2R.process (context1);
            iir1Filter3R.process (context1);
        }

        /** COMP BLEND STUFF */
        juce::AudioBuffer<float> compBuffer (1, buffer.getNumSamples());
        compBuffer.copyFrom (0, 0, channelData, buffer.getNumSamples());
        juce::dsp::AudioBlock<float> block2 (compBuffer);
        juce::dsp::ProcessContextReplacing<float> context2 (block2);
        if (channel == 0)
        {
            compressorL.process (context2);
        }
        else if (channel == 1)
        {
            compressorR.process (context2);
        }

        /** FILTER COMP STUFF */
        juce::AudioBuffer<float> filtCompBuffer (1, buffer.getNumSamples());
        filtCompBuffer.copyFrom (0, 0, channelData, buffer.getNumSamples());
        juce::dsp::AudioBlock<float> block3 (filtCompBuffer);
        juce::dsp::ProcessContextReplacing<float> context3 (block3);
        if (channel == 0)
        {
            iir2Filter1L.process (context3);
            iir2Filter2L.process (context3);
            iir2Filter3L.process (context3);
            compressor2L.process (context3);
        }
        else if (channel == 1)
        {
            iir2Filter1R.process (context3);
            iir2Filter2R.process (context3);
            iir2Filter3R.process (context3);
            compressor2R.process (context3);
        }

        /** LOW OOMPH STUFF */
        juce::AudioBuffer<float> lowOomphBuffer (1, buffer.getNumSamples());
        lowOomphBuffer.copyFrom (0, 0, channelData, buffer.getNumSamples());
        juce::dsp::AudioBlock<float> block5 (lowOomphBuffer);
        juce::dsp::ProcessContextReplacing<float> context5 (block5);
        if (channel == 0)
        {
            iir3Filter1L.process (context5);
            iir3Filter2L.process (context5);
            iir3Filter3L.process (context5);
            compressor3L.process (context5);
        }
        else if (channel == 1)
        {
            iir3Filter1R.process (context5);
            iir3Filter2R.process (context5);
            iir3Filter3R.process (context5);
            compressor3R.process (context5);
        }

        /** LIGHT COMP STUFF */
        juce::AudioBuffer<float> lightCompBuffer (1, buffer.getNumSamples());
        lightCompBuffer.copyFrom (0, 0, channelData, buffer.getNumSamples());
        juce::dsp::AudioBlock<float> block6 (lightCompBuffer);
        juce::dsp::ProcessContextReplacing<float> context6 (block6);
        if (channel == 0)
        {
            compressor4L.process (context6);
        }
        else if (channel == 1)
        {
            compressor4R.process (context6);
        }

        /** MEDIUM COMP STUFF */
        juce::AudioBuffer<float> medCompBuffer (1, buffer.getNumSamples());
        medCompBuffer.copyFrom (0, 0, channelData, buffer.getNumSamples());
        juce::dsp::AudioBlock<float> block7 (medCompBuffer);
        juce::dsp::ProcessContextReplacing<float> context7 (block7);
        if (channel == 0)
        {
            compressor5L.process (context7);
        }
        else if (channel == 1)
        {
            compressor5R.process (context7);
        }

        juce::ignoreUnused (channelData);
        // Empirical gain factor based on distortionAmount
        float gainFactor = calculateGainFactor (distortionAmount);
        /** DSP STUFF */

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float originalSample = channelData[sample];
            float distortedSample = std::tanh (originalSample * distortionAmount);
            distortedSample *= gainFactor;
            float processedSample = processedBuffer.getSample (0, sample);
            float compressedSample = compBuffer.getSample (0, sample);
            float filtCompSample = filtCompBuffer.getSample (0, sample);
            float lowOomphSample = lowOomphBuffer.getSample (0, sample);
            float lightCompSample = lightCompBuffer.getSample (0, sample);
            float medCompSample = medCompBuffer.getSample (0, sample);

            float compGain1 = 10.0f;
            float compGain2 = 2.0f;

            auto distortionOutput = (((1 - 0.2f * distortionAmount) * originalSample) + ((0.2f * distortionAmount) * distortedSample)) * 1.1f;
            auto filtBlendOutput = distortionOutput + (filtBlend * processedSample);
            auto compBlendOutput = (1 - compBlend) * filtBlendOutput + (compBlend * compressedSample * compGain1);
            auto filtCompOutput = compBlendOutput + (filtComp * filtCompSample * compGain2);
            auto lowOomphOutput = ((1 - lowOomph) * filtCompOutput) + (lowOomph * lowOomphSample * 5.0f);
            auto mediumCompOutput = ((1 - mediumCompression) * lowOomphOutput) + (mediumCompression * medCompSample * 5.0f);
            auto lightCompOutput = ((1 - lightCompression) * mediumCompOutput) + (lightCompression * lightCompSample * 5.0f);

            channelData[sample] = lightCompOutput;
        }
    }
    /** DirtyReverbStuff */
    juce::dsp::AudioBlock<float> block4 (buffer);
    juce::dsp::ProcessContextReplacing<float> context4 (block4);
    juce::dsp::Reverb::Parameters params = reverb.getParameters();
    params.wetLevel = dirtyReverb;
    params.dryLevel = 1.0f;
    reverb.setParameters (params);
    reverb.process (context4);
    float currentOutputLevel = *parameters.getRawParameterValue ("outputLevel");
    gain.setGainLinear (currentOutputLevel);

    juce::dsp::AudioBlock<float> block8 (buffer);
    juce::dsp::ProcessContextReplacing<float> context8 (block8);
    gain.process (context8);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
    // juce::ignoreUnused (destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // juce::ignoreUnused (data, sizeInBytes);
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

float PluginProcessor::calculateGainFactor (float dAmount)
{
    float gainFactor = 1.0f - (dAmount * 0.035f);
    return gainFactor;
}
