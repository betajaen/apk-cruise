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

#include "apk/pod.h"

namespace apk {

    namespace bank {

        int32 createSpriteBank(uint16 width, uint16 height, uint16 numSprites);
        void destroySpriteBank(int32 bank);

        void setSpriteBank(int32 bank, uint16 spriteNum, uint8* data);
        void setSpriteBankChunky(int32 bank, uint16 spriteNum, uint8* data, uint32 dataSize, uint8 numBitPlanes);
        void setSpriteOffset(int32 bankNum, uint16 spriteNum, int16 offsetX, int16 offsetY);

        void* getSpriteBankImageData(int32 bank, uint16 spriteNum, uint32* outSize);

    }

}