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


// MOD: #include "common/system.h"
// MOD: #include "common/endian.h"
// MOD: #include "common/list.h"
// MOD: #include "common/rect.h"

// MOD: #include "graphics/palette.h"

#include "cruise/cruise.h"
#include "cruise/cruise_main.h"
#include "cruise/gfxModule.h" // MOD:

namespace Cruise {

static APK_ALIGNED uint8* page00 = NULL; // MOD: uint8 page00[320 * 200];
static APK_ALIGNED uint8* page10 = NULL; // MOD: uint8 page10[320 * 200];
// MOD: char screen[320 * 200];

palEntry lpalette[256];

int palDirtyMin = 256;
int palDirtyMax = -1;

bool _dirtyRectScreen = false;
bool s_paletteIsDirty = false; // MOD:
bool s_ignorePaletteUpdates = false; // MOD:

// MOD:
#define DEBUG_RECTS 0
#define DEBUG_TILES 0
#define DEBUG_FRAME_TIME 0
#define ENABLE_TILE_RENDERER 0

// MOD:
#if ENABLE_TILE_RENDERER == 1
#define TILE_THRESHOLD (MAX_TILES - (MAX_TILES / 3))
static APK_ALIGNED uint8* sDrawTiles = NULL; // MOD:
#endif
#define MAX_TILES ((320 / GFX_TILE_W) * (200 / GFX_TILE_H))  // MOD:
uint16  sDrawTileCount = 0;

#if defined(DEBUG_RECTS)
struct DebugRect {
	uint16 left, top, right, bottom, type;
};
DebugRect sDebugRects[128];
uint16 sDebugRectsCount = 0;

#endif



#if DEBUG_TILES == 1

static uint8 sDebugDrawTiles[MAX_TILES];

static const uint8 sDebugTileCoords[] = {
	0, 0,
	1, 1,
	2, 2,
	3, 2,
	4, 3,
	5, 4,
	6, 5,
	7, 5,
	8, 6,
	9, 7,
	10, 8,
	11, 9,
	12, 9,
	13, 10,
	14, 11,
	15, 12,
	16, 12,
	17, 13,
	18, 14,
	19, 15,
	20, 15,
	21, 16,
	22, 17,
	23, 18,
	24, 19,
	25, 19,
	26, 20,
	27, 21,
	28, 22,
	29, 22,
	30, 23,
	31, 24
};

#endif

static void hline_mod(uint8* dst, uint32 l, uint32 r, uint32 y, int16 mod) { // MOD:
	uint32 idx = mul_320(y) + l;
	for(;l < r;l++, idx++) {
		dst[idx] += mod;
	}
}

static void vline_mod(uint8* dst, uint32 x, uint32 t, uint32 b, int16 mod) { // MOD:
	uint32 idx = mul_320(t) + x;
	for(;t < b;t++, idx+=320) {
		dst[idx] += mod;
	}
}

static void rect_mod(uint8* dst, uint32 l, uint32 t, uint32 r, uint32 b, int16 mod) { // MOD:
	hline_mod(dst, l, r, t, mod);
	hline_mod(dst, l, r, b, mod);
	vline_mod(dst, l, t, b, mod);
	vline_mod(dst, r, t, b, mod);
}

static void coord_mod(uint8* dst, uint32 x, uint32 y, uint8* coords, int8 count, int16 mod) {
	while(count--) {
		uint8 mx = *coords++;
		uint8 my = *coords++;
		uint32 idx = (mul_320(my + y)) + x + mx;
		dst[idx] += mod;
	}
}

static void fill_mod(uint8* dst, uint32 l, uint32 t, uint32 r, uint32 b, int16 mod) {
	uint32 idx = mul_320(t) + l;
	for(uint32 y=t;y < b;y++) {
		for(uint32 x=l;x < r;x++) {
			dst[idx++] += mod;
		}
		idx += 320 - (r - l);
	}
}

gfxModuleDataStruct gfxModuleData = {
	0,			// use Tandy
	0,			// use EGA
	1,			// use VGA

	page00,			// pPage00
	page10,			// pPage10
};

void gfxModuleData_deleteFrameBuffers() {
    MemFree(page00); page00 = NULL;
    MemFree(page10); page00 = NULL;
#if ENABLE_TILE_RENDERER == 1
    MemFree(sDrawTiles); sDrawTiles = NULL;
#endif
}

void gfxModuleData_gfxClearFrameBuffer(uint8 *ptr) {
    apk::memset_aligned(ptr, 0, 64000); // MOD: memset(ptr, 0, 64000);
}

void gfxModuleData_gfxCopyScreen(const uint8 *sourcePtr, uint8 *destPtr) {
    apk::memcpy_aligned(destPtr, sourcePtr, 64000); // MOD: memcpy(destPtr, sourcePtr, 320 * 200);
}

void outputBit(char *buffer, int bitPlaneNumber, uint8 data) {
	*(buffer + (8000 * bitPlaneNumber)) = data;
}

void convertGfxFromMode4(const uint8 *sourcePtr, int width, int height, uint8 *destPtr) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width / 16; ++x) {
			for (int bit = 0; bit < 16; ++bit) {
				uint8 color = 0;
				for (int p = 0; p < 4; ++p) {
					if (READ_BE_UINT16(sourcePtr + p * 2) & (1 << (15 - bit))) {
						color |= 1 << p;
					}
				}
				*destPtr++ = color;
			}
			sourcePtr += 8;
		}
	}
}

