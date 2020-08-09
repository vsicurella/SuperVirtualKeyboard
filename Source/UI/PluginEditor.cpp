/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "../PluginProcessor.h"

using namespace VirtualKeyboard;

//==============================================================================
SvkPluginEditor::SvkPluginEditor(SvkAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	pluginState(processor.getPluginState()),
	appCmdMgr(processor.getAppCmdMgr())
{
	setName("Super Virtual Keyboard");
	setResizable(true, true);
	setBroughtToFrontOnMouseClick(true);

	appCmdMgr->registerAllCommandsForTarget(this);
	appCmdMgr->setFirstCommandTarget(this);

	controlComponent.reset(new PluginControlComponent(processor.svkValueTree, appCmdMgr, pluginState->getPresetManager()));
	//controlComponent->connectToProcessor();
	addAndMakeVisible(controlComponent.get());

	virtualKeyboard = controlComponent->getKeyboard();
	virtualKeyboard->addListener(pluginState->getMidiProcessor());

	colorChooserWindow.reset(new ColorChooserWindow("Color Chooser", Colours::slateblue, DocumentWindow::closeButton));
	colorChooserWindow->setSize(450, 450);
	colorChooserWindow->addChangeListener(this);
	colorChooserWindow->addToDesktop();

	colorSelector.reset(new ColourSelector());
	colorSelector->setSize(450, 450);
	colorChooserWindow->setContentOwned(colorSelector.get(), true);
	colorSelector->addChangeListener(this);

	pluginState->addListener(this);

	//for (auto paramID : processor.getParamIDs())
	//{
	//	processor.svkValueTree.addParameterListener(paramID, this);
	//}
	//DBG("PluginEditor listening to parameters");

	mappingHelper.reset(new MappingHelper(pluginState));

	setMouseClickGrabsKeyboardFocus(true);
	addMouseListener(this, true);
	
	pluginEditorNode = ValueTree(IDs::pluginEditorNode);

	// Gui is recreated
	if (pluginState->pluginEditorNode.isValid() && pluginState->pluginEditorNode.getNumProperties() > 2)
	{
		pluginEditorNode = pluginState->pluginEditorNode;
		setSize(pluginEditorNode[IDs::windowBoundsW], pluginEditorNode[IDs::windowBoundsH]);
	}

	// Intialization
	else
	{
		pluginState->pluginEditorNode = pluginEditorNode;
		setSize(1000, 210);
	}

	setResizeLimits(750, 100, 10e4, 10e4);

	controlComponent->setBounds(getBounds());
	controlComponent->loadPresetNode(pluginState->getPresetNode());

#if (!JUCE_ANDROID && !JUCE_IOS)
	startTimerHz(30);
#endif
}

SvkPluginEditor::~SvkPluginEditor()
{
	//for (auto paramID : processor.getParamIDs())
	//{
	//	processor.svkValueTree.removeParameterListener(paramID, this);
	//}


	colorSelector->removeChangeListener(this);
	colorChooserWindow->removeChangeListener(this);
	virtualKeyboard->removeListener(pluginState->getMidiProcessor());
    pluginState->removeListener(this);
}

//==============================================================================

bool SvkPluginEditor::savePresetToFile()
{
    bool written = pluginState->savePresetToFile();
	if (written)
		DBG("file was saved");
	else
		DBG("not saved");

	return written;
}

bool SvkPluginEditor::saveMode()
{
	return pluginState->saveModeViewedToFile();
}

void SvkPluginEditor::showSaveMenu()
{
    return;
}

bool SvkPluginEditor::loadPreset()
{
	if (pluginState->loadPresetFromFile())
	{
		controlComponent->loadPresetNode(pluginState->getPresetNode());
		return true;
	}

	return false;
}

bool SvkPluginEditor::loadMode()
{
	if (pluginState->loadModeFromFile())
	{
		return true;
	}

	return false;
}

bool SvkPluginEditor::exportReaperMap()
{
	ReaperWriter rpp = ReaperWriter(pluginState->getModeViewed());
	return rpp.write_file();
}

bool SvkPluginEditor::exportAbletonMap()
{
	return false;
}

void SvkPluginEditor::showSettingsDialog()
{
    //updateScrollbarData();
    //controlComponent->setVisible(false);

    //settingsContainer.reset(new SettingsContainer(processor.svkValueTree, pluginState));
    //addAndMakeVisible(settingsContainer.get());
    //settingsContainer->setBounds(0, 0, controlComponent->getWidth(), controlComponent->getHeight());
    //settingsContainer->addChangeListener(this);
}

void SvkPluginEditor::commitCustomScale()
{
	String scaleSteps = controlComponent->getScaleEntryText();
	pluginState->setModeCustom(scaleSteps);
}

void SvkPluginEditor::setMode1()
{
	setMode1(controlComponent->getMode1BoxSelection());
}

void SvkPluginEditor::setMode1(int idIn)
{
    pluginState->handleModeSelection(0, idIn);
}

void SvkPluginEditor::setMode2()
{
	setMode2(controlComponent->getMode2BoxSelection());
}

void SvkPluginEditor::setMode2(int idIn)
{
    pluginState->handleModeSelection(1, idIn);
}

void SvkPluginEditor::setMode1Root()
{
	setMode1Root(controlComponent->getMode1Root());
}

void SvkPluginEditor::setMode1Root(int rootIn)
{
    pluginState->setModeSelectorRoot(0, rootIn);
}

void SvkPluginEditor::setMode2Root()
{
	setMode2Root(controlComponent->getMode2Root());
}

void SvkPluginEditor::setMode2Root(int rootIn)
{
    pluginState->setModeSelectorRoot(1, rootIn);
}

void SvkPluginEditor::setModeView()
{
	setModeView(controlComponent->getModeViewed());
}

void SvkPluginEditor::setModeView(int modeNumberIn)
{
	pluginState->setModeSelectorViewed(modeNumberIn);
    controlComponent->setScaleEntryText(pluginState->getModeViewed()->getStepsString());

	MidiKeyboardState* displayState = modeNumberIn == 0
		? pluginState->getMidiProcessor()->getOriginalKeyboardState()
		: pluginState->getMidiProcessor()->getRemappedKeyboardState();
	
	virtualKeyboard->displayKeyboardState(displayState);
}

void SvkPluginEditor::showModeInfo()
{
	modeInfo = new ModeInfoDialog(pluginState->getModeViewed());
	modeInfo->addChangeListener(this);

	CallOutBox::launchAsynchronously(modeInfo, controlComponent->getScaleTextEditor()->getScreenBounds(), nullptr);
}

void SvkPluginEditor::setMappingMode()
{
	setMappingMode(controlComponent->getMappingMode());
}

void SvkPluginEditor::setMappingMode(int mappingModeId)
{
	pluginState->setMapMode(mappingModeId);
}

void SvkPluginEditor::setMappingStyle()
{
	setMappingStyle(pluginState->getParameterValue(IDs::modeMappingStyle));
}

void SvkPluginEditor::setMappingStyle(int mapStyleId)
{
	pluginState->setMapStyle(mapStyleId);
}

void SvkPluginEditor::showMapOrderEditDialog()
{
    mapByOrderDialog = new MapByOrderDialog(pluginState->getModeMapper(), pluginState->getMode1(), pluginState->getMode2());
    CallOutBox::launchAsynchronously(mapByOrderDialog, controlComponent->getMappingStyleBox()->getScreenBounds(), nullptr);
}

void SvkPluginEditor::applyMap()
{
	pluginState->doMapping();
}

void SvkPluginEditor::beginColorEditing()
{
	colorChooserWindow->setVisible(true);
	virtualKeyboard->setUIMode(UIMode::editMode);
    isColorEditing = true;
}

//==============================================================================

void SvkPluginEditor::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);
}

