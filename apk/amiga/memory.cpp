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

namespace apk {

    void* malloc(size_t size) {
        return NULL;
    }

    void free(void* mem) {
    }

    void memcpy(void* dst, const void* src, size_t length) {
    }

    void memset(void* dst, int val, size_t length) {
    }

    const char* strchr(const char* str, char c) {
    }

    const char* strrchr(const char* str, char c) {
        return NULL;
    }

    char* strchr(char* str, char c) {
        return NULL;
    }

    char* strrchr(char* str, char c) {
        return NULL;
    }

    int strcmp(const char* lhs, const char* rhs) {
        return 0;
    }

    uint32 strlen(const char* str) {
        return 0;
    }

    void strcpy(char* dst, const char* src) {
    }

    void strlcpy(char* dst, const char* src, uint32 length) {
    }

    const char* strlcat(char* dst, const char* src, size_t size) {
        return NULL;
    }

    void sprintf_s(char* dst, size_t dst_length, const char* fmt, ...) {
    }

    char toupper(char c) {
        return c;
    }

}