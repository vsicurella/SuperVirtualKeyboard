# SuperVirtualKeyboard

LATEST VERSION: Alpha 2019-08-22 (v0.021)

SUMMARY:

Super Virtual Keyboard is a MIDI-generating plugin that allows you to explore different keyboard layouts with the intention of making microtonal scales, particularly
larger ones, easier to digest and compose with.

FEATURES:

 - -> This plugin does not yet offer retuning capabilities, so you will have to alter your synth's tuning. <-

 - Select a scale from a large selection of mode presets then click the keys on the keyboard to play.

 - Use the mapping function to remap your midi piano keyboard to a microtonal scale.
 	
	- The first scale selection box represents what scale you're mapping from, which should be your hardware MIDI keyboard's layout. More often than not this will be Meantone[7] 12. The second scale is the scale you will be remapping to.

	- Mapping "Mode to Mode" will map your MIDI keyboard's white keys to the white keys of the selected scale and will approxmate closest degrees on the chromatic keys.

	- Mapping "Mode to Scale" map your full chromatic MIDI keyboard to the selected scale's white keys. This if useful if your scale's mode size is equal to your midi keyboard's scale size
		- for example, Meantone[12] 31 (in 31edo) works great on a standard midi keyboard
		- Superpyth[12] 22 is quite interesting ;)

	- Mapping "Mode by Order" will allow you to map a particular order of keys (order 0 = diatonic white keys, order 1 = pentatonic black keys) to a particular order of keys in the selected scale.

 - If you don't have a midi controller, here are some features you can take advantage of:

 	- Hold "Shift" to hold down notes, "Alt" for "Mono Legato" mode, and "Space" to retrigger held notes.

 	- Use "Up" & "Down" keys on held notes to transpose by one interval, or "Left" & "Right" keys to transpose by one modal step. 

 - Save your layout as a text file that Reaper can load as "MIDI Note Names" so that you can see the layout in Reaper's piano roll!
    
  - View and Edit information about your mode in the "View Mode Info" dialog in the "Edit" menu

  - Save and load your custom scales and midi mappings!
  
   ** Disabled in v0.021 **
 - Change the colors of the keys! Open up the color chooser from the Edit menu, select a color then click and use a modifier to paint the keys. 
    - No modifiers will paint all keys of the same scale degree
    - Shift will paint all belonging to the same mode
    - Control will only paint the clicked key.
    - Right-clicking, with respective modifiers, resets the key to either the default mode or degree color.
  
New in version 0.021
- Updated GUI
- Custom mode folder scanning
- Saving/Importing both modes and presets
- MOS scales up through 23EDO, also 31EDO
- Bug: Ableton export is not yet implemented

TO DO:
 - Add manual mapping
 - Add note names
 - Add MPE Tuning capability
 - Add preset tags and improve preset browsing and management
 - Customize key sizes and positions
 - Change keyboard orientation
 - Set transposition parameters
 - Graphics shading & other niceties
 - Have a complete selection of EDO MOS scales and include more detailed Mode info
 - Allow piano keys to be triggered by computer keyboard keys
 - More...

BUGS:
 - Midi device doesn't get reconfigured when device settings change
 - Window resizing is bad funky
 - Mapping Mode by Order and setting offsets don't work as intended
 - More...

