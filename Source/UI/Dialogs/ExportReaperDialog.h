/*
  ==============================================================================

    ExportReaperDialog.h
    Created: 15 Jul 2026

    Pop-up (CallOutBox) dialog for configuring a Reaper MIDI Note Name export.
    Mirrors ExportKbmDialog's structure.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Components/LabelledComponent.h"
#include "../../IO/ReaperWriter.h"
#include "../../PluginSettings.h"

//==============================================================================
class ExportReaperDialog  : public Component
{
public:
    ExportReaperDialog(const Mode* modeIn, SvkPluginSettings* settingsIn)
        : mode(modeIn), settings(settingsIn)
    {
        ReaperWriter::Options options = settings->getReaperOptions();

        // Degree numbering: mode degrees (id 1) vs scale degrees (id 2).
        degreeTypeBox = new ComboBox("DegreeTypeBox");
        degreeTypeBox->addItem("Mode Degrees", 1);
        degreeTypeBox->addItem("Scale Degrees", 2);
        degreeTypeBox->setSelectedId(options.useScaleDegrees ? 2 : 1, dontSendNotification);
        degreeTypeBox->onChange = [this]() { updateAccidentalControlsEnabled(); };
        degreeTypeLabel.reset(new LabelledComponent(*degreeTypeBox, "Degree Numbering:"));
        addAndMakeVisible(degreeTypeLabel.get());

        // Accidental spelling for chromatic notes; only meaningful under Mode Degrees.
        accidentalFormatBox = new ComboBox("AccidentalFormatBox");
        accidentalFormatBox->addItem("Decimals", 1);
        accidentalFormatBox->addItem("Ups & Downs (Prefer Ups)", 2);
        accidentalFormatBox->addItem("Ups & Downs (Prefer Downs)", 3);
        accidentalFormatBox->setSelectedId(accidentalStyleToId(options.accidentalStyle), dontSendNotification);
        accidentalFormatLabel.reset(new LabelledComponent(*accidentalFormatBox, "Accidentals Format:"));
        addAndMakeVisible(accidentalFormatLabel.get());

        keyCenterNoteSld = new Slider(Slider::SliderStyle::IncDecButtons, Slider::TextBoxLeft);
        keyCenterNoteSld->setRange(0, 127, 1);
        keyCenterNoteSld->setTextBoxStyle(Slider::TextBoxLeft, false, 48, 24);
        keyCenterNoteSld->setValue(options.keyCenterNote, dontSendNotification);
        keyCenterNoteLabel.reset(new LabelledComponent(*keyCenterNoteSld, "Key Center MIDI Note:"));
        addAndMakeVisible(keyCenterNoteLabel.get());

        includeOctavesButton.reset(new ToggleButton("Include Octaves"));
        includeOctavesButton->setToggleState(options.includeOctaves, dontSendNotification);
        includeOctavesButton->onClick = [this]() { updateOctaveControlsEnabled(); };
        addAndMakeVisible(includeOctavesButton.get());

        delimiterEditor = new TextEditor("DelimiterEditor");
        delimiterEditor->setInputRestrictions(8);
        delimiterEditor->setText(options.octaveDelimiter, dontSendNotification);
        delimiterLabel.reset(new LabelledComponent(*delimiterEditor, "Octave Delimiter:"));
        addAndMakeVisible(delimiterLabel.get());

        keyCenterOctaveSld = new Slider(Slider::SliderStyle::IncDecButtons, Slider::TextBoxLeft);
        keyCenterOctaveSld->setRange(-20, 20, 1);
        keyCenterOctaveSld->setTextBoxStyle(Slider::TextBoxLeft, false, 48, 24);
        keyCenterOctaveSld->setValue(options.keyCenterOctave, dontSendNotification);
        keyCenterOctaveLabel.reset(new LabelledComponent(*keyCenterOctaveSld, "Key Center Octave:"));
        addAndMakeVisible(keyCenterOctaveLabel.get());

        saveButton.reset(new TextButton("SaveButton"));
        saveButton->setButtonText("Save");
        saveButton->onClick = [this]() { saveAndClose(); };
        addAndMakeVisible(saveButton.get());

        cancelButton.reset(new TextButton("CancelButton"));
        cancelButton->setButtonText("Cancel");
        cancelButton->onClick = [this]() { closeCallout(); };
        addAndMakeVisible(cancelButton.get());

        updateOctaveControlsEnabled();
        updateAccidentalControlsEnabled();

        setSize(360, 264);
    }

    ~ExportReaperDialog() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);
    }

    void resized() override
    {
        degreeTypeLabel->setComponentSize(120, 24);
        degreeTypeLabel->setTopLeftPosition(8, 8);

        accidentalFormatLabel->setComponentSize(180, 24);
        accidentalFormatLabel->setTopLeftPosition(8, degreeTypeLabel->getBottom() + 8);

        keyCenterNoteLabel->setComponentSize(88, 24);
        keyCenterNoteLabel->setTopLeftPosition(8, accidentalFormatLabel->getBottom() + 8);

        includeOctavesButton->setBounds(8, keyCenterNoteLabel->getBottom() + 8, getWidth() - 16, 24);

        delimiterLabel->setComponentSize(88, 24);
        delimiterLabel->setTopLeftPosition(8, includeOctavesButton->getBottom() + 8);

        keyCenterOctaveLabel->setComponentSize(88, 24);
        keyCenterOctaveLabel->setTopLeftPosition(8, delimiterLabel->getBottom() + 8);

        double unit = proportionOfWidth(1 / 17.0f);
        saveButton->setBounds(unit * 2, getHeight() - 40, unit * 5, 36);
        cancelButton->setBounds(unit * 10, saveButton->getY(), unit * 5, 36);
    }

    void saveAndClose()
    {
        chooser = std::make_unique<FileChooser>("Save Reaper Note Names...", getDefaultDirectory(), "*.txt");

        chooser->launchAsync(FileBrowserComponent::FileChooserFlags::saveMode | FileBrowserComponent::FileChooserFlags::warnAboutOverwriting,
            [this](const FileChooser& fc)
            {
                File fileOut = fc.getResult();

                // Empty result means the native dialog was cancelled.
                if (fileOut == File())
                    return;

                ReaperWriter::Options options = gatherOptions();

                ReaperWriter writer(mode, options);
                if (writer.write(fileOut))
                {
                    settings->setReaperOptions(options);
                    closeCallout();
                }
                else
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "File Write Error", "Could not write Reaper note name file", "Ok");
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

    ReaperWriter::Options gatherOptions() const
    {
        ReaperWriter::Options options;
        options.useScaleDegrees = degreeTypeBox->getSelectedId() == 2;
        options.accidentalStyle = idToAccidentalStyle(accidentalFormatBox->getSelectedId());
        options.includeOctaves = includeOctavesButton->getToggleState();
        options.octaveDelimiter = delimiterEditor->getText();
        options.keyCenterNote = (int) keyCenterNoteSld->getValue();
        options.keyCenterOctave = (int) keyCenterOctaveSld->getValue();
        return options;
    }

    // The delimiter and octave index only matter when octaves are included.
    void updateOctaveControlsEnabled()
    {
        bool octavesOn = includeOctavesButton->getToggleState();
        delimiterEditor->setEnabled(octavesOn);
        keyCenterOctaveSld->setEnabled(octavesOn);
    }

    // Accidentals only exist under Mode Degrees; Scale Degrees numbers every note.
    void updateAccidentalControlsEnabled()
    {
        accidentalFormatBox->setEnabled(degreeTypeBox->getSelectedId() == 1);
    }

    static int accidentalStyleToId(ReaperWriter::Options::AccidentalStyle style)
    {
        switch (style)
        {
            case ReaperWriter::Options::AccidentalStyle::UpsPreferUp:   return 2;
            case ReaperWriter::Options::AccidentalStyle::UpsPreferDown: return 3;
            default:                                                    return 1;
        }
    }

    static ReaperWriter::Options::AccidentalStyle idToAccidentalStyle(int id)
    {
        switch (id)
        {
            case 2:  return ReaperWriter::Options::AccidentalStyle::UpsPreferUp;
            case 3:  return ReaperWriter::Options::AccidentalStyle::UpsPreferDown;
            default: return ReaperWriter::Options::AccidentalStyle::Decimals;
        }
    }

    File getDefaultDirectory() const
    {
        // Prefer Reaper's MIDINoteNames folder if it exists, else Documents.
        File defaultFolder;
        if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::MacOSX) != 0)
        {
            defaultFolder = File::getSpecialLocation(File::userApplicationDataDirectory)
                .getChildFile("Application Support").getChildFile("REAPER").getChildFile("MIDINoteNames");
        }
        else if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::Windows) != 0)
        {
            defaultFolder = File::getSpecialLocation(File::userHomeDirectory)
                .getChildFile("AppData").getChildFile("Roaming").getChildFile("REAPER").getChildFile("MIDINoteNames");
        }

        if (!defaultFolder.isDirectory())
            defaultFolder = File::getSpecialLocation(File::userDocumentsDirectory);

        return defaultFolder;
    }

    const Mode* mode;
    SvkPluginSettings* settings;

    std::unique_ptr<LabelledComponent> degreeTypeLabel;
    ComboBox* degreeTypeBox;

    std::unique_ptr<LabelledComponent> accidentalFormatLabel;
    ComboBox* accidentalFormatBox;

    std::unique_ptr<LabelledComponent> keyCenterNoteLabel;
    Slider* keyCenterNoteSld;

    std::unique_ptr<ToggleButton> includeOctavesButton;

    std::unique_ptr<LabelledComponent> delimiterLabel;
    TextEditor* delimiterEditor;

    std::unique_ptr<LabelledComponent> keyCenterOctaveLabel;
    Slider* keyCenterOctaveSld;

    std::unique_ptr<FileChooser> chooser;

    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> cancelButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExportReaperDialog)
};