void SvkPluginEditor::resized()
{	
	AudioProcessorEditor::resized();

	controlComponent->setSize(getWidth(), getHeight());

//    if (settingsContainer.get())
//         settingsContainer->setSize(getWidth(), getHeight());
	
	pluginEditorNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
	pluginEditorNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
}

//==============================================================================

void SvkPluginEditor::timerCallback()
{
	virtualKeyboard->repaint();
}

//==============================================================================

void SvkPluginEditor::userTriedToCloseWindow()
{
	if (colorChooserWindow->isVisible())
		colorChooserWindow->closeButtonPressed();

	setVisible(false);
}

//==============================================================================

void SvkPluginEditor::presetLoaded(ValueTree presetNodeIn)
{
	controlComponent->loadPresetNode(presetNodeIn);
}

void SvkPluginEditor::modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber)
{
	controlComponent->onModeViewedChange(modeIn);
}

void SvkPluginEditor::inputMappingChanged(NoteMap* inputNoteMap)
{
	virtualKeyboard->setInputNoteMap(inputNoteMap);
}

void SvkPluginEditor::customModeChanged(Mode* newCustomMode)
{
}

void SvkPluginEditor::modeInfoChanged(Mode* modeEdited)
{
	if (pluginState->getModeSelectorViewed() == 1)
		controlComponent->setMode2BoxText(modeEdited->getName());
	else
		controlComponent->setMode1BoxText(modeEdited->getName());
}

