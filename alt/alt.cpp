#include "alt.h"
#include "cruise/cruise.h"
#include "apk/gfx.h"

namespace Cruise {
    OSystem OSystem::s_instance = {};

    void OSystem::updateScreen() {
        apk::gfx::flipScreen();
    }

    void OSystem::copyRectToScreen(uint8* data, uint32 pitch, uint32 x, uint32 y, uint32 w, uint32 h) {
        apk::gfx::blit(data, pitch, x, y, w, h);
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