# SuperVirtualKeyboard

LATEST VERSION: Alpha 2019-08-03 (v0.02)

SUMMARY:

Super Virtual Keyboard is a MIDI-generating plugin that allows you to explore different keyboard layouts with the intention of making microtonal scales, particularly
larger ones, easier to digest and compose with.

FEATURES:

 - -> This plugin does not yet offer retuning capabilities, so you will have to alter your synth's tuning. <-

 - Select a scale from a large selection of mode presets then click the keys on the keyboard to play.

 - Use the Mapping settings in the "Edit Note Mapping" dialog in the "Edit" menu to remap your midi piano keyboard to a microtonal scale.

	- Mapping "Mode to Mode" will put the scale's mode onto the diatonic keys (the "white keys") of your midi keyboard and approxmate closest degrees on the chromatic keys.

	- Mapping "Mode to Scale" will put the scale's mode onto your full chromatic midi keyboard. This if useful if your scale's mode size is equal to your midi keyboard's scale size
		- for example, Meantone[12] 31 (in 31edo) works great on a standard midi keyboard
		- Superpyth[12] 22 is quite interesting ;)
	- Mapping "Mode by Order" will allow you to map a particular order of keys (order 0 = diatonic white keys, order 1 = pentatonic black keys) to a particular order of keys in your mode's scale

 - If you don't have a midi controller, here are some features you can take advantage of:

 	- Hold "Shift" to hold down notes, "Alt" for "Mono Legato" mode, and "Space" to retrigger held notes.

 	- Use "Up" & "Down" keys on held notes to transpose by one interval, or "Left" & "Right" keys to transpose by one modal step. 

 - Save your layout as a text file that Reaper can load as "MIDI Note Names" so that you can see the layout in Reaper's piano roll!
 
 - Change the colors of the keys! Open up the color chooser from the Edit menu, select a color then click and use a modifier to paint      the keys. 
    - No modifiers will paint all keys of the same scale degree
    - Shift will paint all belonging to the same mode
    - Control will only paint the clicked key.
    - Right-clicking, with respective modifiers, resets the key to either the default mode or degree color.
    

  - View and Edit information about your mode in the "View Mode Info" dialog in the "Edit" menu

  - Save and load your custom scales and midi mappings!
  
New in version 0.02
- Updated GUI
- Custom mode folder scanning
- Saving/Importing both modes and presets
- MOS scales up through 23EDO
- Bug: Colors don't stay on the right keys the first time you set them, but should work after that

TO DO:
 - Customize key sizes and positions
 - Change keyboard orientation
 - Add note names
 - Set transposition parameters
 - Graphics shading & other niceties
 - Have a complete selection of EDO MOS scales and include more detailed Mode info
 - Add preset tags and improve preset browsing and management
 - Allow piano keys to be treggered by computer keyboard keys
 - Add preset slots to allow for automated scale switching
 - Add MPE Tuning capability
 - More...

BUGS:
 - Midi device doesn't get reconfigured when device settings change
 - Window resizing is bad funky
 - Mapping Mode by Order and setting offsets don't work as intended
 - Learn manual mapping needs to be re-implemented
 - More...