//==============================================================================

void SvkPluginEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	// Color editing is finished
	if (source == colorChooserWindow.get())
	{
		isColorEditing = false;

		//virtualKeyboard->updatePianoNode();
		//virtualKeyboard->updateKeyColors();
		virtualKeyboard->setUIMode(UIMode::playMode);
		colorChooserWindow->setVisible(false);
	}

	// Color changed
	if (source == colorSelector.get())
	{
		virtualKeyboard->getProperties().set(
			IDs::colorSelected,
			colorSelector->getCurrentColour().toString()
		);
	}

	// Mode Info Changed
	if (source == modeInfo)
	{
		pluginState->commitModeInfo();
		controlComponent->onModeViewedChange(pluginState->getModeViewed());
	}
        
    // Settings closed
//    if (source == settingsContainer.get())
//    {
//        settingsContainer->removeChangeListener(this);
//        settingsContainer->setVisible(false);
//        settingsContainer.reset();
//
//        controlComponent->setVisible(true);
//        controlComponent->resized();
//    }
}

//==============================================================================


File SvkPluginEditor::fileDialog(String message, bool forSaving)
{
	FileChooser chooser(message, File::getSpecialLocation(File::userDocumentsDirectory), "*.svk");

	if (forSaving)
		chooser.browseForFileToSave(true);
	else
		chooser.browseForFileToOpen();

	return chooser.getResult();
}

void SvkPluginEditor::parameterChanged(const String& paramID, float newValue)
{
	//DBG("PARAMETER EDITED: " + paramID + " = " + String(newValue));

	//if (paramID == IDs::mappingMode.toString())
	//{
	//	setMappingMode();
	//}
	//else if (paramID == IDs::modeMappingStyle.toString())
	//{
	//	setMappingStyle();
	//}
	//else if (paramID == IDs::pianoWHRatio.toString())
	//{
	//	virtualKeyboard->setKeySizeRatio(newValue);
	//}
	//else if (paramID == IDs::keyboardNumRows.toString())
	//{
	//	virtualKeyboard->setNumRows(newValue);
	//}
}

//==============================================================================

