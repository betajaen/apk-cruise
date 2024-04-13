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

#if 1

    struct Header;
    struct Footer;

    struct Header {
        uint32  magic;
        uint32  size;
        const char* comment;
        Footer* footer;
    };

    struct Footer {
        Header* header;
    };

    static const uint32 kMagic = 'A' << 24 | 'P' << 16 | 'K' << 8 | 'A';

    static void* _allocMem(uint32 size, const char* comment) {
        uint32 totalSize = sizeof(Header) + sizeof(Footer) + size + (sizeof(void*) == 8 ? (size & 7) : (size & 3));
        uint8* bytes = (uint8*) ::malloc(totalSize);
        assert(bytes);
        ::memset(bytes, 0, totalSize);
        Header* header = (Header*) bytes;
        header->magic = kMagic;
        header->comment = comment;
        header->size = totalSize;
        header->footer = (Footer*) (bytes + sizeof(Header) + size);
        header->footer->header = header;
        /// ::printf("[MEM] Allocate %s (%ld)\n", comment, totalSize);
        return (void*) (bytes + sizeof(Header));
    }

    static void _freeMem(void* mem, const char* comment) {
        // ::printf("[MEM] Free from %s (%p)\n", comment);
        assert(mem);
        Header* header = (((Header*) mem) - 1);
        // ::printf("[MEM] Free %s (%ld)\n", header->comment, header->size);
        assert(header->magic == kMagic);
        assert(header->footer->header == header);
        byte* b = (byte*) header;
        memset(b, 0xCD, header->size);
        ::free(header);
    }

#else

    void* _allocMem(uint32 size, const char* comment) {
        uint8* bytes = (uint8*) ::malloc(size);
        ::memset(bytes, 0, size);
        return (void*) bytes;
    }

    void _freeMem(void* mem, const char* comment) {
        ::free(mem);
    }
#endif

    void* _apk_allocate(APK_SIZE_TYPE size, const char* comment) {
        return _allocMem(size, comment);
    }

    void _apk_deallocate(void* mem, const char* comment) {
        if (mem) {
            _freeMem(mem,  comment);
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


    uint32 string_to_uint32(const char* str) {
        if (str == NULL) {
            return 0;
        }

        if ((str == NULL) || (str[0] == '\0') || (str[0] == '0' && str[1] == '\0')) {
            return 0;
        }

        if (str[0] == '1' && str[1] == '\0') {
            return 1;
        }
        
        uint32 rv = 0;
        while((*str != '\0') && (*str >= '0' && *str <= '9')) {
            rv = rv * 10 + (*str - '0');
            str++;
        }

        return rv;
    }
}
