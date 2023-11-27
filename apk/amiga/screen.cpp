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

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/datatypes.h>

#include <intuition/intuition.h>
#include <intuition/screens.h>

#include <graphics/gfx.h>
#include <graphics/scale.h>
#include <graphics/displayinfo.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

namespace apk { namespace gfx {

		extern struct Screen* mScreen;
		extern struct Window* mWindow;
		extern struct ScreenBuffer* mScreenBuffer;
		extern struct RastPort mRastPort;
    
        static ULONG sPalette[2 + (256 * 3)] = { 0 };
        static ULONG sFadePalette[2 + (256 * 3)] = { 0 };
        static int32 sPaletteFading;
        static int32 sPaletteFadeSteps;
        static int32 sPaletteFadeTime;
        static int32 sPaletteFadeDest;
        static bool  sPaletteDirty = false;

        void writeRect(uint32 l, uint32 t, uint32 r, uint32 b, uint8 col) {
          SetAPen(&mRastPort, col);

          Move(&mRastPort, l, t);
          Draw(&mRastPort, r, t);
          Draw(&mRastPort, r, b);
          Draw(&mRastPort, l, b);
          Draw(&mRastPort, l, t);
        }

        void clearChunkyPixels(uint8 index) {
            SetRast(&mRastPort, index);
        }

        void writePixel(uint32 x, uint32 y, uint8 col) {
            SetAPen(&mRastPort, col);
            WritePixel(&mRastPort, x, y);
        }

        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b) {
            ULONG* dst = &sPalette[1 + index*3];
            *dst++ = r << 24 | 0x00FFffFF;
            *dst++ = g << 24 | 0x00FFffFF;
            *dst++ = b << 24 | 0x00FFffFF;

            sPaletteDirty = TRUE;
        }

        void setRGB(uint8* pal, uint32 begin, uint32 end) {
            ULONG* dst = &sPalette[1 + begin*3];
            for(uint32 i=begin;i < end;i++) {
                *dst++ = *pal++ << 24 | 0x00FFffFF;
                *dst++ = *pal++ << 24 | 0x00FFffFF;
                *dst++ = *pal++ << 24 | 0x00FFffFF;
            }

            sPaletteDirty = TRUE;
        }

        void clearPalette() {

            memset(sPalette, 0, sizeof(sPalette));

            sPalette[0] = 256L << 16 | 0;
            sPalette[1] = 0xFFffffff;
            sPalette[2] = 0xFFffffff;
            sPalette[3] = 0xFFffffff;

            sPaletteDirty = TRUE;
        }

        void paletteFadeIn(uint32 steps) {
            if (sPaletteFading == 0) {
                sPaletteFadeSteps = (int32) CLIP((int32)steps, (int32)1, (int32)255);
                sPaletteFadeTime = -255;
                sPaletteFadeDest = 0;
                sPaletteFading = 1;
            }
        }

        void paletteFadeOut(uint32 steps) {
            if (sPaletteFading == 0) {
                sPaletteFadeSteps = -(int32) CLIP((int32)steps, (int32)1, (int32)255);
                sPaletteFadeTime = 255;
                sPaletteFadeDest = 0;
                sPaletteFading = -1;
            }
        }

        void paletteFunction() {

            if (sPaletteFading == 0) {
                if (sPaletteDirty) {
                    sPaletteDirty = FALSE;
                    LoadRGB32(&mScreen->ViewPort, sPalette);
                }
            }
            else {
                sPaletteDirty = FALSE;

                ULONG* src = sPalette;
                ULONG* dst = sFadePalette;

                *dst = *src;
                src++;
                dst++;

                for(uint32 i=0;i < 256*3;i++) {
                    int32 col = (int32) ((*src) >> 24Ul);
                    col += sPaletteFadeTime;
                    if (col < 0) {
                        col = 0;
                    }
                    else if (col > 255) {
                        col = 255;
                    }
                    *dst = (((uint32)col) << 24UL) | 0x00FFffFF;
                    src++;
                    dst++;
                }
                
                sPaletteFadeTime += sPaletteFadeSteps;

                /*  ROBIN. This is breaking for some reason.  I think the palette is correctly calculated but it's being corrupted on screen
                    either it's too white, or colours are off.

                    It works as a clamped delta which is added to the current palette (which can be changed at any time by the game), and this
                    value is added to the final colour (sFadePalette). This seems to be the same method as calcRGB in mainDraw.cpp.
                */

                printf("Time = %ld, Dest = %ld, Dir = %ld, Steps = %ld\n", sPaletteFadeTime, sPaletteFadeDest, sPaletteFading, sPaletteFadeSteps);
                
                if (sPaletteFading == -1) {
                    if (sPaletteFadeTime > sPaletteFadeDest) {
                        LoadRGB32(&mScreen->ViewPort, sFadePalette);
                        printf("Palette Fade -1");
                        return;
                    }
                }
                else {
                    if (sPaletteFadeTime < sPaletteFadeDest) {
                        LoadRGB32(&mScreen->ViewPort, sFadePalette);
                        printf("Palette Fade +1");
                        return;
                    }
                }

                sPaletteFading = 0;
                LoadRGB32(&mScreen->ViewPort, sPalette);
                printf("Palette Fade 0");

            }

        }

}}