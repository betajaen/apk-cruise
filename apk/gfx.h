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

    namespace gfx {

        bool createScreen(const char* title, uint16 width, uint16 height, uint8 depth);
        void destroyScreen();
        void flipScreen();
        void writeChunkyPixels(uint8* data);
        void clearChunkyPixels(uint8 index);
        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b);
        void setRGB(uint8* pal, uint32 begin, uint32 end);

    }

}