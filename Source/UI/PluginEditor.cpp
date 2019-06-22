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
SvkPluginEditor::SvkPluginEditor(SvkAudioProcessor& p, ApplicationCommandManager* cmdMgr)
	: AudioProcessorEditor(&p), processor(p), appCmdMgr(cmdMgr), pluginState(processor.getPluginState())
{
	setName("Super Virtual Keyboard");
	setResizable(true, true);
	setBroughtToFrontOnMouseClick(true);
	
	keyboardEditorBar = std::make_unique<KeyboardEditorBar>(pluginState, appCmdMgr);
	keyboardEditorBar.get()->setName("Keyboard Editor Bar");
	keyboardEditorBar.get()->setSize(640, 48);
	addAndMakeVisible(keyboardEditorBar.get());
    
	piano = pluginState->getKeyboard();

	view = std::make_unique<Viewport>("Piano Viewport");
	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano, false);
	view.get()->setTopLeftPosition(1, 49);
    
    /*
    colorChooserWindow.reset(new ColorChooserWindow("Color Chooser", Colours::slateblue, DocumentWindow::closeButton));
    colorChooserWindow->setSize(450, 450);
	colorChooserWindow->addChangeListener(this);
	colorChooserWindow->addToDesktop();
     */
    
    midiSettingsWindow = std::make_unique<MidiSettingsWindow>();
    midiSettingsWindow->setSize(560, 150);
    midiSettingsComponent = std::make_unique<MidiSettingsComponent>(pluginState);
    midiSettingsComponent->setSize(560, 150);
    midiSettingsWindow->setContentOwned(midiSettingsComponent.get(), true);
    midiSettingsWindow->setAlwaysOnTop(true);
    
	pluginState->getMidiProcessor()->resetWithRate(processor.getSampleRate());

    pluginState->addChangeListener(this);
    keyboardEditorBar->addChangeListener(this);
    pluginState->getMidiProcessor()->getKeyboardState()->addListener(piano); // displays MIDI on Keyboard
	initNodeData();
    
    midiSettingsComponent->setMode1SelectedId(9);
    midiSettingsComponent->setMode2SelectedId(9);

    midiSettingsComponent->setMode1RootNote(60);
    midiSettingsComponent->setMode2RootNote(60);
    
	appCmdMgr->registerAllCommandsForTarget(this);
	appCmdMgr->registerAllCommandsForTarget(piano);

	setMouseClickGrabsKeyboardFocus(true);
	addMouseListener(this, true);
    
    setSize(1000, 250);
	setResizeLimits(986, 100, 10e4, 10e4);
    
	startTimerHz(20);
}

SvkPluginEditor::~SvkPluginEditor()
{
    pluginState->removeChangeListener(this);
    keyboardEditorBar->removeChangeListener(this);
    pluginState->getMidiProcessor()->getKeyboardState()->removeListener(piano);
}

//==============================================================================

void SvkPluginEditor::initNodeData()
{
	if (pluginState->pluginEditorNode.isValid())
	{
		pluginEditorNode = pluginState->pluginEditorNode;

		setSize(pluginEditorNode[IDs::windowBoundsW], pluginEditorNode[IDs::windowBoundsH]);
        view.get()->setViewPosition((int)pluginEditorNode[IDs::viewportPosition], 0);
	}
	else
	{
		pluginEditorNode = ValueTree(IDs::pluginEditorNode);
		pluginState->pluginEditorNode = pluginEditorNode;
		pluginState->pluginStateNode.addChild(pluginEditorNode, -1, nullptr);

		pluginEditorNode.addChild(pluginState->pianoNode, 0, nullptr);

		view.get()->setViewPositionProportionately(0.52, 0);
        pluginState->getMidiProcessor()->setAutoRemapOn();
	}
    
    update_children_to_preset();
}

void SvkPluginEditor::updateNodeData()
{
	pluginEditorNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
	pluginEditorNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
	pluginEditorNode.setProperty(IDs::viewportPosition, view.get()->getViewPositionX(), nullptr);
}

void SvkPluginEditor::update_children_to_preset()
{
	Mode* modeLoaded = pluginState->getModeLoaded();
	
	keyboardEditorBar->setModeReadoutText(modeLoaded->getStepsString());
	keyboardEditorBar->setModeLibraryText(modeLoaded->getName());
    keyboardEditorBar->setOffsetReadout(modeLoaded->getRootNote());

	midiSettingsComponent->setMode2(pluginState->getModeLoaded());
    
    keyboardEditorBar->repaint();
	DBG("Children Updated");
}

void SvkPluginEditor::beginColorEditing()
{
	colorChooserWindow->setVisible(true);
	piano->setUIMode(UIMode::colorMode);
	keyboardEditorBar->allowUserInput(false);
}

//==============================================================================


bool SvkPluginEditor::save_preset()
{
    bool written = pluginState->savePreset();
	if (written)
		DBG("file was saved");
	else
		DBG("not saved");

	return written;
}