void convertGfxFromMode5(const uint8 *sourcePtr, int width, int height, uint8 *destPtr) {
	int range = (width / 8) * height;

	for (int line = 0; line < 200; line++) {
		uint8 p0;
		uint8 p1;
		uint8 p2;
		uint8 p3;
		uint8 p4;

		for (int col = 0; col < 40; col++) {
			for (int bit = 0; bit < 8; bit++) {
				p0 = (sourcePtr[line*40 + col + range * 0] >> bit) & 1;
				p1 = (sourcePtr[line*40 + col + range * 1] >> bit) & 1;
				p2 = (sourcePtr[line*40 + col + range * 2] >> bit) & 1;
				p3 = (sourcePtr[line*40 + col + range * 3] >> bit) & 1;
				p4 = (sourcePtr[line*40 + col + range * 4] >> bit) & 1;

				destPtr[line * width + col * 8 + (7-bit)] = p0 | (p1 << 1) | (p2 << 2) | (p3 << 3) | (p4 << 4);
			}
		}
	}
}

void gfxModuleData_setDirtyColors(int min, int max) {
	if (min < palDirtyMin)
		palDirtyMin = min;
	if (max > palDirtyMax)
		palDirtyMax = max;
}

void gfxModuleData_setPalColor(int idx, int r, int g, int b) {
	lpalette[idx].R = r;
	lpalette[idx].G = g;
	lpalette[idx].B = b;
	gfxModuleData_setDirtyColors(idx, idx);
}

void gfxModuleData_setPalEntries(const byte *ptr, int start, int num) {
	for (int i = start; i < start + num; i++) {
		int R = *(ptr++);
		int G = *(ptr++);
		int B = *(ptr++);

		lpalette[i].R = R;
		lpalette[i].G = G;
		lpalette[i].B = B;
		// MOD: lpalette[i].A = 255;
	}

	gfxModuleData_setDirtyColors(start, start + num - 1);
}

void gfxModuleData_setPal256(const byte *ptr) {
	gfxModuleData_setPalEntries(ptr, 0, 256);
}

/*void gfxModuleData_setPal(uint8 *ptr) {
	int R;
	int G;
	int B;

	for (int i = 0; i < 256; i++) {
#define convertRatio 36.571428571428571428571428571429
		uint16 atariColor = *ptr;
		//bigEndianShortToNative(&atariColor);
		ptr ++;

		R = (int)(convertRatio * ((atariColor & 0x700) >> 8));
		G = (int)(convertRatio * ((atariColor & 0x070) >> 4));
		B = (int)(convertRatio * ((atariColor & 0x007)));

		if (R > 0xFF)
			R = 0xFF;
		if (G > 0xFF)
			G = 0xFF;
		if (B > 0xFF)
			B = 0xFF;

		lpalette[i].R = R;
		lpalette[i].G = G;
		lpalette[i].B = B;
		lpalette[i].A = 255;
	}

	gfxModuleData_setDirtyColors(0, 16);
}*/

