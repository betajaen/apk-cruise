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

    void* malloc_aligned(APK_SIZE_TYPE size) {
        return AllocVec(size, MEMF_CLEAR); // AllocVec is always longword aligned.
    }

    void free(void* mem) {
        if (mem != NULL) {
            FreeVec(mem);
        }
    }

    void free_aligned(void* mem) {
        if (mem != NULL) {
            FreeVec(mem);  // AllocVec is always longword aligned.
        }
    }

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length) {
        CopyMem(src, dst, length);
    }

    void memcpy_aligned(void* dst, const void* src, APK_SIZE_TYPE length) {
        CopyMemQuick(src, dst, length);
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

    void memset_aligned(void* dst, int val, APK_SIZE_TYPE length) {
        uint32* d = (uint32*) dst;
        length >>= 2;
        while(length--) {
            *d++ = val;
        }
    }

    const char* strchr(const char* str, char c) {
        while(*str != '\0') {
            if (*str == c) {
                return str;
            }
            str++;
        }
        return NULL;
    }

    const char* strrchr(const char* str, char c) {
        debug("strrchr not implemented");
        return NULL;
    }

    char* strchr(char* str, char c) {
        while(*str != '\0') {
            if (*str == c) {
                return str;
            }
            str++;
        }
        return NULL;
    }

    char* strrchr(char* str, char c) {
        debug("strrchr not implemented");
        return NULL;
    }

    int strcmp(const char* lhs, const char* rhs) {

        const unsigned char *p1 = ( const unsigned char * ) lhs;
        const unsigned char *p2 = ( const unsigned char * ) rhs;

        while ( *p1 && *p1 == *p2 ) ++p1, ++p2;

        return ( *p1 > *p2 ) - ( *p2  > *p1 );
    }

    uint32 strlen(const char* str) {
        uint32 length = 0;
        while(*str++ != '\0') {
            length++;
        }
        return length;
    }

    void strcpy(char* dst, const char* src) {
        while(*src != '\0') {
            *dst++ = *src++;
        }
        *dst = '\0';
    }

    void strlcpy(char* dst, const char* src, uint32 length) {
        uint32 src_length = strlen(src);
        uint32 amount_to_copy = length < src_length ? length : src_length;
        while(amount_to_copy > 0) {
            *dst++ = *src++;
            amount_to_copy--;
        }
        *dst = '\0';
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
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        return c;
    }

}