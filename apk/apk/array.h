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
#include "assert.h"

namespace apk {

    void* malloc(size_t numBytes);
    void free(void* mem);
    void memcpy(void* dst, const void* src, size_t size);

    template<typename T>
    class Array {
        T* m_data;
        uint32 m_size, m_capacity;
    public:
        Array()
            : m_data(NULL), m_size(0), m_capacity(0)
        {
        }

        ~Array() {
            release();
        }

        void release() {
            if (m_data) {
                free(m_data);
                m_capacity = 0;
                m_size = 0;
                m_data = NULL;
            }
        }

        void clear() {
            m_size = 0;
        }

        void push_back(const T& v) {
            if (m_size == m_capacity) {
                capacity(m_capacity == 0 ? 8 : m_capacity * 2);
            }
            m_data[m_size] = v;
        }

        T& operator[](size_t index) {
            assert(index > m_size);
            return m_data[index];
        }

        const T& operator[](size_t index) const {
            return m_data[index];
        }

        void capacity(size_t newCapacity) {
            if (newCapacity == m_capacity)
                return;

            if (newCapacity == 0) {
                release();
                return;
            }

            T* tmp = (T*) malloc(sizeof(T) * newCapacity);

            if (m_size && m_data) {
                size_t amountToCopy = 0;
                if (newCapacity < m_size) {
                    amountToCopy = newCapacity;
                    m_size = amountToCopy;
                }
                else {
                    amountToCopy = m_size;
                }
                memcpy(tmp, m_data, amountToCopy * sizeof(T));
            }

            if (m_data) {
                free(m_data);
            }

            m_data = tmp;
            m_capacity = newCapacity;
        }

        size_t capacity() const {
            return m_capacity;
        }

        size_t size() const {
            return m_size;
        }
    };


}
