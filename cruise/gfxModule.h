/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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

#ifndef CRUISE_GFXMODULE_H
#define CRUISE_GFXMODULE_H

#define GFX_TILE_W 32
#define GFX_TILE_H 25

namespace Cruise {

struct gfxModuleDataStruct {
	int useTandy;
	int useEGA;
	int useVGA;

	uint8 *pPage00;
	uint8 *pPage10;
};

struct palEntry {
	uint8 R;
	uint8 G;
	uint8 B;
	//uint8 A;
};

extern gfxModuleDataStruct gfxModuleData;

void gfxModuleData_initFrameBuffers(); // MOD:
void gfxModuleData_deleteFrameBuffers(); // MOD:

void gfxModuleData_gfxClearFrameBuffer(uint8 *ptr);
void gfxModuleData_setDirtyColors(int min, int max);
void gfxModuleData_setPalColor(int idx, int r, int g, int b);
void gfxModuleData_field_90();
void gfxModuleData_gfxWaitVSync();
void gfxModuleData_flip();
void gfxModuleData_updatePalette(bool force = false);
void gfxModuleData_updateScreen();

void gfxCopyRect(const uint8 *sourceBuffer, int width, int height, byte *dest, int x, int y, int color);
void gfxModuleData_gfxCopyScreen(const uint8 *sourcePtr, uint8 *destPtr);
void convertGfxFromMode4(const uint8 *sourcePtr, int width, int height, uint8 *destPtr);
void convertGfxFromMode5(const uint8 *sourcePtr, int width, int height, uint8 *destPtr);
void gfxModuleData_Init();
void gfxModuleData_flipScreen();
//void gfxModuleData_setPal(uint8 * ptr);
void gfxModuleData_clearAll();
void gfxModuleData_zeroPalette();
void gfxModuleData_convertOldPalColor(uint16 oldColor, uint8 *pOutput);
void gfxModuleData_setPalEntries(const byte *ptr, int start, int num);
void gfxModuleData_setPal256(const byte *ptr);
void gfxModuleData_addDirtyRect(const Common::Rect &r);
void gfxModuleData_addDirtyTile(uint16 x, uint16 y); // MOD:
void gfxModuleData_addDirtyColumn(uint16 x); // MOD:
void gfxModuleData_addDirtyAll(); // MOD:
#define RECT_TYPE_ANY     1  // MOD:
#define RECT_TYPE_SPRITE  2  // MOD:
#define RECT_TYPE_TEXT    3  // MOD:
#define RECT_TYPE_POLYGON 4  // MOD:
void gfxModuleData_addDirtyTileRect(int16 l, int16 t, int16 r, int16 b, uint8 type); // MOD:
void flip();
void drawSolidBox(int32 x1, int32 y1, int32 x2, int32 y2, uint8 color);
void resetBitmap(uint8 *dataPtr, int32 dataSize);

void switchBackground(const byte *newBg);

} // End of namespace Cruise

#endif