void gfxModuleData_convertOldPalColor(uint16 oldColor, uint8 *pOutput) {
	int R;
	int G;
	int B;

#define convertRatio 36.571428571428571428571428571429

	R = (int)(convertRatio * ((oldColor & 0x700) >> 8));
	G = (int)(convertRatio * ((oldColor & 0x070) >> 4));
	B = (int)(convertRatio * ((oldColor & 0x007)));

	if (R > 0xFF)
		R = 0xFF;
	if (G > 0xFF)
		G = 0xFF;
	if (B > 0xFF)
		B = 0xFF;

	*(pOutput++) = R;
	*(pOutput++) = G;
	*(pOutput++) = B;
}

void gfxModuleData_gfxWaitVSync() {
}

void gfxModuleData_flip() {
}

void gfxCopyRect(const uint8 *sourceBuffer, int width, int height, byte *dest, int x, int y, int color) {
	int xp, yp;

	for (yp = 0; yp < height; ++yp) {
		const uint8 *srcP = &sourceBuffer[yp * width];
		uint8 *destP = &dest[(y + yp) * 320 + x];

		for (xp = 0; xp < width; ++xp, ++srcP, ++destP) {
			uint8 v = *srcP;
			int xDest = x + xp;
			int yDest = y + yp;

			if ((v != 0) && (xDest >= 0) && (yDest >= 0) && (xDest < 320) && (yDest < 200))
				*destP = (v == 1) ? 0 : color;
		}
	}
}

void gfxModuleData_Init() {
    page00 = (uint8*) MemAlloc(64000); // MOD:
    page10 = (uint8*) MemAlloc(64000); // MOD:
	gfxModuleData.pPage00 = page00; // MOD:
	gfxModuleData.pPage10 = page10; // MOD:
#if ENABLE_TILE_RENDERER == 1
    sDrawTiles = (uint8*) MemAlloc(sizeof(uint8) * MAX_TILES);
#endif
	// MOD: memset(globalScreen, 0, 320 * 200);
	// MOD: memset(page00, 0, 320 * 200);
	// MOD: memset(page10, 0, 320 * 200);
}

void gfxModuleData_clearAll() { // MOD:
    apk::memset_aligned(page00, 0, 64000);
    apk::memset_aligned(page10, 0, 64000);
}

void gfxModuleData_zeroPalette() { // MOD:
    apk::gfx::clearPalette();
}

void gfxModuleData_hilight(uint32 x, uint32 y, uint32 w, uint32 h) {
    uint32 r = x + w;
    uint32 b = y + h;
    for(uint32 j=y;j < b;j++) {
        uint32 offset = (x + (j * 320));
        for(uint32 i=x;i < r;i++) {
            gfxModuleData.pPage00[offset]++;
            offset++;
        }
    }
}

void gfxModuleData_unhilight(uint32 x, uint32 y, uint32 w, uint32 h) {
    uint32 r = x + w;
    uint32 b = y + h;
    for(uint32 j=y;j < b;j++) {
        uint32 offset = (x + (j * 320));
        for(uint32 i=x;i < r;i++) {
            gfxModuleData.pPage00[offset]--;
            offset++;
        }
    }
}

#if DEBUG_FRAME_TIME == 1
static uint32 sFrameNum = 0;
#endif

