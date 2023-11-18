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
#include "apk/requester.h"
#include "apk/sdl2/ext/tinyfiledialogs.h"
#include <SDL2/SDL.h>

namespace apk {

    static char tempPath[255];

    const char* requester_load(const char* title, const char* drawer, const char* extension) {

        char pattern[24];

        if (extension != NULL) {
            sprintf_s(pattern, sizeof(pattern), "*%s", extension);
        }

        if (string_startswith(drawer, "PROGDIR:")) {
            sprintf_s(tempPath, sizeof(tempPath), "%s%s", SDL_GetBasePath(), drawer + strlen("PROGDIR:"));
        }
        else {
            strcpy_s(tempPath, sizeof(tempPath), drawer);
        }

        return tinyfd_openFileDialog(
            title,
            drawer,
            0,
            NULL,
            NULL,
            0
            );
    }

    const char* requester_save(const char* title, const char* drawer, const char* extension) {

        char pattern[24];

        if (extension != NULL) {
            sprintf_s(pattern, sizeof(pattern), "*%s", extension);
        }

        if (string_startswith(drawer, "PROGDIR:")) {
            strcpy_s(tempPath, sizeof(tempPath), drawer + strlen("PROGDIR:"));
        }
        else {
            strcpy_s(tempPath, sizeof(tempPath), drawer);
        }

        return tinyfd_saveFileDialog(
            title,
            drawer,
            0,
            NULL,
            NULL
            );

    }

    void requester_okay(const char* title, const char* text) {
        int32 rv = tinyfd_messageBox(
            title,
            text,
            "ok",
            "info",
            0
        );
    }

    int32 requester_yesno(const char* title, const char* text) {

        int32 rv = tinyfd_messageBox(
            title,
            text,
            "yesno",
            "info",
            0
        );

        return rv;
    }

}