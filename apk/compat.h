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

#include "apk/pod.h"

#define _apk_to_str(x) #x
#define apk_to_str(x) _apk_to_str(x)

namespace apk {
    void* _apk_allocate(APK_SIZE_TYPE size, const char* comment);
    void _apk_deallocate(void* mem, const char* comment);

    template<typename T>
    inline void _apk_delete(T*& mem, const char* comment) {
        if (mem) {
            mem->~T();
            _apk_deallocate(mem, comment);
            mem = NULL;
        }
    }
}

void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, void* p);
void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, const char* comment);

#define apk_new new(__FILE__ ":" apk_to_str(__LINE__))
#define apk_delete(MEM) ::apk::_apk_delete(MEM, __FILE__ ":" apk_to_str(__LINE__))
#define apk_allocate(SIZE) ::apk::_apk_allocate(SIZE, __FILE__ ":" apk_to_str(__LINE__))
#define apk_deallocate(MEM) ::apk::_apk_deallocate(MEM, __FILE__ ":" apk_to_str(__LINE__))
