# Cruise for a Corpse for Commodore Amiga RTG, AGA and modern platforms.

This is a port of Cruise for a Corpse for the Commodore Amiga for AGA and RTG support, it also supports other modern platforms that implement SDL2.

This port is based upon the ScummVM 2.8 (or later) implementation of the 'CruisE' engine only, and does not use other parts of the ScummVM codebase.

## Required data files

These can be either the DOS or Amiga verions

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

---

ScummVM original copyright header as follows.

# [ScummVM README](https://www.scummvm.org/) 

## About ScummVM

ScummVM allows you to play classic graphic point-and-click adventure games, text adventure games, and RPGs, as long as you already have the game data files. ScummVM replaces the executable files shipped with the games, which means you can now play your favorite games on all your favorite devices.

So how did ScummVM get its name? Many of the famous LucasArts adventure games, such as Maniac Mansion and the Monkey Island series, were created using a utility called SCUMM (Script Creation Utility for Maniac Mansion). The ‘VM’ in ScummVM stands for Virtual Machine.

While ScummVM was originally designed to run LucasArts’ SCUMM games, over time support has been added for many other games: see the full list [on our wiki](https://wiki.scummvm.org/index.php?title=Category:Supported_Games). Noteworthy titles include Broken Sword, Myst and Blade Runner, although there are countless other hidden gems to explore.

For more information, compatibility lists, details on donating, the
latest release, progress reports and more, please visit the ScummVM [home
page](https://www.scummvm.org/).

## Quickstart

For the impatient among you, here is how to get ScummVM running in five simple steps.

1. Download ScummVM from [our website](https://www.scummvm.org/downloads/) and install it.

2. Create a directory on your hard drive and copy the game datafiles from the original media to this directory. Repeat this for every game you want to play.

3. Start ScummVM, choose 'Add game', select the directory containing the game datafiles (do not try to select the datafiles themselves!) and press Choose.

4. The Game Options dialog opens to allow configuration of various settings for the game. These can be reconfigured at any time, but for now everything should be OK at the default settings.

5. Select the game you want to play in the list, and press Start. To play a game next time, skip to step 5, unless you want to add more games.

>
> Hint:
>
> To add multiple games in one go, press and hold the shift key, then click 'Add game' -- the label will change to 'Mass Add' and if you press it, you are again asked to select a directory, only this time ScummVM will search through all subdirectories for supported games.



## Reporting a bug

To report a bug, go to the ScummVM [Issue Tracker](https://bugs.scummvm.org/) and log in with your GitHub account.

Please make sure the bug is reproducible, and still occurs in the latest git/Daily build version. Also check the [compatibility list](https://www.scummvm.org/compatibility/) for that game, to ensure the issue is not already known. Please do not report bugs for games that are not listed as completable on the [Supported Games](https://wiki.scummvm.org/index.php?title=Category:Supported_Games) wiki page, or on the compatibility list. We already know those games have bugs!

Please include the following information in the bug report:

- ScummVM version (test the latest git/Daily build)
- Bug details, including instructions for how to reproduce the bug. If possible, include log files, screenshots, and any other relevant information.
- Game language
- Game version (for example, talkie or floppy)
- Platform and Compiler (for example, Win32, Linux or FreeBSD)
- An attached saved game, if possible.
- If this bug only occurred recently, include the last version without the bug, and the first version with the bug. That way we can fix it quicker by looking at the changes made.

Finally, please report each issue separately; do not file multiple issues on the same ticket. It is difficult to track the status of each individual bug when they aren't on their own tickets.

## Documentation

### User documentation

For everything you need to know about how to use ScummVM, see our [user documentation](https://docs.scummvm.org/).

### The ScummVM Wiki

[The wiki](https://wiki.scummvm.org/) is the place to go for information about every game supported by ScummVM. If you're a developer, there's also some very handy information in the Developer section.

### Changelog

Our extensive change log is available [here](NEWS.md).

## SAST Tools

[PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=github&utm_medium=organic&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

## Credits

A massive thank you to the entire team for making the ScummVM project possible. See the credits [here](AUTHORS)!

-----

> Good Luck and Happy Adventuring\!
> The ScummVM team.
> <https://www.scummvm.org/>
