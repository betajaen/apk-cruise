# Cruise for a Corpse for Commodore Amiga AGA/RTG and modern platforms.

This is a port of [Cruise for a Corpse by Delphine Software](https://en.wikipedia.org/wiki/Cruise_for_a_Corpse) for the [Commodore Amiga](https://en.wikipedia.org/wiki/Amiga)  it also supports other modern platforms that implement SDL2.

This port is based upon the ScummVM 2.8 (or later) implementation of the ['CruisE'](https://github.com/scummvm/scummvm/tree/master/engines/cruise) engine only but does not use other parts of the ScummVM codebase.

## Required data files

This requires the original game data files to play which can be either the DOS or Amiga versions. This project does not contain any of the original game data files.

The following files must be placed in the same drawer as the executable, which can be found from your Cruise for a Corpse disks.

* `D1`
* `D2`
* `D3`
* `D4`
* `D5`
* `DELPHINE.LNG` (Optional)
* `SYSTEM.FNT`
* `VOL.1`
* `VOL.2`
* `VOL.3`
* `VOL.4`
* `VOL.5`

## Port Status

### Input
  * [X] Mouse
  * [X] Hot Keys

### Audio
  * [ ] Sound Effects
  * [ ] Music

### Graphics
  * [X] Graphics via graphics.library
  * [X] Graphics via Real-time Graphics
  * [X] Graphics via SDL2
  * [ ] Optimized Sprites
  * [ ] Optimized Backgrounds
  * [ ] Optimized Polygons
  * [ ] Optimized Palette
  * [ ] Planar Support
  * [X] Mouse Cursor images
  
### Save Games
  * [X] Quick Load/Save
  * [X] Load/Save via Requester

### Game Editions
  * [X] DOS 256
  * [ ] Amiga
  * [ ] DOS with Amiga music

### Ports
  * [X] Amiga OS 3.x on 68k
  * [X] SDL2

### Enhancements
  * [X] Controls
  * [ ] Reduced Memory usage

### Installer
  * [ ] Amiga Installer
  * [ ] MacOS Disk Image
  * [ ] Windows Setup

## State of the Game and Observations

### Amiga

* Was able to play the entire game from start to finish with no errors.
* Some palette errors during cut scene, or switching screens.
* Some walkboxes errors have been noticed.
* Performance problems with heavy sprites and/or polygons.
* Crashing when compiled with optimisations.
* No Music/Audio

### SDL2

* Was able to play most of the game using pre-saved games as full playthrough has not been tested yet.
* Some palette errors during cut scene, or switching screens.
* Some walkboxes errors have been noticed.
* Performance problems with heavy sprites and/or polygons.
* No Music/Audio

## Game Differences

The Game uses enhanced controls, these replace the ones described in the manual.

### Enhanced Controls

| Key              | Option                 |
|------------------|------------------------|
| `Esc`            | Quick Game Immediately |
| `Space`          | Pause Game             |
| `Shift` and `F9` | Restart Game   |
| `F5`             | Quick Save |
| `Shift` and `F5` | Save to File |
| `F9`             | Quick Load |
| `Shift` and `F9` | Load from File |


## ScummVM differences

This codebase uses the 'CruisE' engine source code only and therefore uses the same license as ScummVM GPL 3.0

Additional code such classes and Functions that are normally used in the ScummVM source code have been replicated by the Amiga Port Kit (APK) code and the contents of the Engine folder. Any modifications to the 'CruisE' engine are documentated with the 'MOD:' sub-string in or near code as comments. These have been written by new authors that are not associated (see AUTHORS) with the ScummVM Project.
