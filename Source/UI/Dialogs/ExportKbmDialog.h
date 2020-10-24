/*
  ==============================================================================

    ExportKbmDialog.h
    Created: 24 Oct 2020 4:47:57pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Components/LabelledComponent.h"
#include "../../File IO/KbmWriter.h"

//==============================================================================
/*
*/
class ExportKbmDialog  : public Component
{
public:
    ExportKbmDialog(ValueTree mappingNodeIn, const Mode& modeInput, const Mode& modeOutput, const ModeMapper& mapperIn)
        : mappingNode(mappingNodeIn), inputMode(modeInput), outputMode(modeOutput), mapper(mapperIn)
    {
        if (!mappingNode.isValid())
            mappingNode = ValueTree(IDs::midiMapNode);

        refNoteSld = new Slider(Slider::SliderStyle::IncDecButtons, Slider::TextBoxLeft);
        refNoteSld->setRange(0, 127, 1);
        refNoteSld->setTextBoxStyle(Slider::TextBoxLeft, false, 48, 24);
        refNoteSldLabel.reset(new LabelledComponent(*refNoteSld, "Tuning Reference MIDI Note:"));
        addAndMakeVisible(refNoteSldLabel.get());

        refFreqEditor = new TextEditor("ReferenceFreqEditor");
        refFreqEditor->setInputRestrictions(0, "123456890.");
        refFreqEditLabel.reset(new LabelledComponent(*refFreqEditor, "Tuning Reference Frequency:"));
        addAndMakeVisible(refFreqEditLabel.get());

        autoFreqButton.reset(new TextButton("AutoFreqButton", "Use a standard tuning frequency based on selected MIDI note."));
        autoFreqButton->setButtonText("Auto");
        autoFreqButton->onClick = [&]() { refFreqEditor->setText(String(getStdFrequency(refNoteSld->getValue()))); };
        addAndMakeVisible(autoFreqButton.get());

        saveButton.reset(new TextButton("SaveButton"));
        saveButton->setButtonText("Save");
        saveButton->onClick = [&]() { ExportKbmDialog::saveAndClose(); };
        addAndMakeVisible(saveButton.get());

        cancelButton.reset(new TextButton("CancelButton"));
        cancelButton->setButtonText("Cancel");
        cancelButton->onClick = [&]() { delete getParentComponent(); };
        addAndMakeVisible(cancelButton.get());

        if (mappingNode.hasProperty(IDs::tuningRootNote))
            refNoteSld->setValue((int)mappingNode[IDs::tuningRootNote]);
        else
            refNoteSld->setValue(69);

        if (mappingNode.hasProperty(IDs::tuningRootFreq))
            refFreqEditor->setText(mappingNode[IDs::tuningRootFreq].toString());
        else
            refFreqEditor->setText("440.0");
    }

    ~ExportKbmDialog() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    }

    void resized() override
    {
        refNoteSldLabel->setComponentSize(88, 24);
        refNoteSldLabel->setTopLeftPosition(0, 8);
        refFreqEditLabel->setComponentSize(80, 24);
        refFreqEditLabel->setTopLeftPosition(0, refNoteSldLabel->getBottom() + 8);

        autoFreqButton->setBounds(refFreqEditLabel->getRight() + 4, refFreqEditLabel->getY(), 44, 24);

        double unit = proportionOfWidth(1 / 17.0f);
        saveButton->setBounds(unit * 2, getHeight() - 40, unit * 5, 36);
        cancelButton->setBounds(unit * 10, saveButton->getY(), unit * 5, 36);
    }

    void saveAndClose()
    {
        bool success = false;

        File initialDirectory;
        if (mappingNode.hasProperty(IDs::kbmFileLocation) && File::isAbsolutePath(mappingNode[IDs::kbmFileLocation].toString()))
            initialDirectory = File(mappingNode[IDs::kbmFileLocation]);

        FileChooser chooser("Save KBM file...", initialDirectory, "*.kbm");

        if (chooser.browseForFileToSave(true))
        {
            KbmWriter kbm = KbmWriter::fromModes(
                &inputMode, &outputMode, mapper, 0, 127, -1,
                refNoteSld->getValue(), refFreqEditor->getText().getDoubleValue()
            );

            if (kbm.writeTo(chooser.getResult()))
            {
                success = true;
                mappingNode.setProperty(IDs::tuningRootNote, refNoteSld->getValue(), nullptr);
                mappingNode.setProperty(IDs::tuningRootFreq, refFreqEditor->getText().getDoubleValue(), nullptr);
                mappingNode.setProperty(IDs::kbmFileLocation, chooser.getResult().getParentDirectory().getFullPathName(), nullptr);
            }
        }

        if (!success)
        {
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "File Write Error", "Could not write .kbm file", "Ok", this);
        }

        delete getParentComponent();
    }

private:

    ValueTree mappingNode;

    const Mode& inputMode;
    const Mode& outputMode;
    const ModeMapper& mapper;

    std::unique_ptr<LabelledComponent> refNoteSldLabel;
    Slider* refNoteSld;

    std::unique_ptr<LabelledComponent> refFreqEditLabel;
    TextEditor* refFreqEditor;

    std::unique_ptr<TextButton> autoFreqButton;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> cancelButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExportKbmDialog)
};
