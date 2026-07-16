# SuperVirtualKeyboard

[![Build](https://github.com/vsicurella/SuperVirtualKeyboard/actions/workflows/build.yml/badge.svg)](https://github.com/vsicurella/SuperVirtualKeyboard/actions/workflows/build.yml)
[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](UNLICENSE.txt)
[![Platforms](https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](#requirements)

LATEST VERSION: Alpha 2026-07-15 (v0.4.0)

![SuperVirtualKeyboard Preview](/Resources/SVK-SuperPyth22-Preview.png)

## What is "SVK"?

Super Virtual Keyboard is a MIDI-controller plugin that allows you to explore different keyboard layouts with the intention of making microtonal scales easier to digest and compose with.

*This plugin does not yet offer retuning capabilities, so you will have to alter your synth's tuning.*

## Highlighted Features

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

### New in version 0.4.0

1. Improved resizing-related controls
1. Moved Save/Load into a main menu
1. Added a "Minimal View" mode for a bare-bones interface
1. Several new options for exporting a Reaper Note Names file

### To do

* Allow piano keys to be triggered by computer keyboard keys
* Rework header area, scale/mode information, preset browser
* Add note naming system
* Advanced keyboard customization
* Graphics shading & other niceties
* Scale creation tools
* More...

## Requirements

* **OS:** Linux, macOS, or Windows
* **Formats:** VST3 plugin and a Standalone application
* **Host:** Any VST3-compatible DAW for the plugin build, standalone runs directly.

> Note: this is a MIDI-controller plugin that produces MIDI, not audio. Route its output to an instrument, and set that instrument's tuning to match your scale. Tuning features planned in a future version.

## Download & Install

Grab the latest build for your platform from the [Releases page](https://github.com/vsicurella/SuperVirtualKeyboard/releases). Each zip contains the VST3 and Standalone versions.

* **VST3** — copy `SuperVirtualKeyboard.vst3` into your system VST3 folder, then rescan plugins in your DAW:
  * Linux: `~/.vst3`
  * macOS: `~/Library/Audio/Plug-Ins/VST3`
  * Windows: `C:\Program Files\Common Files\VST3`
* **Standalone** — run the app directly

## Building from Source

The project uses CMake and depends on [JUCE](https://juce.com) 8.0.14. Point the `JUCE_PATH` environment variable at your local JUCE directory, then build with the included helper script:

```sh
export JUCE_PATH=/path/to/JUCE      # Windows (PowerShell): $env:JUCE_PATH = "C:\path\to\JUCE"
python build.py                    # configure + build Release, then zip into dist/
```

Useful flags: `--config Debug`, `--clean`, `--jobs N` (limit parallelism on low-memory machines). Run `python build.py --help` for the full list.

By default the built VST3 is copied into your OS plugin folder after each build (on Windows, into %LOCALAPPDATA%/Programs/Common/VST3 so no admin rights are needed). Disable with `-DSVK_COPY_PLUGIN_AFTER_BUILD=OFF`.

## Contributing

Submit bug reports and ideas [here](https://github.com/vsicurella/SuperVirtualKeyboard/issues).
Reach out if you want to work on something to merge in.

## Credits

Created by Vincenzo Sicurella. Built with the [JUCE](https://juce.com) framework.

## License

This software is released into the public domain under the [Unlicense](UNLICENSE.txt). You are free to copy, modify, publish, use, compile, sell, or distribute it, for any purpose, with or without attribution.
