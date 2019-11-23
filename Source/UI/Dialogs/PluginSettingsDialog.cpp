/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginSettingsDialog.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PluginSettingsDialog::PluginSettingsDialog (SvkPluginState* pluginStateIn)
    : pluginState(pluginStateIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("PluginSettingsDialog");
    presetDirectoryText.reset (new TextEditor ("Preset Directory Text"));
    addAndMakeVisible (presetDirectoryText.get());
    presetDirectoryText->setMultiLine (false);
    presetDirectoryText->setReturnKeyStartsNewLine (false);
    presetDirectoryText->setReadOnly (true);
    presetDirectoryText->setScrollbarsShown (true);
    presetDirectoryText->setCaretVisible (false);
    presetDirectoryText->setPopupMenuEnabled (true);
    presetDirectoryText->setText (String());

    presetDirectoryText->setBounds (128, 32, 320, 24);

    presetDirectoryLbl.reset (new Label ("Preset Directory Label",
                                         TRANS("Preset directory:")));
    addAndMakeVisible (presetDirectoryLbl.get());
    presetDirectoryLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    presetDirectoryLbl->setJustificationType (Justification::centredLeft);
    presetDirectoryLbl->setEditable (false, false, false);
    presetDirectoryLbl->setColour (TextEditor::textColourId, Colours::black);
    presetDirectoryLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    presetDirectoryLbl->setBounds (16, 32, 112, 24);

    presetDirectoryBtn.reset (new TextButton ("Preset Directory Button"));
    addAndMakeVisible (presetDirectoryBtn.get());
    presetDirectoryBtn->setButtonText (TRANS("..."));
    presetDirectoryBtn->addListener (this);

    presetDirectoryBtn->setBounds (456, 32, 23, 24);

    modeDirectoryText.reset (new TextEditor ("Mode Directory Text"));
    addAndMakeVisible (modeDirectoryText.get());
    modeDirectoryText->setMultiLine (false);
    modeDirectoryText->setReturnKeyStartsNewLine (false);
    modeDirectoryText->setReadOnly (true);
    modeDirectoryText->setScrollbarsShown (true);
    modeDirectoryText->setCaretVisible (false);
    modeDirectoryText->setPopupMenuEnabled (true);
    modeDirectoryText->setText (String());

    modeDirectoryText->setBounds (128, 72, 320, 24);

    modeDirectoryLbl.reset (new Label ("Mode Directory Label",
                                       TRANS("Mode directory:")));
    addAndMakeVisible (modeDirectoryLbl.get());
    modeDirectoryLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeDirectoryLbl->setJustificationType (Justification::centredLeft);
    modeDirectoryLbl->setEditable (false, false, false);
    modeDirectoryLbl->setColour (TextEditor::textColourId, Colours::black);
    modeDirectoryLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeDirectoryLbl->setBounds (20, 72, 112, 24);

    modeDirectoryBtn.reset (new TextButton ("Mode Directory Button"));
    addAndMakeVisible (modeDirectoryBtn.get());
    modeDirectoryBtn->setButtonText (TRANS("..."));
    modeDirectoryBtn->addListener (this);

    modeDirectoryBtn->setBounds (456, 72, 23, 24);

    settingsDirectoryText.reset (new TextEditor ("Settings Directory Text"));
    addAndMakeVisible (settingsDirectoryText.get());
    settingsDirectoryText->setMultiLine (false);
    settingsDirectoryText->setReturnKeyStartsNewLine (false);
    settingsDirectoryText->setReadOnly (true);
    settingsDirectoryText->setScrollbarsShown (true);
    settingsDirectoryText->setCaretVisible (false);
    settingsDirectoryText->setPopupMenuEnabled (true);
    settingsDirectoryText->setText (String());

    settingsDirectoryText->setBounds (128, 112, 320, 24);

    settingsDirectoryLbl.reset (new Label ("Settings Directory Label",
                                           TRANS("Settings directory:")));
    addAndMakeVisible (settingsDirectoryLbl.get());
    settingsDirectoryLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    settingsDirectoryLbl->setJustificationType (Justification::centredLeft);
    settingsDirectoryLbl->setEditable (false, false, false);
    settingsDirectoryLbl->setColour (TextEditor::textColourId, Colours::black);
    settingsDirectoryLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    settingsDirectoryLbl->setBounds (16, 112, 111, 24);

    settingsDirectoryBtn.reset (new TextButton ("Settings Directory Button"));
    addAndMakeVisible (settingsDirectoryBtn.get());
    settingsDirectoryBtn->setButtonText (TRANS("..."));
    settingsDirectoryBtn->addListener (this);

    settingsDirectoryBtn->setBounds (456, 112, 23, 24);

    localDirectoryBtn.reset (new ToggleButton ("Local Directory Button"));
    addAndMakeVisible (localDirectoryBtn.get());
    localDirectoryBtn->setButtonText (TRANS("Create Local Directories"));
    localDirectoryBtn->addListener (this);
    localDirectoryBtn->setToggleState (true, dontSendNotification);

    localDirectoryBtn->setBounds (24, 153, 184, 24);

    headerLbl.reset (new Label ("Header Label",
                                TRANS("Global Preferences")));
    addAndMakeVisible (headerLbl.get());
    headerLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    headerLbl->setJustificationType (Justification::centredLeft);
    headerLbl->setEditable (false, false, false);
    headerLbl->setColour (TextEditor::textColourId, Colours::black);
    headerLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    midiDeviceBox.reset (new ComboBox ("Midi Outputs"));
    addAndMakeVisible (midiDeviceBox.get());
    midiDeviceBox->setEditableText (false);
    midiDeviceBox->setJustificationType (Justification::centredLeft);
    midiDeviceBox->setTextWhenNothingSelected (TRANS("No Midi Output"));
    midiDeviceBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    midiDeviceBox->addListener (this);

    midiDeviceBox->setBounds (128, 200, 320, 24);

    midiOutputLbl.reset (new Label ("Midi Output Label",
                                    TRANS("Midi Outputs:")));
    addAndMakeVisible (midiOutputLbl.get());
    midiOutputLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    midiOutputLbl->setJustificationType (Justification::centredLeft);
    midiOutputLbl->setEditable (false, false, false);
    midiOutputLbl->setColour (TextEditor::textColourId, Colours::black);
    midiOutputLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    midiOutputLbl->setBounds (32, 200, 111, 24);

    bluetoothBtn.reset (new TextButton ("MIDIBluetoothDevices"));
    addAndMakeVisible (bluetoothBtn.get());
    bluetoothBtn->setButtonText (TRANS("MIDI Bluetooth Devices..."));
    bluetoothBtn->addListener (this);

    bluetoothBtn->setBounds (600, 200, 176, 24);


    //[UserPreSize]
    presetDirectoryText->setText(pluginState->getPluginSettings()->getPresetPath());
    modeDirectoryText->setText(pluginState->getPluginSettings()->getModePath());
    settingsDirectoryText->setText(pluginState->getPluginSettings()->getSettingsPath());


#if JUCE_IOS || JUCE_ANDROID || JUCE_LINUX || JUCE_DEBUG
    availableOuts = pluginState->getMidiProcessor()->getAvailableOutputs();

    for (int i = 0; i < availableOuts.size(); i++)
    {
        midiDeviceBox->addItem(availableOuts[i].name, i+1);
    }

    midiDeviceBox->setText(pluginState->getMidiProcessor()->getOutputName());
    startTimer(1000);
#else
    midiOutputLbl->setVisible(false);
    midiDeviceBox->setVisible(false);
#endif
    //[/UserPreSize]

    setSize (508, 250);


    //[Constructor] You can add your own custom stuff here..
    if (!BluetoothMidiDevicePairingDialogue::isAvailable())
        bluetoothBtn->setEnabled(false);

    bluetoothBtn->onClick = []
    {
        RuntimePermissions::request (RuntimePermissions::bluetoothMidi, [] (bool wasGranted)
        {
            if (wasGranted)
                BluetoothMidiDevicePairingDialogue::open();
        });
    };

    //[/Constructor]
}

