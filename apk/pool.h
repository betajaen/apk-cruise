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
#include "compat.h"

namespace apk {

    template<typename T, ssize_t N>
    class Pool {
        T m_data[N];
    public:
        Pool()
        {
            for(uint32 i=0;i < N;i++) {
                m_data[i].pool_index = -1;
            }
        }

        ~Pool() {
            release();
        }

        Pool(const Pool& pool) {
            copyFrom(pool);
        }

        Pool(Pool&& pool) {
            moveFrom(pool);
        }

        Pool& operator=(const Pool& pool) {
            if (&pool == this) {
                return *this;
            }
            copyFrom(pool);
            return *this;
        }

        Pool& operator=(Pool&& pool) {
            if (&pool == this) {
                return *this;
            }
            moveFrom(pool);
            return *this;
        }

        void copyFrom(const Pool& other) {
            release();
            for(uint32_t i=0;i < N;i++) {
                new ((void*) &m_data[i]) T(other[i]);
            }
        }

        void moveFrom(Pool& other) {
            release();
            for(uint32_t i=0;i < N;i++) {
                new ((void*) &m_data[i]) T(other.m_data[i]);
            }
            other.release();
        }

        void release() {
            for(uint32_t i=0; i < N;i++) {
                m_data[i].~T();
            }
        }

        T* create() {
            for(uint32 i=0; i < N;i++) {
                T* item = &m_data[i];
                if (item->pool_index == -1) {
                    item->pool_index = i;
                    return item;
                }
            }

            T* item = apk_new T();
            item->pool_index = -2;
            return item;
        }

        void destroy(T* item) {
            if (item->pool_index == -2) {
                apk_delete(item);
                return;
            }

            apk::memset(item, 0, sizeof(T));
            item->pool_index = -1;
        }
        
        void destroyQuick(T* item) {
            if (item->pool_index == -2) {
                apk_delete(item);
                return;
            }

            item->pool_index = -1;
        }


    };


}