void gfxModuleData_flipScreen() {
	// MOD: memcpy(globalScreen, gfxModuleData.pPage00, 320 * 200);

	uint32 newCount = 0;

#if DEBUG_RECTS == 1
	{
		for (uint16 i = 0; i < sDebugRectsCount; i++) {
			DebugRect& dr = sDebugRects[i];
			rect_mod(gfxModuleData.pPage00, dr.left, dr.top, dr.right, dr.bottom, +100);
		}
	}
#endif

#if DEBUG_TILES == 1
	{
		uint32 x = 0, y = 0;

		for (uint16 i = 0; i < MAX_TILES; i++) {
			uint8 v = sDrawTiles[i];
			if (v != 0) {
				fill_mod(gfxModuleData.pPage00, x, y, x + GFX_TILE_W, y + GFX_TILE_H, +100);
			}

			x += GFX_TILE_W;
			if (x == 320) {
				x = 0;
				y += GFX_TILE_H;
			}
		}

        memcpy(sDebugDrawTiles, sDrawTiles, MAX_TILES);
	}
#endif

	if (ENABLE_TILE_RENDERER == 0 || sDrawTileCount == MAX_TILES) {
#if DEBUG_FRAME_TIME == 1
    sFrameNum++;
#endif
		apk::gfx::writeChunkyPixels(gfxModuleData.pPage00);

#if DEBUG_RECTS == 1
		apk::gfx::writePixel(64 + (sFrameNum & 0xF), 2, sFrameNum & 0xF);
#endif

#if ENABLE_TILE_RENDERER == 1
		apk::memset_aligned(sDrawTiles, 0, MAX_TILES);
#endif
	}
	else if (sDrawTileCount != 0) {
#if ENABLE_TILE_RENDERER == 1
#if DEBUG_FRAME_TIME == 1 \
    sFrameNum++;
#endif

#if DEBUG_RECTS == 1
		apk::gfx::writePixel(64 + (sFrameNum & 0xF), 2, sFrameNum & 0xF);
#endif

		uint32 offset = 0, x = 0, y = 0;
		for (uint16 i = 0; i < MAX_TILES; i++) {
			uint8 v = sDrawTiles[i];
			if (v != 0) {

				apk::gfx::writeChunkyPixelsBlit(gfxModuleData.pPage00 + offset,
					x,
					y,
					GFX_TILE_W,
					GFX_TILE_H,
					320);

				v = (v-1) & 2;
				sDrawTiles[i] = v;
				if (v) {
					newCount++;
				}
			}

			x += GFX_TILE_W;
			if (x == 320) {
				x = 0;
				y += GFX_TILE_H;
				offset = mul_320(y);
			}
			else {
				offset += GFX_TILE_W;
			}
		}
#endif
	}

	sDrawTileCount = newCount;

#if DEBUG_TILES == 1
	{
		uint32 x = 0, y = 0;

		for (uint16 i = 0; i < MAX_TILES; i++) {
			uint8 v = sDrawTiles[i];
			if (v != 0) {
				fill_mod(gfxModuleData.pPage00, x, y, x + GFX_TILE_W, y + GFX_TILE_H, -100);
			}

			x += GFX_TILE_W;
			if (x == 320) {
				x = 0;
				y += GFX_TILE_H;
			}
		}

        apk::gfx::writeChunkyPixelsBlit(sDebugDrawTiles, 0, 0, (320 / GFX_TILE_W), (200 / GFX_TILE_H), (320 / GFX_TILE_W));
	}
#endif

#if DEBUG_RECTS == 1
	{
		for (uint16 i = 0; i < sDebugRectsCount; i++) {
			DebugRect& dr = sDebugRects[i];
			rect_mod(gfxModuleData.pPage00, dr.left, dr.top, dr.right, dr.bottom, -100);
		}
		sDebugRectsCount = 0;
	}
#endif


#if DEBUG_FRAME_TIME == 1
    apk::gfx::writePixel(63 + (sFrameNum & 0xF), 0, 0);
    apk::gfx::writePixel(64 + (sFrameNum & 0xF), 0, sFrameNum & 0xF);
    apk::gfx::writePixel(65 + (sFrameNum & 0xF), 0, 0);
#endif

}

void gfxModuleData_addDirtyTile(uint16 x, uint16 y) { // MOD:
#if ENABLE_TILE_RENDERER == 1
    x = MIN(x, GFX_TILE_W - 1);
    y = MIN(y, GFX_TILE_H - 1);

    uint32 idx = x + (y * (320 / GFX_TILE_W));
    uint8  state = sDrawTiles[idx];
    if (state == 0) {
		sDrawTileCount++;
		sDrawTiles[idx] = 3;
    }
#endif
}

void gfxModuleData_addDirtyAll() { // MOD:
#if ENABLE_TILE_RENDERER == 1
	if (sDrawTileCount != MAX_TILES) {
		sDrawTileCount = MAX_TILES;
		apk::memset_aligned(sDrawTiles, 0x03030303, MAX_TILES * sizeof(uint8));
	}
#endif
}

void gfxModuleData_addDirtyColumn(uint16 x) { // MOD:
#if ENABLE_TILE_RENDERER == 1
	if (sDrawTileCount == MAX_TILES)
		return;

    x = MIN(x, GFX_TILE_W - 1);

    for(uint y=0;y < (200 / GFX_TILE_H);y++) {
        uint32 idx = x + (y * (320 / GFX_TILE_W));

        uint8  state = sDrawTiles[idx];
        if (state == 0) {
            sDrawTileCount++;
			sDrawTiles[idx] = 3;
        }
    }
#endif
}

