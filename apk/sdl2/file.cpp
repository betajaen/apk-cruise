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
#include "stdio.h"

namespace apk {

    class FileImpl {
    public:
        FILE* fh;
        uint32 size;
    };

    File::File() {
        m_impl = NULL;
    }

    File::~File() {
        close();
    }

    bool File::close() {
        if (m_impl) {
            fclose(m_impl->fh);
            delete m_impl;
            m_impl = NULL;

            printf("Closed file\n");

            return true;
        }
        return false;
    }

    bool File::isOpen() const {
        return m_impl;
    }

    bool File::open(const char* path) {
        char diskPath[256] = { 0 };

        close();

        sprintf_s(diskPath, sizeof(diskPath), "data/dos/%s", path);

        FILE* fh = fopen(diskPath, "r");

        if (fh == NULL) {
            printf("Could not open %s\n", diskPath);
            return false;
        }

        m_impl = new FileImpl();
        m_impl->fh = fh;

        fseek(m_impl->fh, 0, SEEK_END);
        m_impl->size = ftell(m_impl->fh);
        fseek(m_impl->fh, 0, SEEK_SET);

        printf("Opened %s\n", diskPath);

        return true;
    }

    uint32 File::size() const {
        if (m_impl) {
            return m_impl->size;
        }
        return 0;
    }

    bool File::exists(const char* path) {
        char diskPath[256] = { 0 };

        sprintf_s(diskPath, sizeof(diskPath), "data/dos/%s", path);
        FILE* fh = fopen(diskPath, "r");

        if (fh == NULL) {
            printf("DID NOT OPEN %s\n", diskPath);
            return false;
        }

        printf("Opened %s\n", diskPath);

        fclose(fh);
        return true;
    }


    bool File::seek(int32 where, int32 mode) {
        assert(m_impl);

        switch(mode) {
            default:
                return false;
            case kSEEK_SET:
                return fseek(m_impl->fh, where, SEEK_SET) == 0;
            case kSEEK_CUR:
                return fseek(m_impl->fh, where, SEEK_CUR) == 0;
            case kSEEK_END:
                return fseek(m_impl->fh, where, SEEK_END) == 0;
        }
    }

    uint32 File::read(void* data, uint32 size) {
        assert(m_impl);
        uint32 rv = fread(data, size, 1, m_impl->fh);
        return rv;
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