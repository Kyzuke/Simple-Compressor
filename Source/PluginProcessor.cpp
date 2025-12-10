#include "PluginProcessor.h"
#include "PluginEditor.h"

CompressorAudioProcessor::CompressorAudioProcessor()
    : AudioProcessor(BusesProperties()
                      .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameters())
{
}

CompressorAudioProcessor::~CompressorAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold", -60.0f, 0.0f, -24.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RATIO", "Ratio", 1.0f, 20.0f, 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 1.0f, 200.0f, 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 1.0f, 500.0f, 100.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MAKEUP", "Makeup Gain", 0.0f, 24.0f, 0.0f));

    return { params.begin(), params.end() };
}

void CompressorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(getMainBusNumOutputChannels()) };
    compressor.prepare(spec);
}

void CompressorAudioProcessor::releaseResources() {}

bool CompressorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}

void CompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    compressor.setThreshold(*apvts.getRawParameterValue("THRESHOLD"));
    compressor.setRatio(*apvts.getRawParameterValue("RATIO"));
    compressor.setAttack(*apvts.getRawParameterValue("ATTACK"));
    compressor.setRelease(*apvts.getRawParameterValue("RELEASE"));

    float makeup = *apvts.getRawParameterValue("MAKEUP");

    juce::dsp::AudioBlock<float> block(buffer);
    compressor.process(juce::dsp::ProcessContextReplacing<float>(block));

    // Makeup gain
    buffer.applyGain(juce::Decibels::decibelsToGain(makeup));
}

const juce::String CompressorAudioProcessor::getName() const { return "Simple Compressor"; }
bool CompressorAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    // Usa a nossa interface personalizada em vez do editor genérico
    return new CompressorAudioProcessorEditor(*this);
}

bool CompressorAudioProcessor::acceptsMidi() const { return false; }
bool CompressorAudioProcessor::producesMidi() const { return false; }
bool CompressorAudioProcessor::isMidiEffect() const { return false; }
double CompressorAudioProcessor::getTailLengthSeconds() const { return 0.0; }

void CompressorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    apvts.state.writeToStream(stream);
}

void CompressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
        apvts.state = tree;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1; // apenas 1 programa
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int)
{
}

const juce::String CompressorAudioProcessor::getProgramName (int)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int, const juce::String&)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
