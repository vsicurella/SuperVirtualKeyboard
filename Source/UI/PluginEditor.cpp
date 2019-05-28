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
SuperVirtualKeyboardAudioProcessorEditor::SuperVirtualKeyboardAudioProcessorEditor(SvkAudioProcessor& p, ApplicationCommandManager* cmdMgr)
	: AudioProcessorEditor(&p), processor(p), appCmdMgr(cmdMgr), pluginState(processor.getPluginState())
{
	setName("Super Virtual Keyboard");
	setResizable(true, true);
	setBroughtToFrontOnMouseClick(true);
	
	keyboardEditorBar.reset(new KeyboardEditorBar(pluginState, appCmdMgr));
	keyboardEditorBar.get()->setName("Keyboard Editor Bar");
	keyboardEditorBar.get()->setSize(640, 48);
	addAndMakeVisible(keyboardEditorBar.get());
    
	piano.reset(new Keyboard(pluginState));
	piano->setName("The Piano");


	view.reset(new Viewport("Piano Viewport"));
	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano.get());
	view.get()->setTopLeftPosition(1, 49);
    
    colorChooserWindow.reset(new ColorChooserWindow("Color Chooser", Colours::slateblue, DocumentWindow::closeButton));
    colorChooserWindow->setSize(450, 450);
	colorChooserWindow->addChangeListener(this);
	colorChooserWindow->addToDesktop();

    midiSettingsWindow.reset(new MidiSettingsWindow());
    midiSettingsWindow->setSize(800, 600);
	midiSettingsComponent.reset(new MidiSettingsComponent(pluginState));
    midiSettingsComponent->setSize(800, 600);
    midiSettingsWindow->setContentOwned(midiSettingsComponent.get(), true);

    pluginState->addChangeListener(this);
    keyboardEditorBar->addChangeListener(this);
    piano->addListener(pluginState->getMidiProcessor()); // generates MIDI from UI
    pluginState->getMidiProcessor()->getKeyboardState()->addListener(piano.get()); // displays MIDI on Keyboard
	initNodeData();

	appCmdMgr->registerAllCommandsForTarget(this);
	appCmdMgr->registerAllCommandsForTarget(piano.get());

	setMouseClickGrabsKeyboardFocus(true);
	addMouseListener(this, true);
    

    setSize(1000, 250);
	setResizeLimits(640, 100, 10e4, 10e4);

	startTimerHz(20);
}

SuperVirtualKeyboardAudioProcessorEditor::~SuperVirtualKeyboardAudioProcessorEditor()
{
    pluginState->removeChangeListener(this);
    keyboardEditorBar->removeChangeListener(this);
    piano->removeListener(pluginState->getMidiProcessor());
    pluginState->getMidiProcessor()->getKeyboardState()->removeListener(piano.get());
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::initNodeData()
{
	if (pluginState->keyboardWindowNode.isValid())
	{
		keyboardWindowNode = pluginState->keyboardWindowNode;
		update_children_to_preset();
		setSize(keyboardWindowNode[IDs::windowBoundsW], keyboardWindowNode[IDs::windowBoundsH]);
		view.get()->setViewPosition((int)keyboardWindowNode[IDs::viewportPosition], 0);
	}
	else
	{
		keyboardWindowNode = ValueTree(IDs::keyboardWindowNode);
		
		pluginState->loadMode(8);
		updateNodeData();

		keyboardWindowNode.addChild(pluginState->pianoNode, 0, nullptr);
		pluginState->keyboardWindowNode = keyboardWindowNode;
		pluginState->pluginStateNode.addChild(keyboardWindowNode, 2, nullptr);

		view.get()->setViewPositionProportionately(0.52, 0);
	}
}

void SuperVirtualKeyboardAudioProcessorEditor::updateNodeData()
{
	keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
	keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
	keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewPositionX(), nullptr);
}



void SuperVirtualKeyboardAudioProcessorEditor::update_children_to_preset()
{
	Mode* modeLoaded = pluginState->getModeLoaded();
		
	piano->applyMode(modeLoaded);

	keyboardEditorBar->setModeReadoutText(modeLoaded->getStepsString());
	keyboardEditorBar->setModeLibraryText(modeLoaded->getDescription());
    keyboardEditorBar->setOffsetReadout(modeLoaded->getRootNote());
	DBG("Children Updated");
}

void SuperVirtualKeyboardAudioProcessorEditor::beginColorEditing()
{
	colorChooserWindow->setVisible(true);
	piano->setUIMode(UIMode::colorMode);
	keyboardEditorBar->allowUserInput(false);
}

//==============================================================================


