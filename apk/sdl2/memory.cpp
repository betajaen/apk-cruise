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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

namespace apk {

    void* malloc(APK_SIZE_TYPE size) {
        void* mem = ::malloc(size);
        if (mem) {
            ::memset(mem, 0, size);
        }
        return mem;
    }

    void* malloc_aligned(APK_SIZE_TYPE size) {
        void* mem = ::malloc(size);
        if (mem) {
            ::memset(mem, 0, size);
        }
        return mem;
    }

    void free(void* mem) {
        if (mem) {
            return ::free(mem);
        }
    }

    void free_aligned(void* mem) {
        if (mem) {
            return ::free(mem);
        }
    }

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length) {
        ::memcpy(dst, src, length);
    }

    void memcpy_aligned(void* dst, const void* src, APK_SIZE_TYPE length) {
        ::memcpy(dst, src, length);
    }

    void memset(void* dst, uint32 val, APK_SIZE_TYPE length) {
        ::memset(dst, val, length);
    }

    void memset_aligned(void* dst, uint32 val, APK_SIZE_TYPE length) {
        ::memset(dst, val, length);
    }

    const char* strchr(const char* str, char c) {
        return ::strchr(str, c);
    }

    const char* strrchr(const char* str, char c) {
        return ::strrchr(str, c);
    }

    char* strchr(char* str, char c) {
        return ::strchr(str, c);
    }

    char* strrchr(char* str, char c) {
        return ::strrchr(str, c);
    }

    int strcmp(const char* lhs, const char* rhs) {
        return ::strcmp(lhs, rhs);
    }

    uint32 strlen(const char* str) {
        return ::strlen(str);
    }

    void strcpy(char* dst, const char* src) {
        ::strlcpy(dst, src, 0xFFFFFFFF);
    }

    void strlcpy(char* dst, const char* src, uint32 length) {
        ::strlcpy(dst, src, length);
    }

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::vsnprintf(dst, dst_length, fmt, args);
        va_end(args);
    }

    char toupper(char c) {
        return ::toupper(c);
    }

    bool string_startswith(const char* str, const char* prefix) {
        APK_SIZE_TYPE strLen = strlen(str);
        APK_SIZE_TYPE prefixLen = strlen(prefix);

        if (prefixLen > strLen) {
            return false;
        }

        return strncmp(str, prefix, prefixLen) == 0;
    }

    bool string_endswith(const char* str, const char* suffix) {
        APK_SIZE_TYPE strLen = strlen(str);
        APK_SIZE_TYPE suffixLen = strlen(suffix);

        if (suffixLen > strLen) {
            return false;
        }

        return strncmp(str + (strLen - suffixLen), suffix, suffixLen) == 0;
    }
}
