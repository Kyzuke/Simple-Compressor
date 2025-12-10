/*
  ==============================================================================

    PluginEditor.cpp
    Interface gráfica personalizada para o Compressor

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

CompressorAudioProcessorEditor::CompressorAudioProcessorEditor(CompressorAudioProcessor& p)
    : AudioProcessorEditor(&p),
    audioProcessor(p)
{
    using namespace juce;

    // Aplica nosso LookAndFeel customizado em toda a janela
    setLookAndFeel(&lookAndFeel);

    // Tamanho da janela (parecido com plugins reais)
    setSize(600, 340);

    // Cores diferentes para cada feature
    auto colourThreshold = Colour(0xFFF97316); // laranja
    auto colourRatio = Colour(0xFF3B82F6); // azul
    auto colourAttack = Colour(0xFF22C55E); // verde
    auto colourRelease = Colour(0xFFA855F7); // roxo
    auto colourMakeup = Colour(0xFFEAB308); // amarelo

    // Configura knobs e labels com cores individuais
    setupKnob(thresholdSlider, thresholdLabel, "Threshold (dB)", colourThreshold);
    thresholdSlider.setMouseDragSensitivity(80);

    setupKnob(ratioSlider, ratioLabel, "Ratio", colourRatio);
    ratioSlider.setMouseDragSensitivity(80);

    setupKnob(attackSlider, attackLabel, "Attack (ms)", colourAttack);
    attackSlider.setMouseDragSensitivity(80);

    setupKnob(releaseSlider, releaseLabel, "Release (ms)", colourRelease);
    releaseSlider.setMouseDragSensitivity(80);

    setupKnob(makeupSlider, makeupLabel, "Makeup (dB)", colourMakeup);
    makeupSlider.setMouseDragSensitivity(80);

    // Liga sliders ao ValueTreeState
    thresholdAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "THRESHOLD", thresholdSlider);
    ratioAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RATIO", ratioSlider);
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    makeupAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "MAKEUP", makeupSlider);

    // Se quiser mais “suavidade” no drag, pode ajustar:
    // thresholdSlider.setMouseDragSensitivity (120);
    // ratioSlider.setMouseDragSensitivity (120);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    // Sempre voltar o LookAndFeel para evitar problemas
    setLookAndFeel(nullptr);
}

//==============================================================================

void CompressorAudioProcessorEditor::setupKnob(juce::Slider& slider,
    juce::Label& label,
    const juce::String& labelText,
    juce::Colour featureColour)
{
    using namespace juce;

    slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);

    // Cores específicas do parâmetro
    slider.setColour(Slider::rotarySliderFillColourId, featureColour);
    slider.setColour(Slider::thumbColourId, featureColour.brighter(0.3f));
    slider.setColour(Slider::textBoxTextColourId, Colours::white);

    addAndMakeVisible(slider);

    label.setText(labelText, dontSendNotification);
    label.setJustificationType(Justification::centred);
    label.setFont(FontOptions(13.0f, Font::plain));
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label);
}

//==============================================================================

void CompressorAudioProcessorEditor::paint(juce::Graphics& g)
{
    using namespace juce;

    auto bounds = getLocalBounds().toFloat();

    // Fundo em gradiente
    Colour topColour = Colour(0xFF050815);
    Colour bottomColour = Colour(0xFF151B2F);

    g.setGradientFill(ColourGradient(topColour, bounds.getCentreX(), bounds.getY(),
        bottomColour, bounds.getCentreX(), bounds.getBottom(),
        false));
    g.fillAll();

    // Barra superior com título
    auto header = getLocalBounds().removeFromTop(40);
    g.setColour(Colour(0xFF0D1325));
    g.fillRect(header);

    g.setColour(juce::Colours::white);
    g.setFont(FontOptions(18.0f, juce::Font::bold));
    g.drawText("Simple Compressor", header.reduced(10, 0),
        juce::Justification::centredLeft, false);

    // Caixa "COMPRESSION"
    auto area = getLocalBounds().reduced(10);
    area.removeFromTop(40); // já usamos o header
    auto compressionArea = area.reduced(10);

    g.setColour(Colour(0x33000000));
    g.fillRoundedRectangle(compressionArea.toFloat(), 8.0f);

    g.setColour(Colour(0x66FFFFFF));
    g.drawRoundedRectangle(compressionArea.toFloat(), 8.0f, 1.0f);

    g.setFont(FontOptions(14.0f, juce::Font::bold));
    g.setColour(Colour(0xFFCBD5F5));
    g.drawText("COMPRESSION", compressionArea.removeFromTop(24),
        juce::Justification::centredLeft, false);
}

//==============================================================================

void CompressorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);

    // Reservar área do cabeçalho
    area.removeFromTop(40);

    // Área principal
    auto content = area.reduced(10);

    // Tirar título "COMPRESSION"
    content.removeFromTop(24);

    // Layout em duas linhas, 3 colunas
    auto topRow = content.removeFromTop(content.getHeight() / 2);
    auto bottomRow = content;

    const int numColumns = 3;
    const int hGap = 10;

    auto layoutRow = [numColumns, hGap](juce::Rectangle<int> row,
        juce::Slider& s1, juce::Label& l1,
        juce::Slider& s2, juce::Label& l2,
        juce::Slider* s3 = nullptr, juce::Label* l3 = nullptr)
        {
            auto columnWidth = (row.getWidth() - (hGap * (numColumns - 1))) / numColumns;

            auto col1 = row.removeFromLeft(columnWidth);
            row.removeFromLeft(hGap);
            auto col2 = row.removeFromLeft(columnWidth);
            juce::Rectangle<int> col3;

            if (s3 != nullptr && l3 != nullptr)
            {
                row.removeFromLeft(hGap);
                col3 = row.removeFromLeft(columnWidth);
            }

            auto place = [](juce::Rectangle<int> bounds, juce::Slider& slider, juce::Label& label)
                {
                    auto labelHeight = 18;
                    label.setBounds(bounds.removeFromTop(labelHeight));
                    slider.setBounds(bounds.reduced(5));
                };

            place(col1, s1, l1);
            place(col2, s2, l2);

            if (s3 != nullptr && l3 != nullptr)
                place(col3, *s3, *l3);
        };

    // Linha de cima: Threshold, Ratio, Attack
    layoutRow(topRow, thresholdSlider, thresholdLabel,
        ratioSlider, ratioLabel,
        &attackSlider, &attackLabel);

    // Linha de baixo: Release, Makeup (esquerda/direita)
    auto bottomLeft = bottomRow.removeFromLeft(bottomRow.getWidth() / 2);
    auto bottomRight = bottomRow;

    {
        auto labelHeight = 18;

        // Release na esquerda
        auto boundsRelease = bottomLeft;
        releaseLabel.setBounds(boundsRelease.removeFromTop(labelHeight));
        releaseSlider.setBounds(boundsRelease.reduced(20, 5));

        // Makeup na direita
        auto boundsMakeup = bottomRight;
        makeupLabel.setBounds(boundsMakeup.removeFromTop(labelHeight));
        makeupSlider.setBounds(boundsMakeup.reduced(20, 5));
    }
}
