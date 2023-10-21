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

namespace apk::endian {

    constexpr size_t Little = 0;
    constexpr size_t Big = 1;
    
#if defined(__AMIGA__)
    constexpr size_t Native = Big;
#else
    constexpr size_t Native = Little;
#endif

    template<typename T>
    struct unaligned_pod {
        T val;
    } __attribute__((__packed__, __may_alias__));
    
    template<typename T, size_t EFrom, size_t ETo>
    inline T endian_swap(const T& value) {
        if constexpr (EFrom == ETo) {
            return value;
        }
        else {
            if constexpr (sizeof(T) == 4) {
                return (T) __builtin_bswap32(value);
            }
            else if constexpr (sizeof(T) == 2) {
                return (T) __builtin_bswap16(value);
            }
            else {
                static_assert((sizeof(T) == 2 || sizeof(T) == 4), "Unsupported type!");
            }
        }
    }


    template<typename T, size_t ESourceEndian, size_t EDestEndian = Native>
    inline static T peek(const void* mem) {
        const endian::unaligned_pod<T>* v = (const endian::unaligned_pod<T>*) mem;
        return endian_swap<T, ESourceEndian, EDestEndian>(v->val);
    }

    template<typename T, size_t ESourceEndian, size_t EDestEndian = Native>
    inline static void poke(void* mem, T value) {
        endian::unaligned_pod<T>* v = (endian::unaligned_pod<T>*) mem;
        v->val = endian_swap<T, ESourceEndian, EDestEndian>(value);
    }

    template<typename T, size_t ESourceEndian, size_t EDestEndian = Native>
    inline static T pod(const T& value) {
        return endian_swap<T, ESourceEndian, EDestEndian>(value);
    }

    template<typename T, size_t ESourceEndian, size_t EDestEndian = Native>
    inline static void array(T* array, uint32 length) {
        for(uint32 i=0;i < length;i++) {
            *array = endian_swap<T, ESourceEndian, EDestEndian>(*array);
            array++;
        }
    }

}
