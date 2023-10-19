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

        void createScreen(uint16 width, uint16 height, uint8 depth);
        void destroyScreen();
        void flipScreen();
        void blit(uint8* data, uint32 pitch, uint32 x, uint32 y, uint32 w, uint32 h);
        void blit(uint8* data, uint32 size);
        void box(uint32 left, uint32 top, uint32 right, uint32 bottom);
        void cls(uint8 index);
        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b);
        void setRGB(uint8* pal, uint32 begin, uint32 end);

    }

}