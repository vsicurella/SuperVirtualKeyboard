# SuperVirtualKeyboard

LATEST VERSION: Alpha 2019-04-28 (Contact me for build)

SUMMARY:

Super Virtual Keyboard is a MIDI-generating plugin that allows you to explore different keyboard layouts with the intention of making microtonal scales, particularly
larger ones, easier to digest and compose with. This is actually a subproject of a more generalized, larger project to offer many different ways to analyze scales.

FEATURES:

 - -> This plugin does not yet offer retuning capabilities, so you will have to alter your synth's tuning. <-

 - Select a scale from a large selection of mode presets then click the keys on the keyboard to play.

 - Hold "Shift" to hold down notes, "Alt" for "Mono Legato" mode, and "Space" to de/retrigger held notes.

 - Use "Up" & "Down" keys on held notes to transpose by one interval, or "Left" & "Right" keys to transpose by one modal step. 
   At this time, modal transposition is only possible if all held notes belong to the same mode (all white, all black, etc.)

 - Save your layout as a text file that Reaper can load as "MIDI Note Names" so that you can see the layout in Reaper's piano roll!
 
 - Change the colors of the keys! Open up the color chooser from the Edit menu, select a color then click and use a modifier to paint      the keys. 
    - No modifiers will paint all keys of the same scale degree
    - Shift will paint all belonging to the same mode
    - Control will only paint the clicked key.
    - Right-clicking, with respective modifiers, resets the key to either the default mode or degree color.
    
  - Save and load your custom scales and key colors!

TO DO:

 - Customize key sizes and positions
 - Change keyboard orientation
 - Add note names
 - Set transposition parameters
 - Graphics shading & other niceties
 - more...

BUGS:

 - Midi device doesn't get reconfigured when device settings change
 - Window resizing is bad funky
 - Clicking on a held note should turn it off, but it doesn't work on Mac
 - Definitely some more...
