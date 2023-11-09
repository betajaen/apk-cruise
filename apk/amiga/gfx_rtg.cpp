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

#include "apk/types.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/cybergraphics.h>
#include <proto/graphics.h>
#include <proto/datatypes.h>

#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <cybergraphx/cybergraphics.h>

#include <graphics/gfx.h>
#include <graphics/scale.h>
#include <graphics/displayinfo.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

struct Library* CyberGfxBase = NULL;

namespace apk {


    namespace gfx {

		struct Screen* mScreen;
		extern struct Window* mWindow;
		struct ScreenBuffer* mScreenBuffer;
		struct RastPort mRastPort;

        static ULONG sPalette[2 + (256 * 3)] = { 0 };

        bool createWindow(struct Screen* screen, uint16 width, uint16 height, uint8 depth);
        void destroyWindow();

        bool createScreen(const char* title, uint16 width, uint16 height, uint8 depth) {

            CyberGfxBase = OpenLibrary("cybergraphics.library", 41);
            if (!CyberGfxBase) {
                requester_okay("Error!", "Cannot open cybergraphics.library V41.");
                return false;
            }

            ULONG modeId = BestCModeIDTags(
                CYBRBIDTG_NominalWidth, (ULONG) width,
                CYBRBIDTG_NominalHeight, (ULONG) height,
                CYBRBIDTG_Depth, (ULONG) depth,
                TAG_DONE
		    );

            if (modeId == INVALID_ID) {
                requester_okay("Error!", "Cannot find appropriate RTG Screen Mode");
                return 1;
            }

            mScreen = OpenScreenTags(NULL,
                SA_DisplayID, modeId,
                SA_Left, 0UL,
                SA_Top, 0UL,
                SA_Width, (ULONG) width,
                SA_Height, (ULONG) height,
                SA_Depth, (ULONG) depth,
                SA_Title, (ULONG) title,
                SA_Type, CUSTOMSCREEN,
                SA_SysFont, 1,
                TAG_DONE
		    );

            mScreenBuffer = AllocScreenBuffer(
                mScreen,
                NULL,
                SB_SCREEN_BITMAP
            );


            if (mScreenBuffer == NULL) {
                requester_okay("Error", "Could open ScreenBuffer for Screen!");
                return false;
            }

            sPalette[0] = 256L << 16 | 0;
            sPalette[1] = 0xFFffffff;
            sPalette[2] = 0xFFffffff;
            sPalette[3] = 0xFFffffff;

            LoadRGB32(&mScreen->ViewPort, sPalette);

            InitRastPort(&mRastPort);
            mRastPort.BitMap = mScreenBuffer->sb_BitMap;

            if (createWindow(mScreen, width, height, depth) == false)
                return false;

            return true;
        }

        void destroyScreen() {

            if (mScreenBuffer && mScreen) {
                FreeScreenBuffer(mScreen, mScreenBuffer);
                mScreenBuffer = NULL;
            }

            destroyWindow();

            if (mScreen) {
                CloseScreen(mScreen);
                mScreen = NULL;
            }

            if (CyberGfxBase) {
                CloseLibrary(CyberGfxBase);
            }

        }

        void flipScreen() {
        }

        void writeChunkyPixels(uint8* data) {
            WritePixelArray(data, 0, 0, 320, &mRastPort, 0, 0, 320, 240, RECTFMT_LUT8);
        }

        void clearChunkyPixels(uint8 index) {
            SetRast(&mRastPort, index);
        }

        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b) {
            ULONG* dst = &sPalette[1 + index*3];
            *dst++ = r << 24 | 0x00FFffFF;
            *dst++ = g << 24 | 0x00FFffFF;
            *dst++ = b << 24 | 0x00FFffFF;
        }

        void setRGB(uint8* pal, uint32 begin, uint32 end) {
            ULONG* dst = &sPalette[1 + begin*3];
            for(uint32 i=begin;i < end;i++) {
                *dst++ = *pal++ << 24 | 0x00FFffFF;
                *dst++ = *pal++ << 24 | 0x00FFffFF;
                *dst++ = *pal++ << 24 | 0x00FFffFF;
            }
			LoadRGB32(&mScreen->ViewPort, &sPalette[0]);
        }
    }

}