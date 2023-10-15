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

namespace apk { namespace endian {

    union bytes_2 {
        uint16 _unsigned;
        int16  _signed;
        byte   _bytes[2];
    };

    union bytes_4 {
        uint32 _unsigned;
        int32  _signed;
        byte   _bytes[4];
    };

    struct bytes_unaligned_unsigned_2 { uint16 val; } __attribute__((__packed__, __may_alias__));
    struct bytes_unaligned_unsigned_4 { uint32 val; } __attribute__((__packed__, __may_alias__));
    struct bytes_unaligned_signed_2 { int16 val; } __attribute__((__packed__, __may_alias__));
    struct bytes_unaligned_signed_4 { int32 val; } __attribute__((__packed__, __may_alias__));

    inline uint16 swap_2(uint16 v) {
        bytes_2 a, b;
        a._unsigned = v;
        b._bytes[1] = a._bytes[0];
        b._bytes[0] = a._bytes[1];
        return b._unsigned;
    }

    inline int16 swap_2(int16 v) {
        bytes_2 a, b;
        a._signed = v;
        b._bytes[1] = a._bytes[0];
        b._bytes[0] = a._bytes[1];
        return b._signed;
    }

    inline uint32 swap_4(uint32 v) {
        bytes_4 a, b;
        a._unsigned = v;
        b._bytes[3] = a._bytes[0];
        b._bytes[2] = a._bytes[1];
        b._bytes[1] = a._bytes[2];
        b._bytes[0] = a._bytes[3];
        return b._unsigned;
    }

    inline int32 swap_4(int32 v) {
        bytes_4 a, b;
        a._unsigned = v;
        b._bytes[3] = a._bytes[0];
        b._bytes[2] = a._bytes[1];
        b._bytes[1] = a._bytes[2];
        b._bytes[0] = a._bytes[3];
        return b._signed;
    }

#if defined(__AMIGA__)
    inline uint16 from_be_uint16(uint16 v) {
        return v;
    }
    inline int16 from_be_int16(int16 v) {
        return v;
    }
    inline uint32 from_be_uint32(uint32 v) {
        return v;
    }
    inline int32 from_be_int32(int32 v) {
        return v;
    }
    inline uint16 from_le_uint16(uint16 v) {
        return swap_2(v);
    }
    inline int16 from_le_int16(int16 v) {
        return swap_2(v);
    }
    inline uint32 from_le_uint32(uint32 v) {
        return swap_4(v);
    }
    inline int32 from_le_int32(int32 v) {
        return swap_4(v);
    }
    inline uint16 to_be_uint16(uint16 v) {
        return v;
    }
    inline int16 to_be_int16(int16 v) {
        return v;
    }
    inline uint32 to_be_uint32(uint32 v) {
        return v;
    }
    inline int32 to_be_int32(int32 v) {
        return v;
    }
    inline uint16 to_le_uint16(uint16 v) {
        return swap_2(v);
    }
    inline int16 to_le_int16(int16 v) {
        return swap_2(v);
    }
    inline uint32 to_le_uint32(uint32 v) {
        return swap_4(v);
    }
    inline int32 to_le_int32(int32 v) {
        return swap_4(v);
    }
#else

    inline uint16 from_be_uint16(uint16 v) {
        return swap_2(v);
    }
    inline int16 from_be_int16(int16 v) {
        return swap_2(v);
    }
    inline uint32 from_be_uint32(uint32 v) {
        return swap_4(v);
    }
    inline int32 from_be_int32(int32 v) {
        return swap_4(v);
    }
    inline uint16 from_le_uint16(uint16 v) {
        return v;
    }
    inline int16 from_le_int16(int16 v) {
        return v;
    }
    inline uint32 from_le_uint32(uint32 v) {
        return v;
    }
    inline int32 from_le_int32(int32 v) {
        return v;
    }
    inline uint16 to_be_uint16(uint16 v) {
        return swap_2(v);
    }
    inline int16 to_be_int16(int16 v) {
        return swap_2(v);
    }
    inline uint32 to_be_uint32(uint32 v) {
        return swap_4(v);
    }
    inline int32 to_be_int32(int32 v) {
        return swap_4(v);
    }
    inline uint16 to_le_uint16(uint16 v) {
        return v;
    }
    inline int16 to_le_int16(int16 v) {
        return v;
    }
    inline uint32 to_le_uint32(uint32 v) {
        return v;
    }
    inline int32 to_le_int32(int32 v) {
        return v;
    }


#endif


    inline uint16 read_uint16(const void* ptr) {
        return ((const bytes_unaligned_unsigned_2*)ptr)->val;
    }
    inline uint32 read_uint32(const void* ptr) {
        return ((const bytes_unaligned_unsigned_4*)ptr)->val;
    }
    inline int16 read_int16(const void* ptr) {
        return ((const bytes_unaligned_signed_2*)ptr)->val;
    }
    inline int32 read_int32(const void* ptr) {
        return ((const bytes_unaligned_signed_4*)ptr)->val;
    }

