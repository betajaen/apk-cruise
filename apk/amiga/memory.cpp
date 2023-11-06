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

typedef VOID(*PUTCHARPROC)();
static const ULONG PutChar = 0x16c04e75;
static const ULONG LenChar = 0x52934e75;

namespace apk {

    void* malloc(APK_SIZE_TYPE size) {
        return AllocVec(size, MEMF_CLEAR);
    }

    void free(void* mem) {
        if (mem != NULL) {
            FreeVec(mem);
        }
    }

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length) {
        CopyMem(src, dst, length);
    }

    void memset(void* dst, int val, APK_SIZE_TYPE length) {
        if (dst == NULL) {
            return;
        }

        BYTE* d = (BYTE*) dst;
        BYTE v = (BYTE) val;
        for(APK_SIZE_TYPE i=0;i < length;i++) {
            *d++ = v;
        }
    }

    const char* strchr(const char* str, char c) {
    }

    const char* strrchr(const char* str, char c) {
        return NULL;
    }

    char* strchr(char* str, char c) {
        return NULL;
    }

    char* strrchr(char* str, char c) {
        return NULL;
    }

    int strcmp(const char* lhs, const char* rhs) {
        return 0;
    }

    uint32 strlen(const char* str) {
        return 0;
    }

    void strcpy(char* dst, const char* src) {
    }

    void strlcpy(char* dst, const char* src, uint32 length) {
    }

    const char* strlcat(char* dst, const char* src, APK_SIZE_TYPE size) {
        return NULL;
    }

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...) {

		uint32 length = 0;
		const char* arg = (const char*)(&fmt + 1);

		RawDoFmt((CONST_STRPTR)fmt, (APTR)arg, (PUTCHARPROC)&LenChar, &length);

        if (length == 0 || length > dst_length) {
            *dst = '\0';
            return;
		}

        RawDoFmt((CONST_STRPTR)fmt, (APTR)arg, (PUTCHARPROC)&PutChar, dst);
    }

    char toupper(char c) {
        return c;
    }

}