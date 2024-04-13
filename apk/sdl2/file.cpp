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
#include "apk/file.h"
#include "stdio.h"

#include <sys/stat.h>

namespace apk { namespace fs {
    static char s_ProgDir[256] = "PROGDIR:";
}}

namespace apk { namespace path {

    PathType test(const char* path) {
        PathType rv = PathType::None;
        struct stat st;

        if (stat(path, &st) == 0) {
            if (S_ISREG(st.st_mode)) {
                rv = PathType::File;
            }
            else if (S_ISDIR(st.st_mode)) {
                rv = PathType::DrawerVolume;
            }
        }

        return rv;
    }

}}

namespace apk { namespace fs {

    void setProgramDir(const char* path) {
        apk::strcpy_s(s_ProgDir, sizeof(s_ProgDir), path);
    }

    const char* getProgramDir() {
        return s_ProgDir;
    }

}}

namespace apk {

    class FileImpl {
    public:
        FILE* fh;
        uint32 size;
    };

    ReadFile::ReadFile() {
        m_impl = NULL;
    }

    ReadFile::~ReadFile() {
        close();
    }

    bool ReadFile::close() {
        if (m_impl) {
            fclose(m_impl->fh);
            apk_delete(m_impl);
            m_impl = NULL;

            return true;
        }
        return false;
    }

    bool ReadFile::isOpen() const {
        return m_impl;
    }

    bool ReadFile::open(const char* path) {
        char diskPath[256] = { 0 };

        close();

        sprintf_s(diskPath, sizeof(diskPath), "%s", path);

        FILE* fh = fopen(diskPath, "r");

        if (fh == NULL) {
            error("File '%s' could not be found in path!\n", diskPath);
            return false;
        }

        m_impl = apk_new FileImpl();
        m_impl->fh = fh;

        fseek(m_impl->fh, 0, SEEK_END);
        m_impl->size = ftell(m_impl->fh);
        fseek(m_impl->fh, 0, SEEK_SET);

        return true;
    }

    uint32 ReadFile::size() const {
        if (m_impl) {
            return m_impl->size;
        }
        return 0;
    }

    bool ReadFile::exists(const char* path) {
        char diskPath[256] = { 0 };

        sprintf_s(diskPath, sizeof(diskPath), "%s", path);
        FILE* fh = fopen(diskPath, "r");

        if (fh == NULL) {
            return false;
        }

        fclose(fh);
        return true;
    }


    bool ReadFile::seek(int32 where, int32 mode) {
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

    uint32 ReadFile::read(void* data, uint32 size) {
        assert(m_impl);
        uint32 rv = fread(data, size, 1, m_impl->fh);
        return rv;
    }

    int16 ReadFile::readSint16BE() {
        int16 value;
        read(&value, sizeof(value));
        return endian::pod<int16, endian::Big>(value);
    }

    int32 ReadFile::readSint32BE() {
        int32 value;
        read(&value, sizeof(value));
        return endian::pod<int32, endian::Big>(value);
    }

    uint16 ReadFile::readUint16BE() {
        uint16 value;
        read(&value, sizeof(value));
        return endian::pod<uint16, endian::Big>(value);
    }

    uint32 ReadFile::readUint32BE() {
        uint32 value;
        read(&value, sizeof(value));
        return endian::pod<uint32, endian::Big>(value);
    }


    AppendFile::AppendFile() {
        m_impl = NULL;
    }

    AppendFile::~AppendFile() {
        close();
    }

    bool AppendFile::close() {
        if (m_impl) {
            fclose(m_impl->fh);
            apk_delete(m_impl);
            m_impl = NULL;

            return true;
        }
        return false;
    }

    bool AppendFile::isOpen() const {
        return m_impl;
    }

    bool AppendFile::open(const char* path) {
        char diskPath[256] = { 0 };

        close();

        sprintf_s(diskPath, sizeof(diskPath), "%s", path);

        FILE* fh = fopen(diskPath, "w");

        if (fh == NULL) {
            error("Could not open '%s' for appending!\n", diskPath);
            return false;
        }

        m_impl = apk_new FileImpl();
        m_impl->fh = fh;

        fseek(m_impl->fh, 0, SEEK_END);
        m_impl->size = ftell(m_impl->fh);
        fseek(m_impl->fh, 0, SEEK_SET);

        return true;
    }

    uint32 AppendFile::write(void* data, uint32 size) {
        assert(m_impl);
        uint32 rv = fwrite(data, size, 1, m_impl->fh);
        return rv;
    }


}