    inline void write_uint16(const void* ptr, uint16 v) {
        (( bytes_unaligned_unsigned_2*)ptr)->val = v;
    }
    inline void write_uint32(const void* ptr, uint32 v) {
        (( bytes_unaligned_unsigned_4*)ptr)->val = v;
    }
    inline void write_int16(const void* ptr, int16 v) {
        (( bytes_unaligned_signed_2*)ptr)->val = v;
    }
    inline void write_int32(const void* ptr, int32 v) {
        (( bytes_unaligned_signed_4*)ptr)->val = v;
    }

    inline uint16 read_be_uint16(const void* ptr) {
        return from_be_uint16( ((const bytes_unaligned_unsigned_2*)ptr)->val );
    }
    inline uint32 read_be_uint32(const void* ptr) {
        return from_be_uint32(  ((const bytes_unaligned_unsigned_4*)ptr)->val );
    }
    inline int16 read_be_int16(const void* ptr) {
        return from_be_int16( ((const bytes_unaligned_signed_2*)ptr)->val );
    }
    inline int32 read_be_int32(const void* ptr) {
        return from_be_int32( ((const bytes_unaligned_signed_4*)ptr)->val );
    }
    inline void write_be_uint16(const void* ptr, uint16 v) {
        ((bytes_unaligned_unsigned_2*)ptr)->val = to_be_uint16(v);
    }
    inline void write_be_uint32(const void* ptr, uint32 v) {
        ((bytes_unaligned_unsigned_4*)ptr)->val = to_be_uint32(v);
    }
    inline void write_be_int16(const void* ptr, int16 v) {
        ((bytes_unaligned_signed_2*)ptr)->val = to_be_int16(v);
    }
    inline void write_be_int32(const void* ptr, int32 v) {
        ((bytes_unaligned_signed_4*)ptr)->val = to_be_int32(v);
    }
    inline uint16 read_le_uint16(const void* ptr) {
        return from_le_uint16( ((const bytes_unaligned_unsigned_2*)ptr)->val );
    }
    inline uint32 read_le_uint32(const void* ptr) {
        return from_le_uint32(  ((const bytes_unaligned_unsigned_4*)ptr)->val );
    }
    inline int16 read_le_int16(const void* ptr) {
        return from_le_int16( ((const bytes_unaligned_signed_2*)ptr)->val );
    }
    inline int32 read_le_int32(const void* ptr) {
        return from_le_int32( ((const bytes_unaligned_signed_4*)ptr)->val );
    }
    inline void write_le_uint16(const void* ptr, uint16 v) {
        ((bytes_unaligned_unsigned_2*)ptr)->val = to_le_uint16(v);
    }
    inline void write_le_uint32(const void* ptr, uint32 v) {
        ((bytes_unaligned_unsigned_4*)ptr)->val = to_le_uint32(v);
    }
    inline void write_le_int16(const void* ptr, int16 v) {
        ((bytes_unaligned_signed_2*)ptr)->val = to_le_int16(v);
    }
    inline void write_le_int32(const void* ptr, int32 v) {
        ((bytes_unaligned_signed_4*)ptr)->val = to_le_int32(v);
    }

}}


#define READ_BE_UINT32(X) ::apk::endian::read_be_uint32(X)
#define READ_BE_UINT16(X) ::apk::endian::read_be_uint16(X)
#define READ_BE_INT32(X)  ::apk::endian::read_be_int32(X)
#define READ_BE_INT16(X)  ::apk::endian::read_be_int16(X)
#define READ_LE_UINT32(X) ::apk::endian::read_le_uint32(X)
#define READ_LE_UINT16(X) ::apk::endian::read_le_uint16(X)
#define READ_LE_INT32(X)  ::apk::endian::read_le_int32(X)
#define READ_LE_INT16(X)  ::apk::endian::read_le_int16(X)
#define READ_UINT16(X)    ::apk::endian::read_uint16(X)
#define READ_UINT32(X)    ::apk::endian::read_uint32(X)
#define READ_INT16(X)     ::apk::endian::read_int16(X)
#define READ_INT32(X)     ::apk::endian::read_int32(X)
#define WRITE_UINT16(INTO, VAL) ::apk::endian::write_uint16(INTO, VAL)
#define WRITE_UINT32(INTO, VAL) ::apk::endian::write_uint32(INTO, VAL)
#define WRITE_BE_UINT16(INTO, VAL) ::apk::endian::write_be_uint16(INTO, VAL)
#define WRITE_BE_UINT32(INTO, VAL) ::apk::endian::write_be_uint32(INTO, VAL)