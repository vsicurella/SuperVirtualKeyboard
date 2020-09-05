/*
  ==============================================================================

    GeneralSettingsPanel.cpp
    Created: 13 Nov 2019 7:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "GeneralSettingsPanel.h"

GeneralSettingsPanel::GeneralSettingsPanel(SvkPluginState* pluginStateIn)
    : pluginState(pluginStateIn)
{
	setName("PluginSettingsDialog");
	presetDirectoryText.reset(new TextEditor("Preset Directory Text"));
	addAndMakeVisible(presetDirectoryText.get());
	presetDirectoryText->setMultiLine(false);
	presetDirectoryText->setReturnKeyStartsNewLine(false);
	presetDirectoryText->setReadOnly(true);
	presetDirectoryText->setScrollbarsShown(true);
	presetDirectoryText->setCaretVisible(false);
	presetDirectoryText->setPopupMenuEnabled(true);
	presetDirectoryText->setText(String());

	presetDirectoryText->setBounds(128, 32, 320, 24);

	presetDirectoryLbl.reset(new Label("Preset Directory Label",
		TRANS("Preset directory:")));
	addAndMakeVisible(presetDirectoryLbl.get());
	presetDirectoryLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
	presetDirectoryLbl->setJustificationType(Justification::centredLeft);
	presetDirectoryLbl->setEditable(false, false, false);
	presetDirectoryLbl->setColour(TextEditor::textColourId, Colours::black);
	presetDirectoryLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	presetDirectoryLbl->setBounds(16, 32, 112, 24);

	presetDirectoryBtn.reset(new TextButton("Preset Directory Button"));
	addAndMakeVisible(presetDirectoryBtn.get());
	presetDirectoryBtn->setButtonText(TRANS("..."));
	presetDirectoryBtn->addListener(this);

	presetDirectoryBtn->setBounds(456, 32, 23, 24);

	modeDirectoryText.reset(new TextEditor("Mode Directory Text"));
	addAndMakeVisible(modeDirectoryText.get());
	modeDirectoryText->setMultiLine(false);
	modeDirectoryText->setReturnKeyStartsNewLine(false);
	modeDirectoryText->setReadOnly(true);
	modeDirectoryText->setScrollbarsShown(true);
	modeDirectoryText->setCaretVisible(false);
	modeDirectoryText->setPopupMenuEnabled(true);
	modeDirectoryText->setText(String());

	modeDirectoryText->setBounds(128, 72, 320, 24);

	modeDirectoryLbl.reset(new Label("Mode Directory Label",
		TRANS("Mode directory:")));
	addAndMakeVisible(modeDirectoryLbl.get());
	modeDirectoryLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
	modeDirectoryLbl->setJustificationType(Justification::centredLeft);
	modeDirectoryLbl->setEditable(false, false, false);
	modeDirectoryLbl->setColour(TextEditor::textColourId, Colours::black);
	modeDirectoryLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	modeDirectoryLbl->setBounds(20, 72, 112, 24);

	modeDirectoryBtn.reset(new TextButton("Mode Directory Button"));
	addAndMakeVisible(modeDirectoryBtn.get());
	modeDirectoryBtn->setButtonText(TRANS("..."));
	modeDirectoryBtn->addListener(this);

	modeDirectoryBtn->setBounds(456, 72, 23, 24);

	settingsDirectoryText.reset(new TextEditor("Settings Directory Text"));
	addAndMakeVisible(settingsDirectoryText.get());
	settingsDirectoryText->setMultiLine(false);
	settingsDirectoryText->setReturnKeyStartsNewLine(false);
	settingsDirectoryText->setReadOnly(true);
	settingsDirectoryText->setScrollbarsShown(true);
	settingsDirectoryText->setCaretVisible(false);
	settingsDirectoryText->setPopupMenuEnabled(true);
	settingsDirectoryText->setText(String());

	settingsDirectoryText->setBounds(128, 112, 320, 24);

	settingsDirectoryLbl.reset(new Label("Settings Directory Label",
		TRANS("Settings directory:")));
	addAndMakeVisible(settingsDirectoryLbl.get());
	settingsDirectoryLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
	settingsDirectoryLbl->setJustificationType(Justification::centredLeft);
	settingsDirectoryLbl->setEditable(false, false, false);
	settingsDirectoryLbl->setColour(TextEditor::textColourId, Colours::black);
	settingsDirectoryLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	settingsDirectoryLbl->setBounds(16, 112, 111, 24);

	settingsDirectoryBtn.reset(new TextButton("Settings Directory Button"));
	addAndMakeVisible(settingsDirectoryBtn.get());
	settingsDirectoryBtn->setButtonText(TRANS("..."));
	settingsDirectoryBtn->addListener(this);

	settingsDirectoryBtn->setBounds(456, 112, 23, 24);

	localDirectoryBtn.reset(new ToggleButton("Local Directory Button"));
	addAndMakeVisible(localDirectoryBtn.get());
	localDirectoryBtn->setButtonText(TRANS("Create Local Directories"));
	localDirectoryBtn->addListener(this);
	localDirectoryBtn->setToggleState(true, dontSendNotification);

	localDirectoryBtn->setBounds(24, 153, 184, 24);

	headerLbl.reset(new Label("Header Label",
		TRANS("Global Preferences")));
	addAndMakeVisible(headerLbl.get());
	headerLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Bold"));
	headerLbl->setJustificationType(Justification::centredLeft);
	headerLbl->setEditable(false, false, false);
	headerLbl->setColour(TextEditor::textColourId, Colours::black);
	headerLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	midiDeviceBox.reset(new ComboBox("Midi Outputs"));
	addAndMakeVisible(midiDeviceBox.get());
	midiDeviceBox->setEditableText(false);
	midiDeviceBox->setJustificationType(Justification::centredLeft);
	midiDeviceBox->setTextWhenNothingSelected(TRANS("No Midi Output"));
	midiDeviceBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
	midiDeviceBox->addListener(this);

	midiDeviceBox->setBounds(128, 200, 320, 24);

	midiOutputLbl.reset(new Label("Midi Output Label",
		TRANS("Midi Outputs:")));
	addAndMakeVisible(midiOutputLbl.get());
	midiOutputLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
	midiOutputLbl->setJustificationType(Justification::centredLeft);
	midiOutputLbl->setEditable(false, false, false);
	midiOutputLbl->setColour(TextEditor::textColourId, Colours::black);
	midiOutputLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	midiOutputLbl->setBounds(32, 200, 111, 24);


	//[UserPreSize]
	presetDirectoryText->setText(pluginState->getPluginSettings()->getPresetPath());
	modeDirectoryText->setText(pluginState->getPluginSettings()->getModePath());
	settingsDirectoryText->setText(pluginState->getPluginSettings()->getSettingsPath());


#if JUCE_IOS || JUCE_ANDROID || JUCE_LINUX || JUCE_DEBUG
	timerCallback();
	startTimer(750);
#else
	midiOutputLbl->setVisible(false);
	midiDeviceBox->setVisible(false);
#endif
	//[/UserPreSize]

	setSize(508, 250);


}

GeneralSettingsPanel::~GeneralSettingsPanel()
{
	removeAllChangeListeners();

	presetDirectoryText = nullptr;
	presetDirectoryLbl = nullptr;
	presetDirectoryBtn = nullptr;
	modeDirectoryText = nullptr;
	modeDirectoryLbl = nullptr;
	modeDirectoryBtn = nullptr;
	settingsDirectoryText = nullptr;
	settingsDirectoryLbl = nullptr;
	settingsDirectoryBtn = nullptr;
	localDirectoryBtn = nullptr;
	headerLbl = nullptr;
	midiDeviceBox = nullptr;
	midiOutputLbl = nullptr;

}


void GeneralSettingsPanel::paint(Graphics& g)
{
    
}

void GeneralSettingsPanel::resized()
{
	headerLbl->setBounds(proportionOfWidth(0.5000f) - (127 / 2), 0, 127, 24);
}

void GeneralSettingsPanel::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == presetDirectoryBtn.get())
	{
		File newPresetDirectory = findDirectory("Select Preset Directory");
		if (newPresetDirectory.exists())
		{
			pluginState->getPluginSettings()->setPresetDirectory(newPresetDirectory);
			presetDirectoryText->setText(newPresetDirectory.getFullPathName());
		}
	}
	else if (buttonThatWasClicked == modeDirectoryBtn.get())
	{
		File newModeDirectory = findDirectory("Select Mode Directory");
		if (newModeDirectory.exists())
		{
			pluginState->getPluginSettings()->setModeDirectory(newModeDirectory);
			modeDirectoryText->setText(newModeDirectory.getFullPathName());
		}
	}
	else if (buttonThatWasClicked == settingsDirectoryBtn.get())
	{
		File newSettingsDirectory = findDirectory("Select Settings Directory");
		if (newSettingsDirectory.exists())
		{
			pluginState->getPluginSettings()->setSettingsDirectory(newSettingsDirectory);
			settingsDirectoryText->setText(newSettingsDirectory.getFullPathName());
		}
	}
	else if (buttonThatWasClicked == localDirectoryBtn.get())
	{
		pluginState->getPluginSettings()->setCreatePresetFolder(localDirectoryBtn->getToggleState());
	}

}

void GeneralSettingsPanel::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{

	if (comboBoxThatHasChanged == midiDeviceBox.get())
	{
		int deviceNum = comboBoxThatHasChanged->getSelectedId() - 1;

		if (deviceNum >= 0)
			pluginState->getMidiProcessor()->setMidiOutput(availableOuts.getUnchecked(deviceNum).identifier);
	}
}

void GeneralSettingsPanel::timerCallback()
{
	Array<MidiDeviceInfo> devices = pluginState->getMidiProcessor()->getAvailableOutputs();
	midiDeviceBox->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

	if (availableOuts != devices && !midiDeviceBox->isPopupActive())
	{
		availableOuts = devices;
		midiDeviceBox->clear();

		int i = 0;
		for (auto device : availableOuts)
		{
			midiDeviceBox->addItem(device.name, ++i);
		}
	}
}

ComboBox* GeneralSettingsPanel::getMidiOutputBox()
{
	return midiDeviceBox.get();
}

File GeneralSettingsPanel::findDirectory(const String prompt)
{
	File fileOut;
	FileChooser chooser(prompt, File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory));
	chooser.browseForDirectory();

	if (chooser.getResult().exists())
	{
		fileOut = chooser.getResult();
	}

	return fileOut;
}
