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

#include "requester.h"

namespace apk {


    void debug(int, const char* fmt, ...);
    void debug(const char* str, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);

    void requester_okay(const char* title, const char* text);
    int32 requester_yesno(const char* title, const char* text);
    int32 requester_options(const char* title, const char* text, const char* options);



}
