    /*
  ==============================================================================

    MidiSettingsPanel.cpp
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MidiSettingsPanel.h"

MidiSettingsPanel::MidiSettingsPanel(SvkState& presetIn)
    : SvkSettingsPanel("MidiSettingsPanel", presetIn, 
        { 
            "Filters",
            "Devices"
        },
        { 
            SvkProperty::periodShift,
            SvkProperty::transposeAmt,
            SvkProperty::keyboardMidiChannel,
            SvkProperty::midiInputName,
            SvkProperty::midiOutputName 
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
        SvkProperty::periodShift,
        SvkProperty::transposeAmt,
        SvkProperty::keyboardMidiChannel,
        SvkProperty::midiInputName,
        SvkProperty::midiOutputName
    };

    for (auto id : labelledControlIDs)
    {
        idToLabelledControl.set(id, static_cast<LabelledComponent*>(idToControl[id]));
    }

    //midiProcessor = pluginState->getMidiProcessor();

    idToLabelledControl[SvkProperty::periodShift]->setComponentSize(100, 24);
    periodShiftSlider = LabelledComponent::getComponentPointer<Slider>(idToLabelledControl[SvkProperty::periodShift]);
    periodShiftSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    periodShiftSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 24);
    periodShiftSlider->setValue(preset.getPeriodShift());
    periodShiftSlider->addListener(this);

    idToLabelledControl[SvkProperty::transposeAmt]->setComponentSize(100, 24);
    transposeSlider = LabelledComponent::getComponentPointer<Slider>(idToLabelledControl[SvkProperty::transposeAmt]);
    transposeSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    transposeSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 24);
    transposeSlider->setValue(preset.getTransposeAmount());
    transposeSlider->addListener(this);

    idToLabelledControl[SvkProperty::keyboardMidiChannel]->setComponentSize(100, 24);
    midiChannelSlider = LabelledComponent::getComponentPointer<Slider>(idToLabelledControl[SvkProperty::keyboardMidiChannel]);
    midiChannelSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    midiChannelSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 24);
    midiChannelSlider->setValue(preset.getMidiChannelOut());
    midiChannelSlider->addListener(this);

    // Setup device settings if on standalone version, or hide
    if (JUCEApplication::isStandaloneApp())
    {
        inputBoxLabelled = static_cast<LabelledComponent*>(idToControl[SvkProperty::midiInputName]);
        inputBoxLabelled->setComponentSize(320, 24);
        inputBox = LabelledComponent::getComponentPointer<ComboBox>(inputBoxLabelled);
        inputBox->addListener(this);

        outputBoxLabelled = static_cast<LabelledComponent*>(idToControl[SvkProperty::midiOutputName]);
        outputBoxLabelled->setComponentSize(320, 24);
        outputBox = LabelledComponent::getComponentPointer<ComboBox>(outputBoxLabelled);
        outputBox->addListener(this);
    }
    else
    {
        controls.removeObject(idToControl[SvkProperty::midiInputName], true);
        controls.removeObject(idToControl[SvkProperty::midiOutputName], true);

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
        preset.setPeriodShift(sliderThatChanged->getValue());
    }

    else if (sliderThatChanged == transposeSlider)
    {
        preset.setTransposeAmount(sliderThatChanged->getValue());
    }

    else if (sliderThatChanged == midiChannelSlider)
    {
        preset.setMidiChannelOut((int)sliderThatChanged->getValue());
    }
}

void MidiSettingsPanel::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
    // Midi Input Changed
    if (inputBox == comboBoxThatChanged)
    {
        int idx = inputBox->getSelectedId() - 1;
        if (idx >= 0 && idx < availableIns.size())
            preset.setMidiInputDevice(availableIns.getReference(idx));
    }

    // Midi Output Changed
    else if (outputBox == comboBoxThatChanged)
    {
        int idx = outputBox->getSelectedId() - 1;
        if (idx >= 0 && idx < availableOuts.size())
            preset.setMidiOutputDevice(availableOuts.getReference(idx));
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

        int i = 1;
        for (auto& device : availableIns)
            inputBox->addItem(device.name, i++);

        String currentIn = preset.getMidiInputName();
        for (int i = 0; i < availableIns.size(); ++i)
        {
            if (availableIns[i].name == currentIn)
            {
                inputBox->setSelectedId(i + 1, dontSendNotification);
                break;
            }
        }
    }

    Array<MidiDeviceInfo> outputDevices = MidiOutput::getAvailableDevices();

    if (availableOuts != outputDevices && !outputBox->isPopupActive())
    {
        availableOuts = outputDevices;
        outputBox->clear(dontSendNotification);

        int i = 1;
        for (auto& device : availableOuts)
            outputBox->addItem(device.name, i++);

        String currentOut = preset.getMidiOutputName();
        for (int i = 0; i < availableOuts.size(); ++i)
        {
            if (availableOuts[i].name == currentOut)
            {
                outputBox->setSelectedId(i + 1, dontSendNotification);
                break;
            }
        }
    }
}