ApplicationCommandTarget* SvkPluginEditor::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void SvkPluginEditor::getAllCommands(Array<CommandID>& c)
{
	Array<CommandID> commands{
		IDs::CommandIDs::savePresetToFile,
		IDs::CommandIDs::saveMode,
		IDs::CommandIDs::loadPreset,
		IDs::CommandIDs::loadMode,
		IDs::CommandIDs::exportReaperMap,
		IDs::CommandIDs::exportAbletonMap,
        IDs::CommandIDs::showSettingsDialog,
		IDs::CommandIDs::commitCustomScale,
		IDs::CommandIDs::setMode1,
		IDs::CommandIDs::setMode2,
		IDs::CommandIDs::setMode1RootNote,
		IDs::CommandIDs::setMode2RootNote,
		IDs::CommandIDs::setModeViewed,
		IDs::CommandIDs::showModeInfo,
        IDs::CommandIDs::setMappingMode,
		IDs::CommandIDs::setMappingStyle,
        IDs::CommandIDs::showMapOrderEdit,
		IDs::CommandIDs::applyMapping,
		IDs::CommandIDs::beginColorEditing,
		IDs::CommandIDs::setPeriodShift,
		IDs::CommandIDs::setMidiChannelOut,
		//IDs::CommandIDs::showMidiNoteNumbers,
		//IDs::CommandIDs::setKeyStyle
	};

	c.addArray(commands);
}

void SvkPluginEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
	case IDs::CommandIDs::savePresetToFile:
		result.setInfo("Save Preset", "Save your custom layout to a file.", "Preset", 0);
		break;
	case IDs::CommandIDs::saveMode:
		result.setInfo("Save Mode", "Save the currently viewed mode.", "Preset", 0);
		break;
    case IDs::CommandIDs::showSaveMenu:
        result.setInfo("Show Saving Options", "Save current mode or whole preset.", "Preset", 0);
        break;
	case IDs::CommandIDs::loadPreset:
		result.setInfo("Load Preset", "Load a custom layout from a file.", "Preset", 0);
		break;
	case IDs::CommandIDs::loadMode:
		result.setInfo("Load Mode", "Load only the mode of a preset.", "Preset", 0);
		break;
    case IDs::CommandIDs::showLoadMenu:
        result.setInfo("Show Loading Options", "Load a mode or whole preset.", "Preset", 0);
        break;
	case IDs::CommandIDs::exportReaperMap:
		result.setInfo("Export for Reaper", "Exports the current preset to a MIDI Note Name text file for use in Reaper's piano roll.", "Preset", 0);
		break;
	case IDs::CommandIDs::exportAbletonMap:
		result.setInfo("Export for Ableton", "Exports the mode mapping to a MIDI file for to use in Ableton's piano roll for folding.", "Preset", 0);
		break;
    case IDs::CommandIDs::showExportMenu:
        result.setInfo("Show Export Options", "Shows different ways you can export a mode or preset.", "Preset", 0);
        break;
    case IDs::CommandIDs::showSettingsDialog:
        result.setInfo("Show Settings Dialog", "Change default directories", "Settings", 0);
        break;
	case IDs::CommandIDs::commitCustomScale:
		result.setInfo("Commit custom scale", "Registers the entered scale steps as the current custom scale.", "Preset", 0);
		break;
	case IDs::CommandIDs::setMode1:
		result.setInfo("Set Mode 1", "Loads the mode into the Mode 1 slot.", "Preset", 0);
		break;
	case IDs::CommandIDs::setMode2:
		result.setInfo("Set Mode 2", "Loads the mode into the Mode 2 slot.", "Preset", 0);
		break;
	case IDs::CommandIDs::setMode1RootNote:
		result.setInfo("Set Mode 1 Root", "Applies the selected root note for Mode 1.", "Preset", 0);
		break;
	case IDs::CommandIDs::setMode2RootNote:
		result.setInfo("Set Mode 2 Root", "Applies the selected root note for Mode 2.", "Preset", 0);
		break;
	case IDs::CommandIDs::setModeViewed:
		result.setInfo("Set Mode Viewed", "Shows the mode slot on the keyboard.", "Keyboard", 0);
		break;
    case IDs::CommandIDs::showModeInfo:
        result.setInfo("Show Mode Info", "Shows information regarding the selected Mode.", "Mode", 0);
		break;
	case IDs::CommandIDs::setMappingStyle:
		result.setInfo("Mapping Style", "Choose a mapping style for remapping MIDI notes.", "Midi", 0);
		break;
    case IDs::CommandIDs::showMapOrderEdit:
        result.setInfo("Edit Mappings by Order", "Choose how to map modes with the order mapping method.", "Preset", 0);
        break;
	case IDs::CommandIDs::applyMapping:
		result.setInfo("Apply Mapping", "Map incoming MIDI notes to Mode 2 with the selected mapping style.", "Midi", 0);
		break;
    case IDs::CommandIDs::setMappingMode:
        result.setInfo("Auto Map to Scale", "Remap Midi notes when scale changes", "Midi", 0);
        break;
	case IDs::CommandIDs::beginColorEditing:
		result.setInfo("Change Keyboard Colors", "Allows you to change the default colors for the rows of keys.", "Keyboard", 0);
		break;
	case IDs::CommandIDs::setPeriodShift:
		result.setInfo("Shift by Mode 2 Period", "Shift the outgoing MIDI notes by the selected number of Mode 2 periods.", "Midi", 0);
		break;
	case IDs::CommandIDs::setMidiChannelOut:
		result.setInfo("Set MIDI Channel Out", "Set the outgoing MIDI Notes to the selected MIDI Channel.", "Midi", 0);
		break;
	//case IDs::CommandIDs::showMidiNoteNumbers:
	//	result.setInfo("Show Midi Note Numbers", "Shows incoming MIDI notes on Mode 1 and outgoing MIDI Notes on Mode 2.", "Keyboard", 0);
	//	break;
	//case IDs::CommandIDs::setKeyStyle:
	//	result.setInfo("Set Key Style", "Sets the selected style for drawing overlapping degrees between mode degrees.", "Keyboard", 0);
	//	break;
	//case IDs::CommandIDs::setHighlightStyle:
	//	result.setInfo("Set Highlight Style", "Sets the selected style for drawing triggered notes.", "Keyboard", 0);
	//	break;
	default:
		break;
	}
}

