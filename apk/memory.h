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

namespace apk {

    void* malloc(APK_SIZE_TYPE length);
    void* malloc_aligned(APK_SIZE_TYPE length);
    void free(void* mem);
    void free_aligned(void* mem);

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length);
    void memcpy_aligned(void* dst, const void* src, APK_SIZE_TYPE length);

    void memset(void* dst, int val, APK_SIZE_TYPE length);
    void memset_aligned(void* dst, uint32 val, APK_SIZE_TYPE length);

    class MemoryBuffer {
        byte* m_data;
        uint32 m_size, m_pos;

    public:

        MemoryBuffer() {
            m_data = NULL;
            m_size = 0;
            m_pos = 0;
        }

        MemoryBuffer(byte* data, uint32 size) {
            m_data = data;
            m_size = size;
            m_pos = 0;
        }

        ~MemoryBuffer() {
            m_data = NULL;
            m_size = 0;
            m_pos = 0;
        }

        byte* getData() const {
            return m_data;
        }

        bool isEnd() const {
            return m_pos >= m_size;
        }

        void pos(uint32 pos) {
            assert(pos <= m_size);
            m_pos = pos;
        }

        void seek(int32 offset, int from) {
            assert(m_pos <= m_size);
            switch(from) {
                case kSEEK_CUR:
                    m_pos += offset;
                break;
                case kSEEK_SET:
                    m_pos = offset;
                break;
                case kSEEK_END:
                    m_pos = m_size + offset;
                break;
            }
            assert(m_pos <= m_size);
        }

        uint32 pos() {
            return m_pos;
        }

        uint32 size() const {
            return m_size;
        }

        uint32 read(void* data, uint32 size) {
            if (size > m_size - m_pos) {
                size = m_size - m_pos;
            }

            memcpy(data, m_data + m_pos, size);

            m_pos += size;
            return size;
        }


    };

}