#if ENABLE_TILE_RENDERER == 1
static inline int16 grid_x(int16 x) {
    int16 gx = x / (GFX_TILE_W);
	int16 gh = x - (gx * GFX_TILE_W);
	if (gh > (GFX_TILE_W / 2)) {
		gx++;
	}
	return MIN(gx, (320 / GFX_TILE_W) - 1);
}

static inline int16 grid_y(int16 y) {
    int16 gy = y / (GFX_TILE_H);
	int16 gh = y - (gy * GFX_TILE_H);
	if (gh > (GFX_TILE_H / 2)) {
		gy++;
	}
	return MIN(gy, (200 / GFX_TILE_H) - 1);
}
#endif

void gfxModuleData_addDirtyTileRect(int16 left, int16 top, int16 right, int16 bottom, uint8 type) { // MOD:
#if ENABLE_TILE_RENDERER == 1
	if (sDrawTileCount == MAX_TILES)
		return;

	left   = CLIP(left, (int16) 0, (int16) 319);
	top    = CLIP(top, (int16) 0, (int16) 199);
	right  = CLIP(right, (int16) 0, (int16) 319);
	bottom = CLIP(bottom, (int16) 0, (int16) 199);

	SORT(left, right);
	SORT(top, bottom);

#if DEBUG_RECTS == 1
	DebugRect dr;
	dr.left = left;
	dr.top = top;
	dr.right = right;
	dr.bottom = bottom;
	dr.type = type;
	sDebugRects[sDebugRectsCount++] = dr;
#endif

	if (left == 0 && top == 0 && right == 319 && bottom == 199) {
		gfxModuleData_addDirtyAll();
		return;
	}

	left = grid_x(left);
	top = grid_y(top);
	right = grid_x(right);
	bottom = grid_y(bottom);

	for(int16 j=top;j <= bottom;j++) {
		for(int i=left;i <= right;i++) {

			uint32 idx = i + (j * (320 / GFX_TILE_W));
			uint8  state = sDrawTiles[idx];

			if (state == 0) {
				sDrawTileCount++;
				sDrawTiles[idx] = 3;
			}

		}
	}
#endif
}

void gfxModuleData_addDirtyRect(const Common::Rect &r) {
#if 0 // MOD:
    _vm->_dirtyRects.push_back(Common::Rect(	MAX(r.left, (int16)0), MAX(r.top, (int16)0),
		MIN(r.right, (int16)320), MIN(r.bottom, (int16)200)));
#endif
}

/**
 * Creates the union of two rectangles.
 */
static bool unionRectangle(Common::Rect &pDest, const Common::Rect &pSrc1, const Common::Rect &pSrc2) {
	pDest.left   = MIN(pSrc1.left, pSrc2.left);
	pDest.top    = MIN(pSrc1.top, pSrc2.top);
	pDest.right  = MAX(pSrc1.right, pSrc2.right);
	pDest.bottom = MAX(pSrc1.bottom, pSrc2.bottom);

	return !pDest.isEmpty();
}

static void mergeClipRects() {
    return; // MOD:
#if 0 // MOD:
	CruiseEngine::RectList::iterator rOuter, rInner;

	for (rOuter = _vm->_dirtyRects.begin(); !rOuter.isEnd(); ++rOuter) {
		rInner = rOuter;
		while (++rInner != _vm->_dirtyRects.end()) {

			if ((*rOuter).intersects(*rInner)) {
				// these two rectangles overlap, so translate it to a bigger rectangle
				// that contains both of them
				unionRectangle(*rOuter, *rOuter, *rInner);

				// remove the inner rect from the list
				_vm->_dirtyRects.erase(rInner);

				// move back to beginning of list
				rInner = rOuter;
			}
		}
	}
#endif
}

void gfxModuleData_updatePalette(bool force) {
#if 0 // MOD:
	byte paletteRGB[256 * 3];

	if (palDirtyMax != -1) {
		for (int i = palDirtyMin; i <= palDirtyMax; i++) {
			paletteRGB[i * 3 + 0] = lpalette[i].R;
			paletteRGB[i * 3 + 1] = lpalette[i].G;
			paletteRGB[i * 3 + 2] = lpalette[i].B;
		}
		s_paletteIsDirty = true; // MOD:
		g_system->getPaletteManager()->setPalette(paletteRGB + palDirtyMin*3, palDirtyMin, palDirtyMax - palDirtyMin + 1);
		palDirtyMin = 256;
		palDirtyMax = -1;
	}
#else
	if (s_ignorePaletteUpdates == false || force) {
		g_system->getPaletteManager()->setPalette((uint8*) lpalette, 0, 255);
	}
#endif
}