bool SvkPluginEditor::load_preset()
{
	bool loaded = pluginState->loadPreset();

	if (loaded)
	{
		//piano->restoreDataNode(pluginState->getPresetLoaded()->theKeyboardNode);
		update_children_to_preset();
	}

	return loaded;
}

bool SvkPluginEditor::write_reaper_file()
{
	ReaperWriter rpp = ReaperWriter(pluginState->getModeLoaded());
	return rpp.write_file();
}

//==============================================================================

void SvkPluginEditor::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);
}

void SvkPluginEditor::resized()
{
	int viewPositionKeyboardX = view->getViewPositionX();
	AudioProcessorEditor::resized();

	keyboardEditorBar->setBounds(0, 0, getWidth(), 36);
	view->setBounds(0, keyboardEditorBar->getBottom(), getWidth(), getHeight() - keyboardEditorBar->getHeight());
	piano->setBounds(0, 0, piano->getWidthFromHeight(view->getMaximumVisibleHeight()), view->getMaximumVisibleHeight()-1);
	
	view->setViewPosition(viewPositionKeyboardX, 0);
}

//==============================================================================

void SvkPluginEditor::timerCallback()
{
	piano->repaint();
}

//==============================================================================

void SvkPluginEditor::userTriedToCloseWindow()
{
	if (colorChooserWindow->isVisible())
		colorChooserWindow->closeButtonPressed();

	setVisible(false);
}

//==============================================================================

void SvkPluginEditor::mouseDown(const MouseEvent& e)
{
    Key* key = piano->getKeyFromPosition(e);

	if (piano->getUIMode() == UIMode::playMode)
	{
		if (key)
		{
			if (e.mods.isShiftDown() && !e.mods.isAltDown() && key->activeState == 2)
			{
				// note off
				//piano->lastKeyClicked = 0;
				piano->triggerKeyNoteOff(key);
			}
			else
			{
				if (e.mods.isAltDown())
				{
					Key* oldKey = piano->getKey(piano->getLastKeyClicked());
					piano->triggerKeyNoteOff(oldKey);
				}

				piano->triggerKeyNoteOn(key, piano->getKeyVelocity(key, e));
				piano->setLastKeyClicked(key->keyNumber);
			}
		}
	}
	else if (piano->getUIMode() == UIMode::colorMode)
	{
		if (key)
		{
			if (e.mods.isRightButtonDown())
			{
				if (e.mods.isShiftDown())
				{
					piano->resetKeyOrderColors(key->order, true);
				}
				else if (e.mods.isCtrlDown())
				{
					piano->resetKeySingleColor(key->keyNumber);
				}
				else
				{
					piano->resetKeyDegreeColors(key->keyNumber);
				}
			}

			else if (e.mods.isShiftDown())
			{
				if (piano->getKeyDegreeColor(key->scaleDegree).isOpaque())
					piano->resetKeyDegreeColors(key->scaleDegree);

				else if (piano->getKeySingleColor(key->keyNumber).isOpaque())
					piano->resetKeySingleColor(key->keyNumber);

				piano->setKeyColorOrder(key->order, 3, colorChooserWindow->getColorSelected());
			}
			else if (e.mods.isCtrlDown())
				piano->setKeyColor(key->keyNumber, 3, colorChooserWindow->getColorSelected());
			else
				piano->setKeyColorDegree(key->scaleDegree, 3, colorChooserWindow->getColorSelected());
            
            piano->repaint();
		}
	}
    else if (piano->getUIMode() == UIMode::mapMode)
    {
        if (key)
        {
            piano->selectKeyToMap(key);
        }
    }
}

void SvkPluginEditor::mouseDrag(const MouseEvent& e)
{
	if (piano->getUIMode() == UIMode::playMode)
	{
		Key* key = piano->getKeyFromPosition(e);

		if (key)
		{
			if (key->keyNumber != piano->getLastKeyClicked())
			{
				Key* oldKey = piano->getKey(piano->getLastKeyClicked());
				if (!e.mods.isShiftDown() || e.mods.isAltDown())
				{
					piano->triggerKeyNoteOff(oldKey);
				}

				piano->triggerKeyNoteOn(key, piano->getKeyVelocity(key, e));
				piano->setLastKeyClicked(key->keyNumber);
				repaint();
			}
		}
	}
}

void SvkPluginEditor::mouseUp(const MouseEvent& e)
{
	if (piano->getUIMode() == UIMode::playMode)
	{
		Key* key = piano->getKeyFromPosition(e);

		if (key)
		{
			if (!e.mods.isShiftDown())
			{
				piano->triggerKeyNoteOff(key);
				key->activeState = 1;
				repaint();
			}
		}
	}
}

