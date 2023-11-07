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
#include <proto/graphics.h>
#include <proto/datatypes.h>

#include <intuition/intuition.h>
#include <intuition/screens.h>

#include <graphics/gfx.h>
#include <graphics/scale.h>
#include <graphics/displayinfo.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

namespace apk {


    namespace gfx {

		struct Window* mWindow;

        bool createWindow(struct Screen* screen, uint16 width, uint16 height, uint8 depth) {

            mWindow = OpenWindowTags(NULL,
                WA_Left, 0,
                WA_Top, 0,
                WA_Width, width,
                WA_Height, height,
                WA_CustomScreen, (ULONG) screen,
                WA_Backdrop, TRUE,
                WA_Borderless, TRUE,
                WA_DragBar, FALSE,
                WA_Activate, TRUE,
                WA_SimpleRefresh, TRUE,
                WA_CloseGadget, FALSE,
                WA_DepthGadget, FALSE,
                WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_IDCMPUPDATE | IDCMP_MOUSEBUTTONS,
                TAG_END
		    );

            if (mWindow == NULL) {
                requester_okay("Error", "Could open Window for Screen!");
                return false;
            }



            return true;
        }

        void destroyWindow() {


            if (mWindow) {
                CloseWindow(mWindow);
                mWindow = NULL;
            }


        }

    }

    void fetchEvents() {
    }

}