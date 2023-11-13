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

#include <exec/types.h>
#include <exec/ports.h>

#include <proto/timer.h>
#include <inline/timer.h>

struct Device* TimerBase;

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>


namespace apk {


    namespace gfx {

		struct Window* mWindow;
        static bool mWindowLoopStop = FALSE;
        static struct timeval _time0;


        class SystemTimer {

            struct MsgPort* _msgPort;
            struct timerequest* _req;
            uint32 _signalBit;

        public:

            SystemTimer() :
                _msgPort(NULL), _req(NULL) {
            }

            ~SystemTimer() {

            }

            bool open() {
                if (_msgPort) {
                    close();
                }

                _msgPort = CreatePort(NULL, 0);
                if (_msgPort == NULL) {
                    requester_okay("Error", "Could not open message port");
                    return false;
                }

                _req = (struct timerequest*) CreateExtIO(_msgPort, sizeof(struct timerequest));

                if (_req == NULL) {
                    DeletePort(_msgPort);
                    _msgPort = NULL;
                    requester_okay("Error", "Could not create timer request");
                    return false;
                }

                if (OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest*)_req, 0) != 0) {
                    DeleteExtIO((struct IORequest*) _req);
                    _req = NULL;
                    DeletePort(_msgPort);
                    _msgPort = NULL;
                    return false;
                }

                TimerBase = (struct Device*) _req->tr_node.io_Device;

                GetSysTime(&_time0);

                return true;
            }

            void close() {
                 if (_req != NULL) {

                    AbortIO((struct IORequest*) _req);
                    WaitIO((struct IORequest*) _req);

                    CloseDevice((struct IORequest*) _req);
                    DeleteExtIO((struct IORequest*) _req);
                    _req = NULL;
                }

                if (_msgPort != NULL) {
                    DeletePort(_msgPort);
                    _msgPort = NULL;
                }
            }

            uint32 start(uint32 microSeconds) {
                _req->tr_time.tv_secs = 0;
                _req->tr_time.tv_micro = microSeconds;
                _req->tr_node.io_Command = TR_ADDREQUEST;
                SendIO(&_req->tr_node);

                return 1 << _msgPort->mp_SigBit;
            }

            bool isReady() {
                WaitPort(_msgPort);
                GetMsg(_msgPort);

                return true;
            }

            uint32 getSignalBit() const {
                if (_msgPort != NULL) {
                    return 1 << _msgPort->mp_SigBit;
                }
                else {
                    return 0;
                }
            }

        };



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
                WA_RMBTrap, TRUE,
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

        void windowStopLoop() {
            mWindowLoopStop = TRUE;
        }

        typedef void(*WindowEventFn)(void* user, Event& evt);
        typedef void(*WindowTimerFn)(void* user);

        void windowLoop(void* user, uint32 waitTime_usec, WindowEventFn evtFn, WindowTimerFn timerFn) {

            struct IntuiMessage* msg;


            SystemTimer mTimer;
            mTimer.open();

            ULONG timerBit = mTimer.start(waitTime_usec);
            ULONG windowBit = (1 << mWindow->UserPort->mp_SigBit);
		    ULONG signalBits = windowBit | timerBit | SIGBREAKF_CTRL_C;

            mWindowLoopStop = FALSE;
            uint32 mouseX = 0, mouseY = 0;

		    while (mWindowLoopStop == false) {

                ULONG signal = Wait(signalBits);

                if (signal & SIGBREAKF_CTRL_C) {
                    mWindowLoopStop = true;
                    break;
                }

                if (signal & windowBit) {

                    while ((msg = (struct IntuiMessage*)GetMsg(mWindow->UserPort)) != NULL)
                    {
                        Event evt;
                        evt.type = EVENT_NONE;

                        switch (msg->Class)
                        {
                            case IDCMP_CLOSEWINDOW: {
                                mWindowLoopStop = TRUE;
                            }
                            break;
                            case IDCMP_VANILLAKEY: {
                                mouseX = msg->MouseX;
                                mouseY = msg->MouseY;

                                if (msg->Code == 27) {
                                    mWindowLoopStop = TRUE;
                                }
                            }
                            break;
                            case IDCMP_MOUSEMOVE: {
                                mouseX = msg->MouseX;
                                mouseY = msg->MouseY;
                            }
                            break;
                            case IDCMP_MOUSEBUTTONS: {
                                mouseX = msg->MouseX;
                                mouseY = msg->MouseY;

                                evt.mouse.x = mouseX;
                                evt.mouse.y = mouseY;

                                if (msg->Code == SELECTUP) {
                                    evt.type = EVENT_LBUTTONUP;
                                }
                                else if (msg->Code == SELECTDOWN){
                                    evt.type = EVENT_LBUTTONDOWN;
                                }

                                if (msg->Code == MENUUP) {
                                    evt.type = EVENT_RBUTTONUP;
                                }
                                else if (msg->Code == MENUDOWN){
                                    evt.type = EVENT_RBUTTONDOWN;
                                }

                            }
                            break;
                        }

                        ReplyMsg((struct Message*)msg);
                        if (evt.type != EVENT_NONE) {
                            evtFn(user, evt);
                        }
                    }
                }

                if (signal & timerBit) {
                    if (mTimer.isReady()) {
                        timerFn(user);
                        mTimer.start(waitTime_usec);
                    }
                }

		    }

		    mTimer.close();


        }
    }
}