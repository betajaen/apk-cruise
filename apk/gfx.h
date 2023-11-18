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

namespace apk {

    struct Event;

    namespace gfx {

        typedef void(*WindowEventFn)(void* user, Event& evt);
        typedef void(*WindowTimerFn)(void* user);

        bool createScreen(const char* title, uint16 width, uint16 height, uint8 depth);
        void destroyScreen();
        void windowStartLoop(uint32 waitTime_usec);
        void windowStopLoop();
        void pushWindowEventCallback(WindowEventFn, void*);
        void pushWindowTimerCallback(WindowTimerFn, void*);
        void popWindowEventCallback();
        void popWindowTimerCallback();
        void flipScreen();
        void writeChunkyPixels(uint8* data);
        void writeChunkyPixelsBlit(uint8* data, uint32 x, uint32 y, uint32 w, uint32 h, uint32 stride);
        void writeRect(uint32 l, uint32 t, uint32 r, uint32 b, uint8 col);
        void writePixel(uint32 x, uint32 y, uint8 col);
        void clearChunkyPixels(uint8 index);
        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b);
        void setRGB(uint8* pal, uint32 begin, uint32 end);
        void clearPalette();

    }

}