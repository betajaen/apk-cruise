#include "engine/engine.h"
#include "cruise/cruise.h"
#include <apk/gfx.h>
#include <apk/file.h>
#include <apk/prefs.h>

namespace Cruise {
    OSystem OSystem::s_instance = {};

    void OSystem::updateScreen() {
    }

    void OSystem::copyRectToScreen(uint8* data, uint32 pitch, uint32 x, uint32 y, uint32 w, uint32 h) {
    }

    void OSystem::copyToScreen(uint8* data, uint32 size) {
    }

    void OSystem::box(uint32 left, uint32 top, uint32 right, uint32 bottom) {
    }

}

namespace apk {

    bool s_RulesPassedCopyright;
    bool s_RulesCanSaveLoad;
    bool s_RulesAutoSaveNextBackground;
    bool s_RulesInClock;
    int16 s_LastSoundNum;

    bool hasDataFiles() {

        static const char* gameFiles[] = {
            "d1",
            "d2",
            "d3",
            "d4",
            "d5",
            "system.fnt",
            "vol.1",
            "vol.2",
            "vol.3",
            "vol.4",
            "vol.5",
            "vol.cnf",
            "vol.end",
            NULL
        };

        const char** gameFile = gameFiles;
        while(*gameFile != NULL) {

            if (path::test(*gameFile) != path::PathType::File) {
                return false;
            }

            gameFile++;
        }

        return true;
    }

    void gameMain() {

        s_RulesPassedCopyright = false;
        s_RulesCanSaveLoad = false;
        s_RulesInClock = false;
        s_RulesAutoSaveNextBackground = false;
        s_LastSoundNum = 0;


        const char* dataDirectory = prefs::getPrefsString("DATA", "PROGDIR:");
        fs::setProgramDir(dataDirectory);

        if (hasDataFiles() == false) {
            requester_okay("Cruise for a Corpse", "Game data files could not be found in the game path.\n\nPlease ensure that all game files are available.\nSee documentation for further information.\n");
            return;
        }

        if (apk::video::createScreen("Cruise for a Corpse", 320, 200, 8) == false) {
            return;
        }

        CruiseEngine engine(g_system, nullptr);
        _vm = &engine;
        _vm->run();
    }

}

namespace Cruise {

    void PaletteManager::setPalette(uint8* pal, uint32 start, uint32 end) {
        if (!ignoreUpdates) {
            apk::video::setRGB(pal, start, end);
        }
    }

}
