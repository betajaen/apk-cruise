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


#if defined (__AMIGA__)

#else

#include <stdint.h>
#include <stddef.h>

namespace apk {
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef uint8_t byte;
    typedef size_t size_t;

    typedef uint32 uint;

}

#endif

#include "list.h"

namespace apk {


    template<typename T>
    class Array {
        T* m_data;
        uint32 m_size, m_capacity;
    public:
        Array() {}
        ~Array() {}

        void push_back(const T&) {
        }

        T& operator[](size_t index) {
            return m_data[index];
        }

        const T& operator[](size_t index) const {
            return m_data[index];
        }

        void clear() {
        }

        size_t size() const {
            return m_size;
        }
    };


    constexpr int32 kSEEK_SET = 0;
    constexpr int32 kSEEK_CUR = 1;
    constexpr int32 kSEEK_END = 2;

    class File {
    public:
        bool open(const char*) {
            return false;
        }

        bool close() {
            return false;
        }

        bool isOpen() const {
            return false;
        }

        size_t size() const {
            return 0;
        }
        void seek(int32 where) {
            seek(where, kSEEK_CUR);
        }

        void seek(int32 where, int32 mode) {
        }

        size_t read(void* data, size_t size) {
            return 0;
        }

        int16 readSint16BE() {
            return 0;
        }

        int32 readSint32BE() {
            return 0;
        }
        uint16 readUint16BE() {
            return 0;
        }

        uint32 readUint32BE() {
            return 0;
        }

        bool exists(const char*) {
            return false;
        }

    };

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
    template<size_t N>
    void sprintf_s(char (&dst)[N], const char* fmt, ...) {
    }

    void* malloc(size_t length);
    void free(void* mem);

}

#define debug(F, ...)
#define warning(F, ...)
#define error(F, ...)
#define assert(C)
#define READ_BE_UINT32(X) (0)
#define READ_BE_UINT16(X) (0)
#define READ_BE_INT32(X) (0)
#define READ_BE_INT16(X) (0)
#define READ_LE_UINT32(X) (0)
#define READ_LE_UINT16(X) (0)
#define READ_LE_INT32(X) (0)
#define READ_LE_INT16(X) (0)
#define READ_UINT16(X) (0)
#define READ_UINT32(X) (0)
#define READ_INT16(X) (0)
#define READ_INT32(X) (0)

#define WRITE_UINT16(INTO, VAL)
#define WRITE_UINT32(INTO, VAL)
#define WRITE_BE_UINT16(INTO, VAL)
#define WRITE_BE_UINT32(INTO, VAL)

#define ARRAYSIZE(X) ((sizeof(X)) / (sizeof(X[0])))