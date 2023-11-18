/* Amiga Port Kit (APK)
 *
 * (c) Robin Southern - github.com/betajaen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "apk/apk.h"
#include "apk/gfx.h"

namespace apk {
    bool s_FastMode = true;
    uint32 s_FastModeTime = 0;
    bool s_quitRequested = true;

    void gameMain();
}

int apk_main() {
    apk::s_quitRequested = false;

    apk::gameMain();

    apk::gfx::destroyScreen();
}

namespace apk {

    static ULONG ticks = 0;

    void delayMs(uint32 ms) {
        ticks += ms;
    }

    uint32 getMs() {
        ticks++;
        return ticks;
    }
}