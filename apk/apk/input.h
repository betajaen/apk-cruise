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

#pragma once

#include "types.h"

namespace apk {

    constexpr int32 EVENT_LBUTTONDOWN = 1;
    constexpr int32 EVENT_LBUTTONUP = 2;
    constexpr int32 EVENT_RBUTTONDOWN = 3;
    constexpr int32 EVENT_RBUTTONUP = 4;
    constexpr int32 EVENT_MOUSEMOVE = 5;
    constexpr int32 EVENT_KEYDOWN = 6;
    constexpr int32 EVENT_KEYUP = 7;
    constexpr int32 EVENT_QUIT = 8;
    constexpr int32 EVENT_RETURN_TO_LAUNCHER = 9;
    constexpr int32 EVENT_FAST_MODE = 10;
    constexpr int32 EVENT_SKIP_PROTECTION = 11;

    struct Event {
        int32 type;
        struct Mouse {
            int16 x, y;
        } mouse;
        struct Keyboard {
            int32 keycode;
            bool hasFlags(int32) {
                return false;
            }
        } kbd;
    };

    bool pollEvents(Event& evt);

}