bool SvkPluginEditor::perform(const InvocationInfo &info)
{
    switch (info.commandID)
    {
        case IDs::CommandIDs::savePresetToFile:
        {
            savePresetToFile();
            break;
        }
		case IDs::CommandIDs::saveMode:
		{
			saveMode();
			break;
		}
        case IDs::CommandIDs::loadPreset:
        {
            loadPreset();
            break;
        }
		case IDs::CommandIDs::loadMode:
		{
			loadMode();
			break;
		}
        case IDs::CommandIDs::exportReaperMap:
        {
            exportReaperMap();
            break;
        }
		case IDs::CommandIDs::exportAbletonMap:
		{
			exportAbletonMap();
			break;
		}
        case IDs::CommandIDs::showSettingsDialog:
        {
            showSettingsDialog();
            break;
        }
		case IDs::CommandIDs::commitCustomScale:
		{
			commitCustomScale();
			break;
		}
		case IDs::CommandIDs::setMode1:
		{
			setMode1();
			break;
		}
		case IDs::CommandIDs::setMode2:
		{
			setMode2();
			break;
		}
		case IDs::CommandIDs::setMode1RootNote:
		{
			setMode1Root();
			break;
		}
		case IDs::CommandIDs::setMode2RootNote:
		{
			setMode2Root();
			break;
		}
		case IDs::CommandIDs::setModeViewed:
		{
			setModeView();
			break;
		}
		case IDs::CommandIDs::showModeInfo:
		{
			showModeInfo();
			break;
		}
		case IDs::CommandIDs::setMappingStyle:
		{
			setMappingStyle();
			break;
		}
        case IDs::CommandIDs::showMapOrderEdit:
        {
            showMapOrderEditDialog();
            break;
        }
		case IDs::CommandIDs::applyMapping:
		{
			applyMap();
			break;
		}
		case IDs::CommandIDs::setMappingMode:
		{
			setMappingMode();
			break;
		}
		case IDs::CommandIDs::beginColorEditing:
		{
			beginColorEditing();
			break;
		}
		case IDs::CommandIDs::setPeriodShift:
		{
			// TODO
			break;
		}
		case IDs::CommandIDs::setMidiChannelOut:
		{
			// TODO
			break;
		}
        default:
        {
            return false;
        }
    }

    return true;
}
