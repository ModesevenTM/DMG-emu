# DMG-emu

**DMG-emu** is an attempt at making a Game Boy emulator.
This is mainly a research project made just for fun in my free time and isn't supposed to be anywhere near as accurate as other available emulators.

If you are here for a faithful and error-free Game Boy reimplementation, this is not one of them ;)

## Features

- Implementation of GB's CPU and PPU behaviour (accurate enough to get the most important games working without major issues).
- Full MBC1/2 and partial MBC3/30 mapper support.
- Keyboard input support and graphics rendering, thanks to [SDL3](https://www.libsdl.org/).
- Experimental game save data support (for games with built-in save memory).

## Compilation

- Download source code for SDL3 (version [3.4.20](https://github.com/libsdl-org/SDL/releases/tag/release-3.4.20) is recommended, [latest build](https://github.com/libsdl-org/SDL/releases/latest) should work fine as well).
- Extract archive to the same directory as the project and rename the extracted folder to <i>SDL3</i>.
- Open the solution file in Visual Studio 2026 and compile the program.

## Usage

```
DMG-emu.exe "path\to\game\rom"
```

## Future features

- Sound implementation.
- Support for more mappers as well as Game Boy Color enhancements and/or exclusive games.
- Fixed graphical glitches in select titles.
- Improved accuracy for both CPU and graphics rendering.
- Better game saving system.
- Support for OSes other than Windows.
- Code cleanup.