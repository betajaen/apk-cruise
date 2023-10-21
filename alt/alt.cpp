#include "alt.h"
#include "cruise/cruise.h"
#include "apk/gfx.h"

namespace Cruise {
    OSystem OSystem::s_instance = {};

    void OSystem::updateScreen() {
        apk::gfx::flipScreen();
    }

    void OSystem::copyRectToScreen(uint8* data, uint32 pitch, uint32 x, uint32 y, uint32 w, uint32 h) {
    }

    void OSystem::copyToScreen(uint8* data, uint32 size) {
        apk::gfx::blit(data, size);
    }

    void OSystem::box(uint32 left, uint32 top, uint32 right, uint32 bottom) {
    }

}

namespace apk {

    void gameMain() {
        CruiseEngine engine(g_system, nullptr);
        _vm = &engine;
        _vm->run();
    }

    void Engine::initGraphics(apk::int32 w, apk::int32 h) {
        apk::gfx::createScreen(w, h, 8);
    }

}

namespace Cruise {

    void PaletteManager::setPalette(uint8* pal, uint32 start, uint32 end) {
        apk::gfx::setRGB(pal, start, end);
    }

}