void gfxModuleData_updateScreen() {
	g_system->updateScreen();
	g_system->delayMillis(20);
}

void flip() {
#if 0 // MOD:
	CruiseEngine::RectList::iterator dr;

	// Update the palette
	gfxModuleData_updatePalette();

	// Make a copy of the prior frame's dirty rects, and then backup the current frame's rects
	// MOD: CruiseEngine::RectList tempList = _vm->_priorFrameRects;
	// MOD: _vm->_priorFrameRects = _vm->_dirtyRects;
    CruiseEngine::RectList tempList; // MOD:
    _vm->_priorFrameRects.MoveInto(tempList); // MOD:
    _vm->_dirtyRects.CopyInto(_vm->_priorFrameRects); // MOD:

	// Merge the prior frame's dirty rects into the current frame's list
	for (dr = tempList.begin(); dr != tempList.end(); ++dr) {
		Common::Rect &r = *dr;
		_vm->_dirtyRects.push_back(Common::Rect(r.left, r.top, r.right, r.bottom));
	}

	// Merge any overlapping rects to simplify the drawing process
	mergeClipRects();

	// Copy any modified areas
	for (dr = _vm->_dirtyRects.begin(); dr != _vm->_dirtyRects.end(); ++dr) {
		Common::Rect &r = *dr;
		g_system->copyRectToScreen(globalScreen + 320 * r.top + r.left, 320,
			r.left, r.top, r.width(), r.height());
	}

	_vm->_dirtyRects.clear();
    g_system->copyToScreen(globalScreen, sizeof(globalScreen)); // MOD:

	// Allow the screen to update

#endif

}

void drawSolidBox(int32 x1, int32 y1, int32 x2, int32 y2, uint8 color) {
	for (int y = y1; y < y2; ++y) {
		byte *p = &gfxModuleData.pPage00[y * 320 + x1];
		Common::fill(p, p + (x2 - x1), color);
	}
}

void resetBitmap(uint8 *dataPtr, int32 dataSize) {
	memset(dataPtr, 0, dataSize);
}

/**
 * This method compares a new background being switched in against the current background,
 * to figure out rectangles of changed areas for dirty rectangles
 */
void switchBackground(const byte *newBg) {

	gfxModuleData_addDirtyAll();

#if 0 // MOD:
	const byte *bg = gfxModuleData.pPage00;

	// If both the upper corners are different, presume it's a full screen change
	if ((*newBg != *bg) && (*(newBg + 319) != *(bg + 319))) {
		gfxModuleData_addDirtyAll(); // MOD: gfxModuleData_addDirtyRect(Common::Rect(0, 0, 320, 200));
		return;
	}

	/* For an optimisation, any changes are stored as a series of slices than have a height of a single
	 * line each. It is left up to the screen redraw code to automatically merge these together
	 */

	for (int yp = 0; yp < 200; ++yp) {
		int sliceXStart = -1;
		int sliceXEnd = -1;
		for (int xp = 0; xp < 320; ++xp, ++bg, ++newBg) {
			if (*bg != *newBg) {
				if (sliceXStart == -1) {
					// Start of a new slice
					sliceXStart = xp;
					sliceXEnd = MIN(xp + 7, 320);
				} else
					// Carry on of changed area
					sliceXEnd = MAX(xp, sliceXEnd);

			} else if ((sliceXEnd != -1) && (xp >= (sliceXEnd + 10))) {
				// If more than 10 pixels have gone by without any changes, then end the slice
				//// MOD: ROBIN gfxModuleData_addDirtyRect(Common::Rect(sliceXStart, yp, sliceXEnd + 1, MIN(yp + 2, 200)));
				sliceXStart = sliceXEnd = -1;
			}
		}

		//// MOD: ROBIN if (sliceXStart != -1)
		//// MOD: ROBIN 	gfxModuleData_addDirtyRect(Common::Rect(sliceXStart, yp, 320, MIN(yp + 2, 200)));
	}
#endif
}

} // End of namespace Cruise
