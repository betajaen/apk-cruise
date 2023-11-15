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
#include "compat.h"
#include "consts.h"
#include "array.h"
#include "list.h"
#include "assert.h"
#include "file.h"
#include "endian.h"
#include "memory.h"
#include "input.h"
#include "requester.h"
#include "math.h"
#include "stack.h"
#include "text.h"

namespace apk {

    template<typename T>
    inline T MIN(const T& lhs, const T& rhs) {
        return (lhs < rhs ? lhs : rhs);
    }

    template<typename T1, typename T2>
    inline T1 MIN(const T1& lhs, const T2& rhs) {
        return (lhs < rhs ? lhs : rhs);
    }

    template<typename T>
    T ABS(T v) {
        return v < 0 ? -v : v;
    }

    template<typename T>
    T MAX(T lhs, T rhs) {
        return (lhs > rhs) ? lhs : rhs;
    }

    template<typename T>
    T CLIP(T x, T min, T max) {
        if (x < min)
            x = min;
        else if (x > max)
            x = max;
        return x;
    }

    template<typename T>
    void SWAP(T& l, T& r) {
        T t = l;
        l = r;
        r = t;
    }

    template<typename T>
    void SORT(T& l, T& r) {
        if (l > r) {
            SWAP(l, r);
        }
    }



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
            return (left <= x) && (x < right) && (top <= y) && (y < bottom);
        }

        bool isEmpty() const {
            return (left >= right || top >= bottom);
        }

        bool intersects(Rect& rect) {
            return (left < rect.right) && (rect.left < right) && (top < rect.bottom) && (rect.top < bottom);
        }

        int16 width() const {
            return right - left;
        }

        int16 height() const {
            return bottom - top;
        }
    };


    void printf(const char* fmt, ...);

    bool isQuitRequested();

    void delayMs(uint32 ms);
    uint32 getMs();


}


#define APK_ALIGNED /* aligned */

#define ARRAYSIZE(X) ((sizeof(X)) / (sizeof(X[0])))