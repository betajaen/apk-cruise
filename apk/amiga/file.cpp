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
#include "apk/file.h"

namespace apk {

    class FileImpl {
    public:
        ULONG fh;
    };

    File::File() {
        m_impl = NULL;
    }

    File::~File() {
        close();
    }

    bool File::close() {
        return false;
    }

    bool File::isOpen() const {
        return false;
    }

    bool File::open(const char* path) {
        return false;
    }

    uint32 File::size() const {
        return 0;
    }

    bool File::exists(const char* path) {
        return false;
    }


    bool File::seek(int32 where, int32 mode) {
        return false;
    }

    uint32 File::read(void* data, uint32 size) {
        return 0;
    }

    int16 File::readSint16BE() {
        return 0;
    }

    int32 File::readSint32BE() {
        return 0;
    }

    uint16 File::readUint16BE() {
        return 0;
    }

    uint32 File::readUint32BE() {
        return 0;
    }

}