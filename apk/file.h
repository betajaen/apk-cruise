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
#include "consts.h"

namespace apk { namespace fs {
    
    void setProgramDir(const char* path);
    
    const char* getProgramDir();

}}

namespace apk { namespace path {

    enum class PathType {
        None,
        File,
        DrawerVolume
    };

    PathType test(const char* path);

}}

namespace apk {


    class FileImpl;

    class ReadFile {

        FileImpl* m_impl;

    public:

        ReadFile();
        ~ReadFile();

        bool open(const char*);

        bool close();

        bool isOpen() const;

        uint32 size() const;

        bool seek(int32 where, int32 mode = kSEEK_SET);

        uint32 read(void* data, uint32 size);

        int16 readSint16BE();

        int32 readSint32BE();

        uint16 readUint16BE();

        uint32 readUint32BE();

        static bool exists(const char*);

    };


    class AppendFile {

        FileImpl* m_impl;

    public:

        AppendFile();
        ~AppendFile();

        bool open(const char*);

        bool close();

        bool isOpen() const;

        uint32 write(void* data, uint32 size);

    };



}
