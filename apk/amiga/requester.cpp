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

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>

#include <dos/dos.h>
#include <workbench/workbench.h>
#include <inline/exec.h>
#include <inline/dos.h>

namespace apk {

    void requester_okay(const char* title, const char* text) {
        EasyStruct str;
        str.es_StructSize = sizeof(EasyStruct);
        str.es_Flags = 0;
        str.es_GadgetFormat = (CONST_STRPTR)"OK";
        str.es_TextFormat = (CONST_STRPTR)text;
        str.es_Title = (CONST_STRPTR)title;

        EasyRequest(NULL, &str, NULL);
    }

    int32 requester_yesno(const char* title, const char* text) {
        EasyStruct str;
        str.es_StructSize = sizeof(EasyStruct);
        str.es_Flags = 0;
        str.es_GadgetFormat = (CONST_STRPTR)"YES|NO";
        str.es_TextFormat = (CONST_STRPTR)text;
        str.es_Title = (CONST_STRPTR)title;

        int rv = EasyRequest(NULL, &str, NULL);

        return rv;
    }

    int32 requester_options(const char* title, const char* text, const char* options) {
        EasyStruct str;
        str.es_StructSize = sizeof(EasyStruct);
        str.es_Flags = 0;
        str.es_GadgetFormat = (CONST_STRPTR)options;
        str.es_TextFormat = (CONST_STRPTR)text;
        str.es_Title = (CONST_STRPTR)title;

        int rv = EasyRequest(NULL, &str, NULL);

        return rv;
    }

}