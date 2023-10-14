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


#include <stdio.h>
#include <SDL2/SDL.h>
#include "apk/types.h"

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    printf("initialization successful!\n");

    SDL_Quit();
}


namespace apk {

    void* malloc(size_t size) {
        return SDL_malloc(size);
    }

    void free(void* mem) {
        return SDL_free(mem);
    }

    void memcpy(void* dst, const void* src, size_t length) {
        SDL_memcpy(dst, src, length);
    }

    void memset(void* dst, int val, size_t length) {
        SDL_memset(dst, val, length);
    }

    const char* strchr(const char* str, char c) {
        return SDL_strchr(str, c);
    }

    const char* strrchr(const char* str, char c) {
        return SDL_strrchr(str, c);
    }

    char* strchr(char* str, char c) {
        return SDL_strchr(str, c);
    }

    char* strrchr(char* str, char c) {
        return SDL_strrchr(str, c);
    }

    int strcmp(const char* lhs, const char* rhs) {
        return SDL_strcmp(lhs, rhs);
    }

    uint32 strlen(const char* str) {
        return SDL_strlen(str);
    }

    void strcpy(char* dst, const char* src) {
        SDL_strlcpy(dst, src, 0xFFFFFFFF);
    }

    void strlcpy(char* dst, const char* src, uint32 length) {
        SDL_strlcpy(dst, src, length);
    }

    const char* strlcat(char* dst, const char* src, size_t size) {
        return ::strcat(dst, src);
    }

    void sprintf_s(char* dst, size_t dst_length, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        SDL_vsnprintf(dst, dst_length, fmt, args);
        va_end(args);
    }

    char toupper(char c) {
        return SDL_toupper(c);
    }

}