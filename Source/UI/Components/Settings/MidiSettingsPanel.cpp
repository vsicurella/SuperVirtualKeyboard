    /*
  ==============================================================================

    MidiSettingsPanel.cpp
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MidiSettingsPanel.h"

MidiSettingsPanel::MidiSettingsPanel(SvkPluginState* pluginStateIn)
    : SvkSettingsPanel("MidiSettingsPanel", pluginStateIn, 
        { 
            "Filters",
            "Devices"
        },
        { 
            IDs::periodShift,
            IDs::transposeAmt,
            IDs::keyboardMidiChannel,
            IDs::midiInputName,
            IDs::midiOutputName 
        },
        { 
            SvkControlProperties(ControlTypeNames::SliderControl, "Period Shift", true, 0, 0, -10, 10),
            SvkControlProperties(ControlTypeNames::SliderControl, "Transpose", true, 0, 0, -128, 128),
            SvkControlProperties(ControlTypeNames::SliderControl, "MIDI Channel Out", true, 0, 1, 1, 16),
            SvkControlProperties(ControlTypeNames::MenuControl, "Midi Input Device", true, 1),
            SvkControlProperties(ControlTypeNames::MenuControl, "Midi Output Device", true, 1)
        }
    )
{
    Array<Identifier> labelledControlIDs =
    {
        IDs::periodShift,
        IDs::transposeAmt,
        IDs::keyboardMidiChannel,
        IDs::midiInputName,
        IDs::midiOutputName
    };

    for (auto id : labelledControlIDs)
    {
        idToLabelledControl.set(id, static_cast<LabelledComponent*>(idToControl[id]));
    }

    midiProcessor = pluginState->getMidiProcessor();

    idToLabelledControl[IDs::periodShift]->setComponentSize(100, 24);
    periodShiftSlider = LabelledComponent::getComponentPointer<Slider>(idToLabelledControl[IDs::periodShift]);
    periodShiftSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    periodShiftSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 24);
    periodShiftSlider->setValue(midiProcessor->getPeriodShift());
    periodShiftSlider->addListener(this);

    idToLabelledControl[IDs::transposeAmt]->setComponentSize(100, 24);
    transposeSlider = LabelledComponent::getComponentPointer<Slider>(idToLabelledControl[IDs::transposeAmt]);
    transposeSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    transposeSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 24);
    transposeSlider->setValue(midiProcessor->getTransposeAmt());
    transposeSlider->addListener(this);

    idToLabelledControl[IDs::keyboardMidiChannel]->setComponentSize(100, 24);
    midiChannelSlider = LabelledComponent::getComponentPointer<Slider>(idToLabelledControl[IDs::keyboardMidiChannel]);
    midiChannelSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    midiChannelSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 24);
    midiChannelSlider->setValue(midiProcessor->getMidiChannelOut());
    midiChannelSlider->addListener(this);

    // Setup device settings if on standalone version, or hide
    if (JUCEApplication::isStandaloneApp())
    {
        inputBoxLabelled = static_cast<LabelledComponent*>(idToControl[IDs::midiInputName]);
        inputBoxLabelled->setComponentSize(320, 24);
        inputBox = LabelledComponent::getComponentPointer<ComboBox>(inputBoxLabelled);
        inputBox->addListener(this);

        outputBoxLabelled = static_cast<LabelledComponent*>(idToControl[IDs::midiOutputName]);
        outputBoxLabelled->setComponentSize(320, 24);
        outputBox = LabelledComponent::getComponentPointer<ComboBox>(outputBoxLabelled);
        outputBox->addListener(this);
    }
    else
    {
        controls.removeObject(idToControl[IDs::midiInputName], true);
        controls.removeObject(idToControl[IDs::midiOutputName], true);

        sectionHeaderLabels.remove(1);
        flexParent.items.remove(1);
    }
}

MidiSettingsPanel::~MidiSettingsPanel()
{
}

void MidiSettingsPanel::visibilityChanged()
{
    if (isVisible() && JUCEApplication::isStandaloneApp())
    {
        refreshDevices();
        startTimer(1000);
    }
    else
    {
        stopTimer();
    }
}

void MidiSettingsPanel::sliderValueChanged(Slider* sliderThatChanged)
{
    if (sliderThatChanged == periodShiftSlider)
    {
        midiProcessor->setPeriodShift(sliderThatChanged->getValue());
    }

    else if (sliderThatChanged == transposeSlider)
    {
        midiProcessor->setTransposeAmt(sliderThatChanged->getValue());
    }

    else if (sliderThatChanged == periodShiftSlider)
    {
        midiProcessor->setMidiChannelOut(sliderThatChanged->getValue());
    }
}

void MidiSettingsPanel::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
    // Midi Input Changed
    if (inputBox == comboBoxThatChanged)
    {
        MidiDeviceInfo& device = availableIns.getReference(inputBox->getSelectedId() - 1);
        DBG("Midi Input Selected: " + device.name);
        pluginState->getMidiProcessor()->setMidiInput(device.identifier);
    }

    // Midi Output Changed
    else if (outputBox == comboBoxThatChanged)
    {
        MidiDeviceInfo& device = availableOuts.getReference(outputBox->getSelectedId() - 1);
        DBG("Midi Output Selected: " + device.name);
        pluginState->getMidiProcessor()->setMidiOutput(device.identifier);
    }
}

void MidiSettingsPanel::timerCallback()
{
    refreshDevices();
}

void MidiSettingsPanel::refreshDevices()
{
    Array<MidiDeviceInfo> inputDevices = MidiInput::getAvailableDevices();

    if (availableIns != inputDevices && !inputBox->isPopupActive())
    {
        availableIns = inputDevices;
        inputBox->clear(dontSendNotification);
        inputBox->setText(pluginState->getMidiProcessor()->getInputName(), dontSendNotification);

        int i = 1;
        for (auto device : availableIns)
        {
            inputBox->addItem(device.name, i++);
        }
    }

    Array<MidiDeviceInfo> outputDevices = MidiOutput::getAvailableDevices();

    if (availableOuts != outputDevices && !outputBox->isPopupActive())
    {
        availableOuts = outputDevices;
        outputBox->clear(dontSendNotification);
        outputBox->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

        int i = 1;
        for (auto device : availableOuts)
        {
            outputBox->addItem(device.name, i++);
        }
    }
}