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

    inline uint32 mul_320(uint32 x) {
        return (x << 8) + (x << 6);
    }

    template<typename T>
    inline T MIN(const T& lhs, const T& rhs) {
        return (lhs < rhs ? lhs : rhs);
    }

    template<typename T1, typename T2>
    inline T1 MIN(const T1& lhs, const T2& rhs) {
        return (lhs < rhs ? lhs : rhs);
    }

    template<typename T>
    inline T ABS(T v) {
        return v < 0 ? -v : v;
    }

    template<typename T>
    inline T MAX(T lhs, T rhs) {
        return (lhs > rhs) ? lhs : rhs;
    }

    template<typename T>
    inline T CLIP(T x, T min, T max) {
        if (x < min)
            x = min;
        else if (x > max)
            x = max;
        return x;
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



}
