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
#include "apk/assert.h"

#include <proto/exec.h>
#include <proto/dos.h>

typedef VOID(*PUTCHARPROC)();

static const ULONG PutChar = 0x16c04e75;
static const ULONG LenChar = 0x52934e75;
static char BufChar[1024] = { 0 };

namespace apk {

    void printf(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
		PutStr(BufChar);
    }

    void debug(int l, const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[D] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void debug(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[D] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void warning(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[W] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void error(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[E] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void doAssert(const char* file, int line) {
        static char tmp[200];
        sprintf_s(tmp, sizeof(tmp), "Program asserted at\n%s:%ld", file, line);
        requester_okay("Assert!", tmp);
        while(1);
    }



}