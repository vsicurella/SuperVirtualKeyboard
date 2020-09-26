# SuperVirtualKeyboard

LATEST VERSION: Alpha 2020-10-?? (v0.03)

![SuperVirtualKeyboard Preview](/Resources/SVK-SuperPyth22-Preview.png)

## SUMMARY:

Super Virtual Keyboard is a MIDI-controller plugin that allows you to explore different keyboard layouts with the intention of making microtonal scales easier to digest and compose with.

*This plugin does not yet offer retuning capabilities, so you will have to alter your synth's tuning.*

## FEATURES:
* Select a scale from a large selection of mode presets then click the keys on the keyboard to play.

* Set custom colors for keys by scale degree, key layer (like all white vs all black), or individual keys, which can be saved and reloaded with your scale. Click "Edit Colors", select the color, and then click the key you want to change the color of.
  * No key modifiers will paint all keys of the same scale degree
  * Shift will paint all belonging to the same layer
  * Control will only paint the clicked key.
  * Right-clicking, with respective modifiers, resets the key(s) to a default color.

* If you don't have a midi controller, here are some features you can take advantage of:
  * Hold "Shift" to hold down notes, "Alt" for "Mono Legato" mode, and "Space" to retrigger held notes.
  * Use "Up" & "Down" keys on held notes to transpose by one interval, or "Left" & "Right" keys to transpose by one modal step. 

* Save your layout as a text file that Reaper can load as "MIDI Note Names" so that you can see the layout in Reaper's piano roll!    
  * View and Edit information about your mode in the "View Mode Info" dialog in the "Edit" menu
  * Save and load your custom scales and midi mappings!

* Use the mapping function to remap your midi piano keyboard to a microtonal scale. To enable, open the dropdown box on the top left and choose "Auto Mapping". This will reveal some more controls, as well as a mapping style.
  * The first scale selection box on the right represents what scale you're mapping from, the input scale, which should be your hardware MIDI keyboard's layout. This will probably be the standard layout, Meantone[7] 12. The second scale under it is the output scale, the scale you will be remapping to.
  * The "Mode to Mode" style will map your MIDI keyboard's white keys to the white keys of the selected scale and will approxmate closest degrees on the chromatic keys.
  * The "Mode to Scale" style will map your full chromatic MIDI keyboard to the selected scale's white keys. This if useful if your output scale's mode size is equal to your input scale's size (the MIDI keyboard's scale size, which is usually 12).
    * For example, Meantone[12] 31 (in 31-EDO) works great on a standard midi keyboard, which creates a chromatic layout from 31-EDO.
    * Superpyth[12] 22 and Pajara[12] 22 are other interesting ones to play around with in this mode.
  * The "by Layers" style will allow you to map a certain layer of keys (layer 1 = diatonic white keys, order 2 = pentatonic black keys) to a certain layer of keys in the output scale.
    * An example would be making a the scale pattern "7 7 7 7 7" which is 35-EDO, and then mapping your input white keys the 7-EDO output white keys, and your black keys to one of the 5-EDO subsets.

### New in version 0.03:
1. Improved preset structure and recalling, however *new preset format is incompatible with older presets*
   * Modes may still load
   * I can add support for old presets upon request
   * The new preset structure includes versioning which make backwards-compatibility more feasible
1. Improved GUI
1. New Settings menu
   * Reintroduced key color features
   * Adjacent key placement style
   * Fixed period shifting and transposition
   * MIDI Device selection for standalone app

### To do:
* Add manual mapping
* Allow piano keys to be triggered by computer keyboard keys
* Add note names
* Add tuning capability (??)
* Add preset tags and improve preset browsing and management
* Advanced key shape customization
* Change keyboard orientation
* Graphics shading & other niceties
* Different methods of scale creating
* More...

### Known Bugs:
 - Window resizing is bad funky
 - More...
