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
// MOD: #include "common/util.h"

namespace Cruise {

typedef char ColorP;

#define SCREENHEIGHT 200
#define MAXPTS 10
#define putdot(x,y) {if ((y >= 0) && (y < SCREENHEIGHT)) dots[y][counters[y]++] = x;}

extern char *polyOutputBuffer; // MOD:

void hline(int x1, int x2, int y, char c) {
#if 1 // MOD:
	if (y < 0 || y > 199)
		return;
    if (x2 < x1) {
		SWAP(x1, x2);
	}
	x1 = MAX(0, x1);
	x2 = MIN(319, x2);
	uint32 idx = mul_320(y) + x1;
	int32 count = x2 - x1;
	while(count--) {
		polyOutputBuffer[idx++] = c;
	}
#else
	for (; x1 <= x2; x1++) {
		pixel(x1, y, c);
	}
#endif
}

void hline_unchecked(int x1, int x2, int y, char c) { // MOD:
	uint32 idx = mul_320(y) + x1;
	int32 count = x2 - x1;
	while(count--) {
		polyOutputBuffer[idx++] = c;
	}
}

void vline(int x, int y1, int y2, char c) {
#if 1 // MOD:
    if (x < 0 || x > 319)
        return;
    if (y2 < y1) {
		SWAP(y1, y2);
	}
    y1 = MAX(0, y1);
    y2 = MIN(199, y2);
    uint32 idx = mul_320(y1) + x;
    uint32 count = y2 - y1;
    while(count--) {
        polyOutputBuffer[idx] = c;
        idx += 320;
    }
#else
	for (; y1 <= y2; y1++) {
		pixel(x, y1, c);
	}
#endif
}

void vline_unchecked(int x, int y1, int y2, char c) {
    uint32 idx = mul_320(y1) + x;
    uint32 count = y2 - y1;
    while(count--) {
        polyOutputBuffer[idx] = c;
        idx += 320;
    }
}

void bsubline_1(int x1, int y1, int x2, int y2, char c) {
	int x, y, ddx, ddy, e;
	ddx = ABS(x2 - x1);
	ddy = ABS(y2 - y1) << 1;
	e = ddx - ddy;
	ddx <<= 1;

	if (x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	CLIP(x1, 0, 319); // MOD:
	CLIP(x2, 0, 319); // MOD:
	CLIP(y1, 0, 199); // MOD:
	CLIP(y2, 0, 199); // MOD:

	for (x = x1, y = y1; x <= x2; x++) {

		// MOD: pixel(x, y, c);
		uint32 idx = mul_320(y) + x;  // MOD:
		polyOutputBuffer[idx] = c;    // MOD:
		if (e < 0) {
			y++;
			e += ddx - ddy;
		} else {
			e -= ddy;
		}
	}

}

void bsubline_2(int x1, int y1, int x2, int y2, char c) {

	int x, y, ddx, ddy, e;
	ddx = ABS(x2 - x1) << 1;
	ddy = ABS(y2 - y1);
	e = ddy - ddx;
	ddy <<= 1;

	if (y1 > y2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	CLIP(x1, 0, 319); // MOD:
	CLIP(x2, 0, 319); // MOD:
	CLIP(y1, 0, 199); // MOD:
	CLIP(y2, 0, 199); // MOD:

	for (y = y1, x = x1; y <= y2; y++) {

		// MOD: pixel(x, y, c);
		uint32 idx = mul_320(y) + x;  // MOD:
		polyOutputBuffer[idx] = c;    // MOD:
		if (e < 0) {
			x++;
			e += ddy - ddx;
		} else {
			e -= ddx;
		}
	}

}

void bsubline_3(int x1, int y1, int x2, int y2, char c) {

	int x, y, ddx, ddy, e;

	ddx = ABS(x1 - x2) << 1;
	ddy = ABS(y2 - y1);
	e = ddy - ddx;
	ddy <<= 1;

	if (y1 > y2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	CLIP(x1, 0, 319); // MOD:
	CLIP(x2, 0, 319); // MOD:
	CLIP(y1, 0, 199); // MOD:
	CLIP(y2, 0, 199); // MOD:

	for (y = y1, x = x1; y <= y2; y++) {

		// MOD: pixel(x, y, c);
		uint32 idx = mul_320(y) + x;  // MOD:
		polyOutputBuffer[idx] = c;    // MOD:

		if (e < 0) {
			x--;
			e += ddy - ddx;
		} else {
			e -= ddx;
		}
	}

}

void bsubline_4(int x1, int y1, int x2, int y2, char c) {

	int x, y, ddx, ddy, e;

	ddy = ABS(y2 - y1) << 1;
	ddx = ABS(x1 - x2);
	e = ddx - ddy;
	ddx <<= 1;

	if (x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	CLIP(x1, 0, 319); // MOD:
	CLIP(x2, 0, 319); // MOD:
	CLIP(y1, 0, 199); // MOD:
	CLIP(y2, 0, 199); // MOD:

	for (x = x1, y = y1; x <= x2; x++) {

		// MOD: pixel(x, y, c);
		uint32 idx = mul_320(y) + x;  // MOD:
		polyOutputBuffer[idx] = c;    // MOD:

		if (e < 0) {
			y--;
			e += ddx - ddy;
		} else {
			e -= ddy;
		}
	}
}

void line(int x1, int y1, int x2, int y2, char c) {
	// MOD: float k;

	if ((x1 == x2) && (y1 == y2)) {
		pixel(x1, y1, c);
		return;
	}

	if (x1 == x2) {
		vline(x1, MIN(y1, y2), MAX(y1, y2), c);
		return;
	}

	if (y1 == y2) {
		hline(MIN(x1, x2), MAX(x1, x2), y1, c);
		return;
	}
#if 0 // MOD:
	// FPU support on Amiga is optional, this will crash on most Amigas.
	// TODO: Change this to a better algorithm.
	k = (float)(y2 - y1) / (float)(x2 - x1);

	if ((k >= 0) && (k <= 1)) {
		bsubline_1(x1, y1, x2, y2, c);
	} else if (k > 1) {
		bsubline_2(x1, y1, x2, y2, c);
	} else if ((k < 0) && (k >= -1)) {
		bsubline_4(x1, y1, x2, y2, c);
	} else {
		bsubline_3(x1, y1, x2, y2, c);
	}
#endif
}

// Filled polygons. This probably isn't pixel-perfect compared to the original,
// but it seems to work a bit better than the previous version.

static void add_intersect(int *intersect, int x, byte &num) {
	if (num < MAXPTS) {

		int i = 0;
		for (i = num; i > 0 && intersect[i - 1] > x; i--)
			intersect[i] = intersect[i - 1];

		intersect[i] = x;
		num++;
	}
}

void fillpoly(int16 *point_data, int lineCount, ColorP color) {
	static int intersect[SCREENHEIGHT][MAXPTS];
	static byte num_intersect[SCREENHEIGHT];

	switch (lineCount) {
	case 0:		// do nothing
		return;
	case 1:		// draw pixel
		pixel(point_data[0], point_data[1], color);
		return;
	case 2:		// draw line
		line(point_data[0], point_data[1], point_data[2], point_data[3], color);
		return;
	default:		// go on and draw polygon
		break;
	}

	// Reinit array counters
	for (int i = 0; i < SCREENHEIGHT; i++) {
		num_intersect[i] = 0;
	}

	// Find the top/bottom of the polygon.
	int top = point_data[1];
	int bottom = point_data[1];

	for (int i = 1; i < lineCount; i++) {
		if (point_data[2 * i + 1] < top)
			top = point_data[2 * i + 1];
		else if (point_data[2 * i + 1] > bottom)
			bottom = point_data[2 * i + 1];
	}

	if (top < 0)
		top = 0;
	if (bottom >= SCREENHEIGHT)
		bottom = SCREENHEIGHT - 1;

	// Calculate intersections for each scan line
	for (int y = top; y <= bottom; y++) {
		int x2 = point_data[2 * lineCount - 2];
		int y2 = point_data[2 * lineCount - 1];

		for (int i = 0; i < lineCount; i++) {
			int x1 = x2;
			int y1 = y2;
			x2 = point_data[2 * i];
			y2 = point_data[2 * i + 1];

			// Test if the line intersects the scan line

			if ((y < y1) != (y < y2)) {
				if (y1 == y2) {
					add_intersect(intersect[y], x1, num_intersect[y]);
					add_intersect(intersect[y], x2, num_intersect[y]);
				} else if (x1 == x2) {
					add_intersect(intersect[y], x1, num_intersect[y]);
				} else {
					add_intersect(intersect[y], x1 + ((y - y1) * (x2 - x1)) / (y2 - y1), num_intersect[y]);
				}
			}
		}
	}

	// Drawing.
	for (int y = top; y <= bottom; y++) {
		for (int i = 0; i < num_intersect[y]; i += 2) {
			hline(intersect[y][i], intersect[y][i + 1], y, color);
		}
	}
}

} // End of namespace Cruise
