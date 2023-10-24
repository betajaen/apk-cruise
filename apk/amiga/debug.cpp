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
#include "apk/assert.h"

namespace apk {

    void printf(const char* fmt, ...) {
    }

    void debug(int l, const char* fmt, ...) {
    }

    void debug(const char* fmt, ...) {
    }

    void warning(const char* fmt, ...) {
    }

    void error(const char* fmt, ...) {
    }

    void doAssert(const char* file, int line) {
    }

    bool pollEvents(Event& evt) {
        return false;
    }


}