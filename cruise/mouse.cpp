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

#include "cruise/cruise_main.h"
#include "cruise/staticres.h"

// MOD: #include "common/system.h"
// MOD: #include "graphics/cursorman.h"

namespace Cruise {

int16 main10;

struct MouseCursor {
	int hotspotX;
	int hotspotY;
	const byte *bitmap;
};

static const MouseCursor mouseCursors[] = {
	{ 1, 1, mouseCursorNormal },
	{ 0, 0, mouseCursorDisk },
	{ 7, 7, mouseCursorCross },
	{ 0, 0, mouseCursorNoMouse },
	{ 10, 6, mouseCursorWalk },
	{ 10, 6, mouseCursorExit },
	{ 10, 6, mouseCursorMagnifyingGlass }
};

CursorType currentCursor = CURSOR_NOMOUSE;

static const byte cursorPalette[] = {
	0, 0, 0,
	0xff, 0xff, 0xff
};

static bool s_InitializedMouseBank = false;

void initMouse() {
	if (s_InitializedMouseBank == false) {
		byte mouseCursor[16 * 16];
		s_InitializedMouseBank = true;
		
		for(uint i=0;i < CURSOR_MAX;i++) {

			const MouseCursor *mc = &mouseCursors[i];
			const byte *src = mc->bitmap;
			for (int i = 0; i < 32; ++i) {
				int offs = i * 8;
				for (byte mask = 0x80; mask != 0; mask >>= 1) {
					if (src[0] & mask) {
						mouseCursor[offs] = 2;
					} else if (src[32] & mask) {
						mouseCursor[offs] = 1;
					} else {
						mouseCursor[offs] = 0;
					}
					++offs;
				}
				++src;
			}

			apk::video::createSpriteBitMapFromChunky(i, 16, 16, mouseCursor);
		}
	}

	apk::video::setSpriteVisible(0, true);
}

void changeCursor(CursorType eType) {
	assert(eType >= 0 && eType < CURSOR_MAX);

#if 1 // MOD:
	if (eType == 3)
		return;

    if (eType == currentCursor) {
		return;
	}

	currentCursor = eType;

	if (s_InitializedMouseBank == false) {
		initMouse();
	}
	//else {
	const MouseCursor *mc = &mouseCursors[currentCursor];
	apk::video::setSpriteBanked(0, currentCursor);
	apk::video::setSpriteOffset(0, -mc->hotspotX, -mc->hotspotY);
    apk::video::setSpriteVisible(0, true);
	//}

	/*
	byte mouseCursor[16 * 16];
	const MouseCursor *mc = &mouseCursors[eType];
	const byte *src = mc->bitmap;
	for (int i = 0; i < 32; ++i) {
		int offs = i * 8;
		for (byte mask = 0x80; mask != 0; mask >>= 1) {
			if (src[0] & mask) {
				mouseCursor[offs] = 2;
			} else if (src[32] & mask) {
				mouseCursor[offs] = 1;
			} else {
				mouseCursor[offs] = 0;
			}
			++offs;
		}
		++src;
	}

	apk::gfx::setCursorChunky((uint8*) mouseCursor, sizeof(mouseCursor), 16, 16, -mc->hotspotX, -mc->hotspotY);
	*/
    return;
#else	
	if (currentCursor != eType) {
		byte mouseCursor[16 * 16];
		const MouseCursor *mc = &mouseCursors[eType];
		const byte *src = mc->bitmap;
		for (int i = 0; i < 32; ++i) {
			int offs = i * 8;
			for (byte mask = 0x80; mask != 0; mask >>= 1) {
				if (src[0] & mask) {
					mouseCursor[offs] = 1;
				} else if (src[32] & mask) {
					mouseCursor[offs] = 0;
				} else {
					mouseCursor[offs] = 0xFF;
				}
				++offs;
			}
			++src;
		}
        CursorMan.replaceCursor(mouseCursor, 16, 16, mc->hotspotX, mc->hotspotY, 0xFF);
		CursorMan.replaceCursorPalette(cursorPalette, 0, 2);
		currentCursor = eType;
	}
#endif
}

bool isMouseOn() {
	return (currentCursor != CURSOR_NOMOUSE) && CursorMan.isVisible();
}

void mouseOff() {
    return; // MOD;
	CursorMan.showMouse(false);
	g_system->updateScreen();
}

void mouseOn() {
    return; // MOD;
	CursorMan.showMouse(true);
	g_system->updateScreen();
}

} // End of namespace Cruise
