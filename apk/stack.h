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
    class Stack {
        T m_data[N];
        ssize_t m_top;
    public:
        Stack()
            : m_top(-1)
        {
        }

        ~Stack() {
            release();
        }

        Stack(const Stack& stack) {
            copyFrom(stack);
        }

        Stack(Stack&& stack) {
            moveFrom(stack);
        }

        Stack& operator=(const Stack& stack) {
            if (&stack == this) {
                return *this;
            }
            copyFrom(stack);
            return *this;
        }

        Stack& operator=(Stack&& stack) {
            if (&stack == this) {
                return *this;
            }
            moveFrom(stack);
            return *this;
        }

        bool isFull() const {
            return m_top == N;
        }

        bool isEmpty() const {
            return m_top == -1;
        }

        void copyFrom(const Stack& other) {
            release();
            m_top = other.m_top;
            for(uint32_t i=0;i < m_top;i++) {
                new ((void*) &m_data[i]) T(other[i]);
            }
        }

        void moveFrom(Stack& other) {
            release();
            m_top = other.m_top;
            for(uint32_t i=0;i < m_top;i++) {
                new ((void*) &m_data[i]) T(other.m_data[i]);
            }
            other.release();
        }

        void release() {
            if (m_top >= 0) {
                for(uint32_t i=0; i < m_top;i++) {
                    m_data[i].~T();
                }
                m_top = -1;
            }
        }

        void push_back(const T& v) {
            assert(m_top >= -1 && m_top < N)
            m_top++;
            new ((void*) &m_data[m_top]) T(v);
        }

        void pop_back() {
            assert(m_top >= 0)
            m_data[m_top].~T();
            m_top--;
        }

        T& top() {
            assert(m_top >= 0);
            return m_data[m_top];
        }

        const T& top() const {
            assert(m_top >= 0);
            return m_data[m_top];
        }

    };


}