void SvkPluginEditor::mouseMove(const MouseEvent& e)
{
	if (piano->getUIMode() != UIMode::colorMode)
	{
		Key* key = piano->getKeyFromPosition(e);

		if (key)
		{
			if (key->activeState == 0)
			{
				key->activeState = 1;
				repaint();
			}
		}
	}
}

//==============================================================================

void SvkPluginEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    // New Mode loaded
    if (source == pluginState)
    {
        piano->resetKeyColors(true);
        piano->updatePianoNode();
        update_children_to_preset();
    }
    
    // Color editing has finished
	if (source == colorChooserWindow.get())
	{
		if (piano->getUIMode() == UIMode::colorMode)
		{
			piano->updatePianoNode();

			piano->applyMode(pluginState->getModeLoaded());
			piano->setUIMode(UIMode::playMode);
			keyboardEditorBar->allowUserInput();
		}
	}
    
    // Prepare to play
    if (source == &processor)
    {
        //pluginState->midiStateIn->addListener(piano);
    }
    
    // Root note or Mapping button toggled 
    if (source == keyboardEditorBar.get())
    {
        if (keyboardEditorBar->isMapButtonOn())
        {
            pluginState->getMidiProcessor()->pauseMidiInput();
            piano->setUIMode(UIMode::mapMode);
        }
        else
        {
            pluginState->getMidiProcessor()->pauseMidiInput(false);
            piano->setUIMode(UIMode::playMode);
        }

        midiSettingsComponent->setMode2RootNote(keyboardEditorBar->getOffsetReadout());
		update_children_to_preset();
    }

	// Mode Info Changed
	if (source == modeInfo)
	{
        // should make this better
        pluginState->modePresetNode = pluginState->getModeLoaded()->modeNode;
		pluginState->updatePluginFromParentNode();
	}
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

//==============================================================================

ApplicationCommandTarget* SvkPluginEditor::getNextCommandTarget()
{
	return piano;// findFirstTargetParentComponent();
}

void SvkPluginEditor::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		IDs::CommandIDs::saveCustomLayout,
		IDs::CommandIDs::loadCustomLayout,
		IDs::CommandIDs::saveReaperMap,
		IDs::CommandIDs::setKeyColor,
        IDs::CommandIDs::showModeInfo,
		IDs::CommandIDs::remapMidiNotes
	};

	c.addArray(commands);
}

void SvkPluginEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
	case IDs::CommandIDs::saveCustomLayout:
		result.setInfo("Save Layout", "Save your custom layout to a file.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::horizontal);
		//result.addDefaultKeypress('c', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::loadCustomLayout:
		result.setInfo("Load Layout", "Load a custom layout from a file.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalLeft);
		//result.addDefaultKeypress('a', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::saveReaperMap:
		result.setInfo("Save Reaper Note Names", "Save the current layout as a Reaper MIDI Note Name text file.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalRight);
		//result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setKeyColor:
		result.setInfo("Change Keyboard Colors", "Allows you to change the default colors for the rows of keys.", "Piano", 0);
        result.setActive(false);
		//result.addDefaultKeypress('c', ModifierKeys::shiftModifier);
		break;
    case IDs::CommandIDs::showModeInfo:
        result.setInfo("Show Mode Info", "Shows information regarding the selected Mode.", "Modes", 0);
        result.setActive(true);
	case IDs::CommandIDs::remapMidiNotes:
		result.setInfo("Set midi note mapping", "Allows you to remap your keyboard to trigger modal notes.", "Midi", 0);
		break;
    case IDs::CommandIDs::autoRemap:
        result.setInfo("Auto Map to Scale", "Remap Midi notes when scale changes", "Midi", 0);
        result.setTicked(pluginState->getMidiProcessor()->isAutoRemapping());
        break;
	default:
		break;
	}
}

bool SvkPluginEditor::perform(const InvocationInfo &info)
{
    switch (info.commandID)
    {
        case IDs::CommandIDs::saveCustomLayout:
        {
            save_preset();
            break;
        }
        case IDs::CommandIDs::loadCustomLayout:
        {
            load_preset();
            break;
        }
        case IDs::CommandIDs::saveReaperMap:
        {
            write_reaper_file();
            break;
        }
        case IDs::CommandIDs::setKeyColor:
        {
            beginColorEditing();
            break;
        }
        case IDs::CommandIDs::showModeInfo:
        {
            modeInfo = new ModeInfoDialog(pluginState->getModeLoaded());
			modeInfo->addChangeListener(this);
            CallOutBox::launchAsynchronously(modeInfo, getScreenBounds(), nullptr);
            break;
        }
        case IDs::CommandIDs::remapMidiNotes:
        {
            midiSettingsWindow->setVisible(true);
            break;
        }
        case IDs::CommandIDs::autoRemap:
        {
            pluginState->getMidiProcessor()->setAutoRemapOn(!pluginState->getMidiProcessor()->isAutoRemapping());
            break;
        }
        default:
        {
            return false;
        }
            
    }
    return true;
}

