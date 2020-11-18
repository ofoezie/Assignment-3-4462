/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A3AudioProcessorEditor::A3AudioProcessorEditor(A3AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 400);
    addAndMakeVisible(&openButton);
    openButton.onClick = [this] { buttonClicked(); };


    emulatedImpulseMenu.setJustificationType(juce::Justification::centred);
    emulatedImpulseMenu.addItem("Emulated IR Off", 1);
    emulatedImpulseMenu.addItem("Fast Decay IR", 2);
    emulatedImpulseMenu.addItem("Slow Decay IR", 3);
    addAndMakeVisible(&emulatedImpulseMenu);

    reverbT.setJustificationType(juce::Justification::centred);
    reverbT.addItem("No Reverb", 1);
    reverbT.addItem("Basic Reverb", 2);
    reverbT.addItem("Recorded Impulse Response", 3);
    reverbT.addItem("Constructed Impulse Response", 4);
    reverbT.addItem("Simluation Reverb(Not Implemented)", 5);
    addAndMakeVisible(&reverbT);


    impulseMenu.setJustificationType(juce::Justification::centred);

    impulseMenu.addItem("Impulse Response: Off", 1);

    impulseMenu.addItem("Parking Garage", 2);
    impulseMenu.addItem("St Nicolaes Church", 3);
    impulseMenu.addItem("Large Wide Echo Hall", 4);
    impulseMenu.addItem("Conic Long Echo Hall", 5);
    impulseMenu.addItem("On a Star", 6);
    impulseMenu.addItem("Large Long Echo Hall", 7);
    impulseMenu.addItem("In a Silo", 8);
    impulseMenu.addItem("{ Uploaded Impulse Response }", 9);

    addAndMakeVisible(&impulseMenu);


    cutOffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    cutOffSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    cutOffSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&cutOffSlider);
    delaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delaySlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delaySlider);



    drySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    drySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    drySlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&drySlider);

    roomSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    roomSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    roomSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&roomSlider);
    wetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    wetSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    wetSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&wetSlider);
    widthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    widthSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&widthSlider);

    IRLengthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    IRLengthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    IRLengthSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&IRLengthSlider);


    cutOffValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutOffSlider);
    rateValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASERRATE", rateSlider);
    depthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASERDEPTH", depthSlider);
    gainValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    gainValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY", delaySlider);

    dryValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRYLEVEL", drySlider);

    roomValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ROOMSIZE", roomSlider);
    wetValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WET", wetSlider);
    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WIDTH", widthSlider);
    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "IRLENGTH", IRLengthSlider);


    filterMenuValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERMENU", filterMenu);
    phaserMenuValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "PHASERMENU", phaserMenu);
    ImpulseValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "IMPULSEMENU", impulseMenu);
    reverbValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "REVERBMENU", reverbT);
    emulatedImpulseValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "EMULATEDMENU", emulatedImpulseMenu);

}

A3AudioProcessorEditor::~A3AudioProcessorEditor()
{
}

//==============================================================================
void A3AudioProcessorEditor::paint(juce::Graphics& g)
{
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::greenyellow);
    juce::Rectangle<int> area = getLocalBounds().reduced(5);
    juce::Rectangle<int> titleArea = area.removeFromTop(20);

    g.fillAll(juce::Colours::grey);
    g.setColour(juce::Colours::white);
    g.drawText("A3 Starter", titleArea, juce::Justification::centredTop);

    juce::Rectangle <int> innerArea = getLocalBounds().reduced(25);//(25, 25, 350, 150);
    g.setColour(juce::Colours::cadetblue);
    g.drawRect(innerArea, 1.0f);

    int innerWidth = innerArea.getWidth();
    g.drawText("Cutoff", cutOffSlider.getX() - 10, cutOffSlider.getY() - 10, innerWidth / 6, 25, juce::Justification::centred);

    g.drawText("Basic Reverb settings ", drySlider.getX()-(innerWidth/7.5), drySlider.getY()-30 , innerWidth / 4, 25, juce::Justification::centred);
    g.drawText("Simulation Reverb settings ", cutOffSlider.getX()- (innerWidth/4.5), cutOffSlider.getY()-30, innerWidth / 3, 25, juce::Justification::centred);
    g.drawText("Dry", drySlider.getX() - 10, drySlider.getY() - 10, innerWidth / 6, 25, juce::Justification::centred);

    g.drawText("Room size", roomSlider.getX() - 10, roomSlider.getY() - 10, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Wet", wetSlider.getX() - 10, wetSlider.getY() - 10, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Width", widthSlider.getX() - 10, widthSlider.getY() - 10, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay time ", delaySlider.getX() - 10, delaySlider.getY() - 10, innerWidth / 6, 25, juce::Justification::centred);

    g.drawText("Length of Emulated IR Reverb \n (Warning: There may be loud noises)", emulatedImpulseMenu.getX() + 5, IRLengthSlider.getY()+5, innerWidth / 3, 40, juce::Justification::centred);
}

void A3AudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    juce::Rectangle<int> menus = area.removeFromTop(20);
    int width = menus.getWidth();

    reverbT.setBounds(menus.removeFromLeft(width / 3 - 5));
    impulseMenu.setBounds(menus.removeFromLeft(width / 3 - 5));
    emulatedImpulseMenu.setBounds(menus.removeFromLeft(width / 3 - 5));
    openButton.setBounds(impulseMenu.getX(), impulseMenu.getY() + 40, impulseMenu.getWidth(), 40);

    drySlider.setBounds(145, 140, width / 6 - 10, 100);

    roomSlider.setBounds(260, 140, width / 6 - 10, 100);
    wetSlider.setBounds(375, 140, width / 6 - 10, 100);
    widthSlider.setBounds(490, 140, width / 6 - 10, 100);
    cutOffSlider.setBounds(260, 280, width /6 - 10, 100);
    delaySlider.setBounds( 375, 280, width / 6 - 10, 100);
    IRLengthSlider.setBounds(emulatedImpulseMenu.getX(), emulatedImpulseMenu.getY() + 30, width / 4 - 10, 100);
}

void A3AudioProcessorEditor::buttonClicked() {
    juce::FileChooser chooser("Select a Wave file to play...", {}, "*.wav");

    if (chooser.browseForFileToOpen())
    {

        auto file = chooser.getResult();
        audioProcessor.userfile = file;
        if (file.exists()) {

            impulseMenu.changeItemText(9, file.getFileNameWithoutExtension());
            impulseMenu.setSelectedId(9);
            impulseMenu.setText(file.getFileNameWithoutExtension());
        }

    }

}
