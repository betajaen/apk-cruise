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

#include <proto/dos.h>

namespace apk {

    class FileImpl {
    public:
        ULONG fh;
        ULONG size;
    };

    File::File() {
        m_impl = NULL;
    }

    File::~File() {
        close();
    }

    bool File::close() {
        if (m_impl) {
            Close(m_impl->fh);
            delete m_impl;
            m_impl = NULL;
            return true;
        }
        return false;
    }

    bool File::isOpen() const {
        return m_impl != NULL;
    }

    bool File::open(const char* path) {
        close();

        char diskPath[256] = { 0 };
        sprintf_s(diskPath, sizeof(diskPath), "PROGDIR:data/dos/%s", path);

        ULONG fh = Open(diskPath, MODE_OLDFILE);
        if (fh == 0UL) {
            return false;
        }

        m_impl = new FileImpl();
        m_impl->fh = fh;
        Seek(fh, 0, OFFSET_END);
        m_impl->size = Seek(fh, 0, OFFSET_BEGINNING);

        return true;
    }

    uint32 File::size() const {

        if (m_impl == NULL) {
            return 0;
        }

        return m_impl->size;
    }

    bool File::exists(const char* path) {

        char diskPath[256] = { 0 };
        sprintf_s(diskPath, sizeof(diskPath), "PROGDIR:data/dos/%s", path);

        ULONG fh = Open(diskPath, MODE_OLDFILE);

        if (fh == 0UL) {
            return false;
        }

        Close(fh);

        return true;
    }


    bool File::seek(int32 where, int32 mode) {
        assert(m_impl);

        switch(mode) {
            default:
                return false;
            case kSEEK_SET:
                return Seek(m_impl->fh, where, OFFSET_BEGINNING) != 0;
            case kSEEK_CUR:
                return Seek(m_impl->fh, where, OFFSET_CURRENT) != 0;
            case kSEEK_END:
                return Seek(m_impl->fh, where, OFFSET_END) != 0;
        }
    }

    uint32 File::read(void* data, uint32 size) {
        assert(m_impl);

        return (ULONG) Read(m_impl->fh, data, size);
    }

    int16 File::readSint16BE() {
        int16 value;
        read(&value, sizeof(value));
        return endian::pod<int16, endian::Big>(value);
    }

    int32 File::readSint32BE() {
        int32 value;
        read(&value, sizeof(value));
        return endian::pod<int32, endian::Big>(value);
    }

    uint16 File::readUint16BE() {
        uint16 value;
        read(&value, sizeof(value));
        return endian::pod<uint16, endian::Big>(value);
    }

    uint32 File::readUint32BE() {
        uint32 value;
        read(&value, sizeof(value));
        return endian::pod<uint32, endian::Big>(value);
    }

}