PluginSettingsDialog::~PluginSettingsDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	removeAllChangeListeners();
    //[/Destructor_pre]

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
    bluetoothBtn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginSettingsDialog::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginSettingsDialog::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    headerLbl->setBounds (proportionOfWidth (0.5000f) - (127 / 2), 0, 127, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PluginSettingsDialog::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == presetDirectoryBtn.get())
    {
        //[UserButtonCode_presetDirectoryBtn] -- add your button handler code here..
        File newPresetDirectory = findDirectory("Select Preset Directory");
        if (newPresetDirectory.exists())
        {
            pluginState->getPluginSettings()->setPresetDirectory(newPresetDirectory);
            presetDirectoryText->setText(newPresetDirectory.getFullPathName());
        }
        //[/UserButtonCode_presetDirectoryBtn]
    }
    else if (buttonThatWasClicked == modeDirectoryBtn.get())
    {
        //[UserButtonCode_modeDirectoryBtn] -- add your button handler code here..
        File newModeDirectory = findDirectory("Select Mode Directory");
        if (newModeDirectory.exists())
        {
            pluginState->getPluginSettings()->setModeDirectory(newModeDirectory);
            modeDirectoryText->setText(newModeDirectory.getFullPathName());
        }
        //[/UserButtonCode_modeDirectoryBtn]
    }
    else if (buttonThatWasClicked == settingsDirectoryBtn.get())
    {
        //[UserButtonCode_settingsDirectoryBtn] -- add your button handler code here..
        File newSettingsDirectory = findDirectory("Select Settings Directory");
        if (newSettingsDirectory.exists())
        {
            pluginState->getPluginSettings()->setSettingsDirectory(newSettingsDirectory);
            settingsDirectoryText->setText(newSettingsDirectory.getFullPathName());
        }
        //[/UserButtonCode_settingsDirectoryBtn]
    }
    else if (buttonThatWasClicked == localDirectoryBtn.get())
    {
        //[UserButtonCode_localDirectoryBtn] -- add your button handler code here..
        pluginState->getPluginSettings()->setCreatePresetFolder(localDirectoryBtn->getToggleState());
        //[/UserButtonCode_localDirectoryBtn]
    }
    else if (buttonThatWasClicked == bluetoothBtn.get())
    {
        //[UserButtonCode_bluetoothBtn] -- add your button handler code here..

        //[/UserButtonCode_bluetoothBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PluginSettingsDialog::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == midiDeviceBox.get())
    {
        //[UserComboBoxCode_midiDeviceBox] -- add your combo box handling code here..
        DBG("trijjered");
		pluginState->getMidiProcessor()->setMidiOutput(availableOuts.getUnchecked(midiDeviceBox->getSelectedId() - 1).identifier);
		sendChangeMessage();
        //[/UserComboBoxCode_midiDeviceBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PluginSettingsDialog::timerCallback()
{
    availableOuts = MidiOutput::getAvailableDevices();
    midiDeviceBox->clear();

    int i = 0;
    for (auto device : availableOuts)
    {
        midiDeviceBox->addItem(device.name, ++i);
    }

    midiDeviceBox->setText(pluginState->getMidiProcessor()->getOutputName());
}

ComboBox* PluginSettingsDialog::getMidiOutputBox()
{
	return midiDeviceBox.get();
}

File PluginSettingsDialog::findDirectory(const String prompt)
{
    File fileOut;
    FileChooser chooser(prompt, File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory));
    //chooser.browseForDirectory();

    if (chooser.getResult().exists())
    {
        fileOut = chooser.getResult();
    }

    return fileOut;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginSettingsDialog" componentName="PluginSettingsDialog"
                 parentClasses="public Component, public ChangeBroadcaster, private Timer"
                 constructorParams="SvkPluginState* pluginStateIn" variableInitialisers="pluginState(pluginStateIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="508" initialHeight="250">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="Preset Directory Text" id="a2079bd0bc4dc5c0" memberName="presetDirectoryText"
              virtualName="" explicitFocusOrder="0" pos="128 32 320 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <LABEL name="Preset Directory Label" id="9ebb9306afb3740d" memberName="presetDirectoryLbl"
         virtualName="" explicitFocusOrder="0" pos="16 32 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Preset directory:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Preset Directory Button" id="8d3dd2615b659e70" memberName="presetDirectoryBtn"
              virtualName="" explicitFocusOrder="0" pos="456 32 23 24" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="Mode Directory Text" id="b0410e37b6ddc10d" memberName="modeDirectoryText"
              virtualName="" explicitFocusOrder="0" pos="128 72 320 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <LABEL name="Mode Directory Label" id="760a507b727b3ab9" memberName="modeDirectoryLbl"
         virtualName="" explicitFocusOrder="0" pos="20 72 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode directory:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Mode Directory Button" id="1796e45b011fbb26" memberName="modeDirectoryBtn"
              virtualName="" explicitFocusOrder="0" pos="456 72 23 24" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="Settings Directory Text" id="3fc4591e37599390" memberName="settingsDirectoryText"
              virtualName="" explicitFocusOrder="0" pos="128 112 320 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <LABEL name="Settings Directory Label" id="bf6b1a713ba96217" memberName="settingsDirectoryLbl"
         virtualName="" explicitFocusOrder="0" pos="16 112 111 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Settings directory:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Settings Directory Button" id="7e80f511d98c5fc3" memberName="settingsDirectoryBtn"
              virtualName="" explicitFocusOrder="0" pos="456 112 23 24" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="Local Directory Button" id="b6216e25a807982f" memberName="localDirectoryBtn"
                virtualName="" explicitFocusOrder="0" pos="24 153 184 24" buttonText="Create Local Directories"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <LABEL name="Header Label" id="6df6057198db7be1" memberName="headerLbl"
         virtualName="" explicitFocusOrder="0" pos="50%c 0 127 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Global Preferences" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <COMBOBOX name="Midi Outputs" id="8ece1efe3fd87d84" memberName="midiDeviceBox"
            virtualName="" explicitFocusOrder="0" pos="128 200 320 24" editable="0"
            layout="33" items="" textWhenNonSelected="No Midi Output" textWhenNoItems="(no choices)"/>
  <LABEL name="Midi Output Label" id="e459f27b7dfb0123" memberName="midiOutputLbl"
         virtualName="" explicitFocusOrder="0" pos="32 200 111 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Midi Outputs:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <TEXTBUTTON name="MIDIBluetoothDevices" id="f5999cce512d75c9" memberName="bluetoothBtn"
              virtualName="" explicitFocusOrder="0" pos="600 200 176 24" buttonText="MIDI Bluetooth Devices..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

