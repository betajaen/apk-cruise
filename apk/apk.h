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
#include "time.h"
#include "debug.h"

namespace apk {


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

    bool isQuitRequested();



}


#define APK_ALIGNED /* aligned */

#define ARRAYSIZE(X) ((sizeof(X)) / (sizeof(X[0])))