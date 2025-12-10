/*
  ==============================================================================

    PluginEditor.h
    Interface gráfica personalizada para o Compressor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

namespace CompressorUIConstants
{
    // Ângulos do arco do knob (como muitos plugins: ~7h até ~5h)
    constexpr float rotaryStartAngle = juce::MathConstants<float>::pi * 1.25f;
    constexpr float rotaryEndAngle = juce::MathConstants<float>::pi * 2.75f;
}

//==============================================================================
// LookAndFeel customizado (cores e knobs bonitos)
class CompressorLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CompressorLookAndFeel()
    {
        using namespace juce;

        // Tema escuro geral
        setColour(ResizableWindow::backgroundColourId, Colour(0xFF101018));
        setColour(Slider::textBoxBackgroundColourId, Colour(0xFF121212));
        setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
        setColour(Label::textColourId, Colours::white);
    }

    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float /*rotaryStartAngle*/,
        float /*rotaryEndAngle*/,
        juce::Slider& slider) override
    {
        using namespace juce;
        using namespace CompressorUIConstants;

        const float radius = (float)jmin(width, height) * 0.45f;
        const float centreX = (float)x + (float)width * 0.5f;
        const float centreY = (float)y + (float)height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;

        const float startAngle = rotaryStartAngle;
        const float endAngle = rotaryEndAngle;
        const float angle = startAngle + sliderPosProportional * (endAngle - startAngle);

        auto knobFill = slider.findColour(Slider::rotarySliderFillColourId);
        auto knobThumb = slider.findColour(Slider::thumbColourId);

        // Fundo do knob
        g.setColour(Colour(0xFF0F172A));
        g.fillEllipse(rx, ry, rw, rw);

        // Anel externo
        g.setColour(Colour(0xFF1F2937));
        g.drawEllipse(rx, ry, rw, rw, 1.5f);

        // Arco de valor
        {
            Path valueArc;
            valueArc.addCentredArc(centreX, centreY, radius, radius,
                0.0f, startAngle, angle, true);
            g.setColour(knobFill);
            g.strokePath(valueArc, PathStrokeType(3.0f, PathStrokeType::curved, PathStrokeType::rounded));
        }

        // Ponteiro
        {
            const float pointerLength = radius * 0.8f;
            const float pointerThickness = 2.0f;

            Path p;
            p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
            p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

            g.setColour(knobThumb);
            g.fillPath(p);
        }
    }
};

//==============================================================================

class CompressorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    CompressorAudioProcessorEditor(CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    CompressorAudioProcessor& audioProcessor;

    CompressorLookAndFeel lookAndFeel;

    // Sliders dos parâmetros (agora sliders normais, com desenho customizado)
    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    juce::Slider makeupSlider;

    // Labels
    juce::Label thresholdLabel;
    juce::Label ratioLabel;
    juce::Label attackLabel;
    juce::Label releaseLabel;
    juce::Label makeupLabel;

    // Attachments para ligar com o APVTS
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> thresholdAttachment;
    std::unique_ptr<SliderAttachment> ratioAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> makeupAttachment;

    void setupKnob(juce::Slider& slider,
        juce::Label& label,
        const juce::String& labelText,
        juce::Colour featureColour);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessorEditor)
};
