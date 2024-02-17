#include "PluginEditor.h"
#include "PluginUI_HealthyDiet.h"

int const SliderWidthDivisor = 11;
int const SliderSpacing = 15;
float const FontSize = 13.0f;

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    juce::ignoreUnused (processorRef);

    // addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    // inspectButton.onClick = [&] {
    //     if (!inspector)
    //     {
    //         inspector = std::make_unique<melatonin::Inspector> (*this);
    //         inspector->onClose = [this]() { inspector.reset(); };
    //     }

    //     inspector->setVisible (true);
    // };

    auto bounds = getLocalBounds();
    auto topLeft = bounds.getTopLeft();
    auto bottomRight = bounds.getBottomRight();
    auto topLeftX = topLeft.getX();
    // auto topLeftY = topLeft.getY();
    auto bottomRightX = bottomRight.getX();
    // auto bottomRightY = bottomRight.getY();
    auto sliderWidthDefault = (bottomRightX - topLeftX) / SliderWidthDivisor;
    // Distortion Amount Slider
    distortionAmountSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    distortionAmountSlider.setBounds (10, 10, sliderWidthDefault, 200);
    distortionAmountSlider.setRange (1.0, 10.0);
    distortionAmountSlider.setValue (1.0);
    distortionAmountSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    distortionAmountSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    distortionAmountSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    distortionAmountSlider.addListener (this);

    // Distortion Amount Text
    distortionAmountLabel.setText ("Distortion", juce::dontSendNotification);
    distortionAmountLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    distortionAmountLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    distortionAmountLabel.setJustificationType (juce::Justification::centred);
    distortionAmountLabel.attachToComponent (&distortionAmountSlider, false);

    //FiltBlend Slider
    filtBlendSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    filtBlendSlider.setBounds (10, 10, sliderWidthDefault, 200);
    filtBlendSlider.setRange (0.0, 1.0);
    filtBlendSlider.setValue (0.0);
    filtBlendSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    filtBlendSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    filtBlendSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    filtBlendSlider.addListener (this);

    //FiltBlend Text
    filtBlendLabel.setText ("FiltBlend", juce::dontSendNotification);
    filtBlendLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    filtBlendLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    filtBlendLabel.setJustificationType (juce::Justification::centred);
    filtBlendLabel.attachToComponent (&filtBlendSlider, false);

    //CompBlend Slider
    compBlendSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    compBlendSlider.setBounds (10, 10, sliderWidthDefault, 200);
    compBlendSlider.setRange (0.0, 1.0);
    compBlendSlider.setValue (0.0);
    compBlendSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    compBlendSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    compBlendSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    compBlendSlider.addListener (this);

    //CompBlend Text
    compBlendLabel.setText ("CompBlend", juce::dontSendNotification);
    compBlendLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    compBlendLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    compBlendLabel.setJustificationType (juce::Justification::centred);
    compBlendLabel.attachToComponent (&compBlendSlider, false);

    //filtComp Slider
    filtCompSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    filtCompSlider.setBounds (10, 10, sliderWidthDefault, 200);
    filtCompSlider.setRange (0.0, 1.0);
    filtCompSlider.setValue (0.0);
    filtCompSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    filtCompSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    filtCompSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    filtCompSlider.addListener (this);

    //filtComp Text
    filtCompLabel.setText ("FiltComp", juce::dontSendNotification);
    filtCompLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    filtCompLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    filtCompLabel.setJustificationType (juce::Justification::centred);
    filtCompLabel.attachToComponent (&filtCompSlider, false);

    //dirtyReverb Slider
    dirtyReverbSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    dirtyReverbSlider.setBounds (10, 10, sliderWidthDefault, 200);
    dirtyReverbSlider.setRange (0.0, 1.0);
    dirtyReverbSlider.setValue (0.0);
    dirtyReverbSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    dirtyReverbSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    dirtyReverbSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    dirtyReverbSlider.addListener (this);

    //dirtyReverb Text
    dirtyReverbLabel.setText ("DirtyReverb", juce::dontSendNotification);
    dirtyReverbLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    dirtyReverbLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    dirtyReverbLabel.setJustificationType (juce::Justification::centred);
    dirtyReverbLabel.attachToComponent (&dirtyReverbSlider, false);

    //lowOomph Slider
    lowOomphSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    lowOomphSlider.setBounds (10, 10, sliderWidthDefault, 200);
    lowOomphSlider.setRange (0.0, 1.0);
    lowOomphSlider.setValue (0.0);
    lowOomphSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    lowOomphSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    lowOomphSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    lowOomphSlider.addListener (this);

    //lowOomph Text
    lowOomphLabel.setText ("LowOomph", juce::dontSendNotification);
    lowOomphLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    lowOomphLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    lowOomphLabel.setJustificationType (juce::Justification::centred);
    lowOomphLabel.attachToComponent (&lowOomphSlider, false);

    //lightComp Slider
    lightCompSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    lightCompSlider.setBounds (10, 10, sliderWidthDefault, 200);
    lightCompSlider.setRange (0.0, 1.0);
    lightCompSlider.setValue (0.0);
    lightCompSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    lightCompSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    lightCompSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    lightCompSlider.addListener (this);

    //lightComp Text
    lightCompLabel.setText ("LightComp", juce::dontSendNotification);
    lightCompLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    lightCompLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    lightCompLabel.setJustificationType (juce::Justification::centred);
    lightCompLabel.attachToComponent (&lightCompSlider, false);

    //medComp Slider
    medCompSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    medCompSlider.setBounds (10, 10, sliderWidthDefault, 200);
    medCompSlider.setRange (0.0, 1.0);
    medCompSlider.setValue (0.0);
    medCompSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    medCompSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    medCompSlider.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    medCompSlider.addListener (this);

    //medComp Text
    medCompLabel.setText ("MedComp", juce::dontSendNotification);
    medCompLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    medCompLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    medCompLabel.setJustificationType (juce::Justification::centred);
    medCompLabel.attachToComponent (&medCompSlider, false);

    //outputLevel Slider
    outputLevel.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
    outputLevel.setBounds (10, 10, sliderWidthDefault, 200);
    outputLevel.setRange (0.0, 2.0);
    outputLevel.setValue (1.0);
    outputLevel.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    outputLevel.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    outputLevel.setColour (juce::Slider::thumbColourId, juce::Colours::black);
    outputLevel.addListener (this);

    //outputLevel Text
    outputLevelLabel.setText ("OutputLevel", juce::dontSendNotification);
    outputLevelLabel.setFont (juce::Font (FontSize, juce::Font::bold));
    outputLevelLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    outputLevelLabel.setJustificationType (juce::Justification::centred);
    outputLevelLabel.attachToComponent (&outputLevel, false);

    // addAndMakeVisible (distortionAmountLabel);
    // addAndMakeVisible (filtBlendLabel);
    // addAndMakeVisible (compBlendLabel);
    // addAndMakeVisible (filtCompLabel);
    // addAndMakeVisible (dirtyReverbLabel);
    // addAndMakeVisible (lowOomphLabel);
    // addAndMakeVisible (lightCompLabel);
    // addAndMakeVisible (medCompLabel);
    // addAndMakeVisible (outputLevelLabel);

    addAndMakeVisible (distortionAmountSlider);
    addAndMakeVisible (filtBlendSlider);
    addAndMakeVisible (compBlendSlider);
    addAndMakeVisible (filtCompSlider);
    addAndMakeVisible (dirtyReverbSlider);
    addAndMakeVisible (lowOomphSlider);
    addAndMakeVisible (lightCompSlider);
    addAndMakeVisible (medCompSlider);
    addAndMakeVisible (outputLevel);

    distortionAmountSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "distortionAmount", distortionAmountSlider);
    filtBlendSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "filtBlend", filtBlendSlider);
    compBlendSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "compBlend", compBlendSlider);
    filtCompSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "filtComp", filtCompSlider);
    dirtyReverbSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "dirtyReverb", dirtyReverbSlider);
    lowOomphSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "lowOomph", lowOomphSlider);
    lightCompSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "lightCompression", lightCompSlider);
    medCompSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "mediumCompression", medCompSlider);
    outputLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.parameters, "outputLevel", outputLevel);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    juce::Image backgroundImage = juce::ImageCache::getFromMemory (PluginUI_HealthyDiet_png, static_cast<int> (PluginUI_HealthyDiet_png_len));
    if (!backgroundImage.isNull())
    {
        // Set the target area to fit the resized image
        juce::Rectangle<float> targetArea (0, 0, 800, 400);

        // Draw the image resized to fit the target area
        g.drawImageWithin (backgroundImage,
            static_cast<int> (targetArea.getX()),
            static_cast<int> (targetArea.getY()),
            static_cast<int> (targetArea.getWidth()),
            static_cast<int> (targetArea.getHeight()),
            juce::RectanglePlacement::centred,
            false);
    }
    else
    {
        g.fillAll (juce::Colours::white); // Fallback color if image fails to load
    }
    g.setColour (juce::Colours::black);
    auto bounds = getLocalBounds();
    auto topLeft = bounds.getTopLeft();
    auto bottomRight = bounds.getBottomRight();
    auto topLeftX = topLeft.getX();
    auto topLeftY = topLeft.getY();
    auto bottomRightX = bottomRight.getX();
    auto bottomRightY = bottomRight.getY();
    // g.drawLine (topLeftX, topLeftY, bottomRightX, bottomRightY, static_cast<float> (bottomRightX - topLeftX) / 100.0f);
    auto LineY = (bottomRightY - topLeftY) / 32;
    for (int i = 2; i < 32; i++)
    {
        if (i == 7 || i == 26)
        {
            g.drawLine (topLeftX, topLeftY + (i * LineY), bottomRightX, topLeftY + (i * LineY), static_cast<float> (bottomRightX - topLeftX) / 150.0f);
        }
    }

    int margin = 10; // Margin from the top and right edges
    int textWidth = 150;
    int textHeight = 40;
    juce::Rectangle<int> textArea (getWidth() - textWidth - margin, getHeight() - textHeight - margin, textWidth, textHeight);
    auto pluginBottomText = juce::String ("Algoravioli") + " v" + juce::String (VERSION);
    // g.setFont (juce::Font ("Arial", 30.0f, juce::Font::plain));
    // // Draw the text right-justified within the defined area
    g.drawText (pluginBottomText, textArea, juce::Justification::right, true);
    // auto area = getLocalBounds();
    // g.setColour (juce::Colours::white);
    // g.setFont (16.0f);
    // auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    // g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();

    auto bounds = getLocalBounds();
    auto topLeft = bounds.getTopLeft();
    auto bottomRight = bounds.getBottomRight();
    auto topLeftX = topLeft.getX();
    // auto topLeftY = topLeft.getY();
    auto bottomRightX = bottomRight.getX();
    // auto bottomRightY = bottomRight.getY();

    auto sliderWidthDefault = (bottomRightX - topLeftX) / SliderWidthDivisor;

    // Set the vertical slider area, adjusting the X position to shift left
    auto sliderWidth = sliderWidthDefault;
    auto sliderHeight = 200;
    auto sliderX = 10; // You can decrease this value to move the slider further left
    auto sliderY = area.getCentreY() - (sliderHeight / 2); // Center the slider vertically

    // Set the label bounds to be just below the slider
    auto labelHeight = 20;
    auto labelY = sliderY + sliderHeight + 20; // 10 pixels below the slider

    // for distortionAmount
    distortionAmountSlider.setBounds (sliderX, sliderY, sliderWidth, sliderHeight);
    // distortionAmountLabel.setBounds (sliderX, labelY, sliderWidth, labelHeight);
    // for filtBlend
    filtBlendSlider.setBounds (sliderX + sliderWidth + SliderSpacing, sliderY, sliderWidth, sliderHeight);
    // filtBlendLabel.setBounds (sliderX + sliderWidth + SliderSpacing, labelY, sliderWidth, labelHeight);
    // for compBlend
    compBlendSlider.setBounds (sliderX + (sliderWidth + SliderSpacing) * 2, sliderY, sliderWidth, sliderHeight);
    // compBlendLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 2, labelY, sliderWidth, labelHeight);
    // for filtComp
    filtCompSlider.setBounds (sliderX + (sliderWidth + SliderSpacing) * 3, sliderY, sliderWidth, sliderHeight);
    // filtCompLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 3, labelY, sliderWidth, labelHeight);
    // for dirtyReverb
    dirtyReverbSlider.setBounds (sliderX + (sliderWidth + SliderSpacing) * 4, sliderY, sliderWidth, sliderHeight);
    // dirtyReverbLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 4, labelY, sliderWidth, labelHeight);
    // for lowOomph
    lowOomphSlider.setBounds (sliderX + (sliderWidth + SliderSpacing) * 5, sliderY, sliderWidth, sliderHeight);
    // lowOomphLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 5, labelY, sliderWidth, labelHeight);
    // for lightComp
    lightCompSlider.setBounds (sliderX + (sliderWidth + SliderSpacing) * 6, sliderY, sliderWidth, sliderHeight);
    // lightCompLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 6, labelY, sliderWidth, labelHeight);
    // for medComp
    medCompSlider.setBounds (sliderX + (sliderWidth + SliderSpacing) * 7, sliderY, sliderWidth, sliderHeight);
    // medCompLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 7, labelY, sliderWidth, labelHeight);
    // for outputLevel
    outputLevel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 8, sliderY, sliderWidth, sliderHeight);
    // outputLevelLabel.setBounds (sliderX + (sliderWidth + SliderSpacing) * 8, labelY, sliderWidth, labelHeight);

    // inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre (100, 50));
    distortionAmountLabel.setVisible (false);
    filtBlendLabel.setVisible (false);
    compBlendLabel.setVisible (false);
    filtCompLabel.setVisible (false);
    dirtyReverbLabel.setVisible (false);
    lowOomphLabel.setVisible (false);
    lightCompLabel.setVisible (false);
    medCompLabel.setVisible (false);
    outputLevelLabel.setVisible (false);
}

void PluginEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &distortionAmountSlider)
    {
        processorRef.setDistortion (static_cast<float> (slider->getValue()));
    }
    else if (slider == &filtBlendSlider)
    {
        processorRef.setFiltBlend (static_cast<float> (slider->getValue()));
    }
    else if (slider == &compBlendSlider)
    {
        processorRef.setCompBlend (static_cast<float> (slider->getValue()));
    }
    else if (slider == &filtCompSlider)
    {
        processorRef.setFiltComp (static_cast<float> (slider->getValue()));
    }
    else if (slider == &dirtyReverbSlider)
    {
        processorRef.setDirtyReverb (static_cast<float> (slider->getValue()));
    }
    else if (slider == &lowOomphSlider)
    {
        processorRef.setLowOomph (static_cast<float> (slider->getValue()));
    }
    else if (slider == &lightCompSlider)
    {
        processorRef.setLightComp (static_cast<float> (slider->getValue()));
    }
    else if (slider == &medCompSlider)
    {
        processorRef.setMedComp (static_cast<float> (slider->getValue()));
    }
    else if (slider == &outputLevel)
    {
        processorRef.setOutputLevel (static_cast<float> (slider->getValue()));
    }
}