bool SuperVirtualKeyboardAudioProcessorEditor::save_preset()
{
	piano->updatePianoNode();
	pluginState->updateKeyboardSettingsPreset();
	bool written = pluginState->savePreset();
	if (written)
		DBG("file was saved");
	else
		DBG("not saved");

	return written;
}

bool SuperVirtualKeyboardAudioProcessorEditor::load_preset()
{
	bool loaded = pluginState->loadPreset();

	if (loaded)
	{
		piano->restoreDataNode(pluginState->getPresetLoaded()->theKeyboardNode);
		update_children_to_preset();
	}

	return loaded;
}

bool SuperVirtualKeyboardAudioProcessorEditor::write_reaper_file()
{
	ReaperWriter rpp = ReaperWriter(pluginState->getModeLoaded());
	return rpp.write_file();
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);
}

void SuperVirtualKeyboardAudioProcessorEditor::resized()
{
	int viewPositionKeyboardX = view->getViewPositionX();
	AudioProcessorEditor::resized();

	keyboardEditorBar->setBounds(0, 6, getWidth(), 36);
	view->setBounds(0, keyboardEditorBar->getBottom(), getWidth(), getHeight() - keyboardEditorBar->getHeight() - view.get()->getScrollBarThickness());
	piano->setBounds(0, 0, piano->getWidthFromHeight(view->getMaximumVisibleHeight()), view->getMaximumVisibleHeight()-1);
	
	view->setViewPosition(viewPositionKeyboardX, 0);

	// update node
	if (keyboardWindowNode.isValid())
	{
		updateNodeData();
	}
	
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::timerCallback()
{
	//piano.get()->getMidiKeyboardState()->processNextMidiBuffer(
	//	*processor.get_midi_buffer(), 0, 4096, true);

	piano.get()->repaint();
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::userTriedToCloseWindow()
{
	if (colorChooserWindow->isVisible())
		colorChooserWindow->closeButtonPressed();

	setVisible(false);
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::mouseDown(const MouseEvent& e)
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

void SuperVirtualKeyboardAudioProcessorEditor::mouseDrag(const MouseEvent& e)
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

void SuperVirtualKeyboardAudioProcessorEditor::mouseUp(const MouseEvent& e)
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

void SuperVirtualKeyboardAudioProcessorEditor::mouseMove(const MouseEvent& e)
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

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
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
			pluginState->updateKeyboardSettingsPreset();

			piano->applyMode(pluginState->getModeLoaded());
			piano->setUIMode(UIMode::playMode);
			keyboardEditorBar->allowUserInput();
		}
	}
    
    // Prepare to play
    if (source == &processor)
    {
        //pluginState->midiStateIn->addListener(piano.get());
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

		update_children_to_preset();
    }
}

//==============================================================================


File SuperVirtualKeyboardAudioProcessorEditor::fileDialog(String message, bool forSaving)
{
	FileChooser chooser(message, File::getSpecialLocation(File::userDocumentsDirectory), "*.svk");

	if (forSaving)
		chooser.browseForFileToSave(true);
	else
		chooser.browseForFileToOpen();

	return chooser.getResult();
}

//==============================================================================

ApplicationCommandTarget* SuperVirtualKeyboardAudioProcessorEditor::getNextCommandTarget()
{
	return piano.get();// findFirstTargetParentComponent();
}

void SuperVirtualKeyboardAudioProcessorEditor::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		IDs::CommandIDs::saveCustomLayout,
		IDs::CommandIDs::loadCustomLayout,
		IDs::CommandIDs::saveReaperMap,
		IDs::CommandIDs::setKeyColor,
		IDs::CommandIDs::remapMidiNotes
	};

	c.addArray(commands);
}

void SuperVirtualKeyboardAudioProcessorEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
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
		//result.addDefaultKeypress('c', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::remapMidiNotes:
		result.setInfo("Set midi note mapping", "Allows you to remap your keyboard to trigger modal notes.", "Midi", 0);
		break;
	default:
		break;
	}
}

bool SuperVirtualKeyboardAudioProcessorEditor::perform(const InvocationInfo &info)
{
	switch (info.commandID)
	{
	case IDs::CommandIDs::saveCustomLayout:
		save_preset();
		break;
	case IDs::CommandIDs::loadCustomLayout:
		load_preset();
		break;
	case IDs::CommandIDs::saveReaperMap:
		write_reaper_file();
		break;
	case IDs::CommandIDs::setKeyColor:
		beginColorEditing();
		break;
	case IDs::CommandIDs::remapMidiNotes:
		midiSettingsWindow->setVisible(true);
		break;
	default:
		return false;
	}
	return true;
}

