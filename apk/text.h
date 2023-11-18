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

#pragma once

#include "pod.h"

namespace apk {


    int strcmp(const char* lhs, const char* rhs);


    uint32 strlen(const char* str);
    void strcpy( char* dst, const char* src);
    void strlcpy(char* dst, const char* src, uint32 length);

    inline void strcpy_s(char* dst, uint32 length, const char* src) {
        strlcpy(dst, src, length);
    }

    template<uint32 N>
    inline void strcpy_s(char (&dst)[N], const char *src) {
        strlcpy(dst, src, N);
    }

    inline void strncpy(char* dst, const char* src, uint32 length) {
        strlcpy(dst, src, length);
    }

    const char* strchr(const char* str, char search);
    const char* strrchr(const char* str, char search);

    char* strchr(char* str, char search);
    char* strrchr(char* str, char search);

    char toupper(char);

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...);

    bool string_startswith(const char* str, const char* suffix);
    bool string_endswith(const char* str, const char* suffix);
}
