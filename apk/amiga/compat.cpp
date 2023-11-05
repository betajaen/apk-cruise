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
    void* malloc(APK_SIZE_TYPE size);
    void free(void* mem);
}

void* operator new(APK_SIZE_TYPE, void* p)
{
    return p;
}

void* operator new(APK_SIZE_TYPE n)
{
    return apk::malloc(n);
}

void operator delete(void* p)
{
    return apk::free(p);
}

void operator delete(void* p, APK_SIZE_TYPE n) {
    return apk::free(p);
}