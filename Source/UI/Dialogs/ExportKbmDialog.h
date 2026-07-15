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
#include "../../IO/KbmWriter.h"

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
            mappingNode = ValueTree(SvkProperty::midiMapNode);

        refNoteSld = new Slider(Slider::SliderStyle::IncDecButtons, Slider::TextBoxLeft);
        refNoteSld->setRange(0, 127, 1);
        refNoteSld->setTextBoxStyle(Slider::TextBoxLeft, false, 48, 24);
        refNoteSldLabel.reset(new LabelledComponent(*refNoteSld, "Tuning Reference MIDI Note:"));
        addAndMakeVisible(refNoteSldLabel.get());

        refFreqEditor = new TextEditor("ReferenceFreqEditor");
        refFreqEditor->setInputRestrictions(0, "1234567890.");
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
        cancelButton->onClick = [this]() { closeCallout(); };
        addAndMakeVisible(cancelButton.get());

        if (mappingNode.hasProperty(SvkProperty::tuningRootNote))
            refNoteSld->setValue((int)mappingNode[SvkProperty::tuningRootNote]);
        else
            refNoteSld->setValue(69);

        if (mappingNode.hasProperty(SvkProperty::tuningRootFreq))
            refFreqEditor->setText(mappingNode[SvkProperty::tuningRootFreq].toString());
        else
            refFreqEditor->setText("440.0");

        setSize(360, 140);
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
        // Default to the user's Documents folder; only reuse a previously saved
        // location if one was stored for this mapping.
        File initialDirectory = File::getSpecialLocation(File::userDocumentsDirectory);
        if (mappingNode.hasProperty(SvkProperty::kbmFileLocation) && File::isAbsolutePath(mappingNode[SvkProperty::kbmFileLocation].toString()))
            initialDirectory = File(mappingNode[SvkProperty::kbmFileLocation]);

        chooser = std::make_unique<FileChooser>("Save KBM file...", initialDirectory, "*.kbm");

        chooser->launchAsync(FileBrowserComponent::FileChooserFlags::saveMode | FileBrowserComponent::FileChooserFlags::warnAboutOverwriting, [this](const FileChooser& fc)
        {
            File fileOut = fc.getResult();

            // Empty result means the native dialog was cancelled.
            if (fileOut == File())
                return;

            KbmWriter kbm = KbmWriter::fromModes(
                &inputMode, &outputMode, mapper, 0, 127, -1,
                refNoteSld->getValue(), refFreqEditor->getText().getDoubleValue()
            );

            if (kbm.writeTo(fileOut))
            {
                mappingNode.setProperty(SvkProperty::tuningRootNote, refNoteSld->getValue(), nullptr);
                mappingNode.setProperty(SvkProperty::tuningRootFreq, refFreqEditor->getText().getDoubleValue(), nullptr);
                mappingNode.setProperty(SvkProperty::kbmFileLocation, fileOut.getParentDirectory().getFullPathName(), nullptr);

                closeCallout();
            }
            else
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "File Write Error", "Could not write .kbm file", "Ok");
            }
        });
    }

    // Dismisses the enclosing CallOutBox asynchronously (safe to call from a
    // child's callback — unlike deleting the parent component synchronously).
    void closeCallout()
    {
        if (auto* box = findParentComponentOfClass<CallOutBox>())
            box->dismiss();
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

    std::unique_ptr<FileChooser> chooser;

    std::unique_ptr<TextButton> autoFreqButton;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> cancelButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExportKbmDialog)
};
