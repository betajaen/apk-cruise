#include "engine/engine.h"
#include "cruise/cruise.h"
#include "apk/gfx.h"

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

    void gameMain() {
        if (apk::gfx::createScreen("Cruise for a Corpse", 320, 200, 8) == false) {
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
            apk::gfx::setRGB(pal, start, end);
        }
    }

}