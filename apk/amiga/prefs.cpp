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

#include <apk/apk.h>
#include <apk/text.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <workbench/startup.h>

namespace apk { namespace prefs {

	struct DiskObject* sDiskObject = NULL;

    const char* getPrefsString(const char* name, const char* defaultValue) {

        if (sDiskObject == NULL) {
            return defaultValue;
        }

        STRPTR tooltype = (STRPTR) FindToolType((CONST_STRPTR*) sDiskObject->do_ToolTypes, name);

        if (tooltype == NULL) {
            return defaultValue;
        }

        return tooltype;
    }

    uint32 getPrefsNumber(const char* name, uint32 defaultValue) {if (sDiskObject == NULL) {
            return defaultValue;
        }

        STRPTR tooltype = (STRPTR) FindToolType((CONST_STRPTR*) sDiskObject->do_ToolTypes, name);

        if (tooltype == NULL) {
            return defaultValue;
        }

        return string_to_uint32(tooltype);
    }

}}