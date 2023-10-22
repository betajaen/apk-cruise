# Cruise for a Corpse for Commodore Amiga RTG, AGA and modern platforms.

This is a port of Cruise for a Corpse for the Commodore Amiga for AGA and RTG support, it also supports other modern platforms that implement SDL2.

This port is based upon the ScummVM 2.8 (or later) implementation of the 'CruisE' engine only but does not use other parts of the ScummVM codebase.

## Required data files

These can be either the DOS or Amiga versions. This project does not contain any of the original game data files.

The following files must be placed in the data/dos or data/amiga drawers, which can be found from your Cruise for a Corpse disks.

* D1
* D2
* D3
* D4
* D5
* DELPHINE.LNG (Optional)
* SYSTEM.FNT
* VOL.1
* VOL.2
* VOL.3
* VOL.4
* VOL.5

## Port Status

### Amiga

* [ ] Input
* [ ] Sound
* [ ] Music
* [ ] Graphics via graphics.library (ECS/AGA)
* [ ] Graphics via RTG
* [ ] Saved Games
* [ ] Amiga 32 colour game support
* [ ] DOS 256 colour game support
* [ ] Mixed Amiga/DOS game support
* [ ] Port: Amiga OS 3.x
* [ ] Controls enhancements
* [ ] Installer

#### Observations

None.

### SDL 2.0

* [x] Makefiles
* [x] Input
* [ ] Sound
* [ ] Music
* [x] Graphics via SDL
* [ ] Saved Games
* [x] Amiga 32 colour game support
* [x] DOS 256 colour game support
* [ ] Mixed Amiga/DOS support
* [x] Port: MacOS
* [ ] Windows
* [ ] Linux
* [ ] Controls enhancements
* [ ] Installer

#### Observations

Game can be played normally. Some string errors have been observed. Some walkboxes errors have been noticed. Palette errors occur in opening cutscene.

## ScummVM differences

This codebase uses the 'CruisE' engine source code only and therefore uses the same license as ScummVM GPL 3.0

Additional code such classes and Functions that are normally used in the ScummVM source code have been replicated by the Amiga Port Kit (APK) code and the contents of the Engine folder. Any modifications to the 'CruisE' engine are documentated with the 'MOD:' sub-string in or near code as comments. These have been written by new authors that are not associated (see AUTHORS) with the ScummVM Project.
