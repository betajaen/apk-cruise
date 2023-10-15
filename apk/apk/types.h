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
#include "array.h"
#include "list.h"
#include "assert.h"
#include "file.h"
#include "endian.h"

namespace apk {


    class String {
        char* mem;
        int32 length;
    public:
        String() : mem(NULL), length(0) {}

        String(const char* c_str) {
        }

        String(const char* c_str, int32 length) {
        }

        const char* c_str() const {
            return mem;
        }
    };

    struct Point {
        int16 x, y;

        Point() : x(0), y(0) {}
        Point(int16 X, int16 Y) : x(X), y(Y) {}
    };

    struct Rect {
        int16 top, left, right, bottom;

        Rect() : top(0), left(0), right(0), bottom(0) {}


        Rect(int16 L, int16 T, int16 R, int16 H) : top(L), left(T), right(R), bottom(H) {}
        Rect(int16 R, int16 B) : top(0), left(0), right(R), bottom(B) {}

        bool contains(int16 x, int16 y) const {
            return false;
        }

        bool isEmpty() const {
            return false;
        }

        bool intersects(Rect& rect) {
            return false;
        }

        int16 width() const {
            return right - left;
        }

        int16 height() const {
            return bottom - top;
        }
    };


    int strcmp(const char* lhs, const char* rhs);
    void memcpy(void* dst, const void* src, size_t length);
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

    const char* strlcat(char* str, const char* str2, size_t size);

    inline const char* strcat(char* str, const char* str2) {
        return strlcat(str, str2, 0xFFFFffff);
    }

    inline const char* strcat_s(char* str, const char* str2) {
        return strlcat(str, str2, 0xFFFFffff);
    }

    char toupper(char);

    void memset(void* dst, int val, size_t length);
    void sprintf_s(char* dst, size_t dst_length, const char* fmt, ...);
    void printf(const char* fmt, ...);

    void* malloc(size_t length);
    void free(void* mem);

    bool isQuitRequested();


    void debug(int, const char* fmt, ...);
    void debug(const char* str, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);
}



#define ARRAYSIZE(X) ((sizeof(X)) / (sizeof(X[0])))