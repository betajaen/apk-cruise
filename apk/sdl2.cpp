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
#include "apk/gfx.h"

namespace apk {

    constexpr int32 kScreenScale = 3;
    static bool s_FastMode = true;
    static uint32 s_FastModeTime = 0;
    static Array<Event> s_Event;

    namespace gfx {
        SDL_Window* s_screen = NULL;
        SDL_Surface* s_screenSurface = NULL;
        byte* s_virtualSurface = NULL;
        SDL_Color s_virtualPalette[256] = { 0 };
        bool s_virtualPaletteDirty = false;
        uint32 s_width = 0, s_height = 0, s_widthHeight = 0;
    }

    bool s_quitRequested = false;

    void gameMain();

    bool isQuitRequested() {
        return s_quitRequested;
    }

    void delayMs(uint32 ms) {
        if (s_FastMode) {
            s_FastModeTime += ms;
        }
        else {
            SDL_Delay(ms);
        }
    }

    uint32 getMs() {
        if (s_FastMode) {
            return s_FastModeTime;
        }
        else {
            return SDL_GetTicks();
        }
    }
}

int main(void)
{
    SDL_assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

    apk::s_quitRequested = false;
    apk::gameMain();
    apk::gfx::destroyScreen();

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

    void printf(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void debug(int l, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[DBG %d] ", l);
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void debug(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[DBG] ");
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void warning(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[WRN] ");
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void error(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[ERR] ");
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    char toupper(char c) {
        return SDL_toupper(c);
    }

    void doAssert(const char* file, int line) {
        ::printf("\n[ASSERT!] %s:%d\n", file, line);
        SDL_assert(false);
    }

    bool pollEvents(Event& evt) {
        if (s_Event.size() == 0) {
            return false;
        }
        evt = s_Event[s_Event.size()-1];
        s_Event.pop_back();
        return true;
    }
}

namespace apk { namespace gfx {

    void createScreen(uint16 width, uint16 height, uint8 depth) {
        SDL_assert(s_screen == NULL);

        s_screen = SDL_CreateWindow("APK",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width * kScreenScale,
            height * kScreenScale,
            SDL_WINDOW_SHOWN);

        SDL_assert(s_screen);


        s_screenSurface =SDL_GetWindowSurface(s_screen);

        s_width = width;
        s_height = height;
        s_widthHeight = width * height;

        s_virtualSurface = (byte*) malloc(s_widthHeight);

        for(int32 i=1;i < 256;i++) {
            s_virtualPalette[i].r = 255 - i;
            s_virtualPalette[i].g = 255 - i;
            s_virtualPalette[i].b = 255 - i;
            s_virtualPalette[i].a = 255 - i;
        }

        memset(s_virtualSurface, 0, s_widthHeight);
    }

    void destroyScreen() {
        SDL_assert(s_virtualSurface);
        free(s_virtualSurface);
        s_virtualSurface = NULL;

        SDL_assert(s_screen);
        SDL_DestroyWindow(s_screen);
        s_screen = NULL;
        s_width = 0;
        s_height = 0;
    }

    static void scaleCopy(SDL_Surface* dst, byte* src, uint32 scale, uint32 w, uint32 h) {
;
        SDL_LockSurface(dst);

        uint8* s = src;
        uint8* d = (uint8*)dst->pixels;
        const size_t stride =  w * 4 * scale;
        uint8 line[stride];

        for(uint32 y=0;y < h;y++) {

            uint8* l = s;
            uint8* t = line;

            for (uint32 x=0;x < w;x++) {
                uint8 idx = *l;
                SDL_Color col = s_virtualPalette[idx];

                for (uint32 j=0;j < scale;j++) {
                    *t++ = col.b;
                    *t++ = col.g;
                    *t++ = col.r;
                    *t++ = 0xFF;
                }
                l++;
            }

            for (uint32 j=0;j < scale;j++) {
                memcpy(d, line, sizeof(line));
                d += sizeof(line);
            }

            s += w;
        }

        SDL_UnlockSurface(dst);
    }


    void flipScreen() {
        scaleCopy(s_screenSurface, s_virtualSurface, kScreenScale, s_width, s_height);
        SDL_UpdateWindowSurface(s_screen);

        SDL_Event evt;
        while(SDL_PollEvent(&evt)) {
            switch(evt.type) {
                case SDL_QUIT: {
                    s_quitRequested = true;
                    Event evt;
                    evt.type = EVENT_QUIT;
                    s_Event.push_back(evt);
                }
                break;
                case SDL_MOUSEMOTION:
                {
                    int32 mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    mouseX /= kScreenScale;
                    mouseY /= kScreenScale;
                    Event evt;
                    evt.type = EVENT_MOUSEMOVE;
                    evt.mouse.x = mouseX;
                    evt.mouse.y = mouseY;
                    s_Event.push_back(evt);
                }
                break;
                case SDL_KEYUP:
                {
                    if (evt.key.keysym.scancode == SDL_SCANCODE_1) {
                        Event evt;
                        evt.type = EVENT_FAST_MODE;
                        s_Event.push_back(evt);
                    }
                    else if (evt.key.keysym.scancode == SDL_SCANCODE_2) {
                        Event evt;
                        evt.type = EVENT_SKIP_PROTECTION;
                        s_Event.push_back(evt);
                    }
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP: {
                    if (evt.type == SDL_MOUSEBUTTONDOWN) {
                        if (evt.button.button == SDL_BUTTON_LEFT) {
                            Event evt;
                            evt.type = EVENT_LBUTTONDOWN;
                            s_Event.push_back(evt);
                        }
                        if (evt.button.button == SDL_BUTTON_RIGHT) {
                            Event evt;
                            evt.type = EVENT_RBUTTONDOWN;
                            s_Event.push_back(evt);
                        }
                    }
                    else if (evt.type == SDL_MOUSEBUTTONUP) {
                        if (evt.button.button == SDL_BUTTON_LEFT) {
                            Event evt;
                            evt.type = EVENT_LBUTTONUP;
                            s_Event.push_back(evt);
                        }
                        if (evt.button.button == SDL_BUTTON_RIGHT) {
                            Event evt;
                            evt.type = EVENT_RBUTTONUP;
                            s_Event.push_back(evt);
                        }
                    }
                    // printf("Mouse=%d,%d LMB=%d RMB=%d\n", s_ScreenMouseX, s_ScreenMouseY, s_LMBMouse, s_RMBMouse);
                }
                break;
            }
        }

        if (s_FastMode) {
            s_FastModeTime++;
        }
    }

    void blit(uint8* data, uint32 size) {

        assert(size <= s_widthHeight);

        uint8* pixels = (uint8*)s_virtualSurface;
        memcpy(pixels, data, size);
    }

    void cls(uint8 index) {
        memset(s_virtualSurface, index, s_widthHeight);
    }

    void setRGB(uint8 index, uint8 r, uint8 g, uint8 b) {
        s_virtualPalette[index] = { r, g, b, 255 };
        s_virtualPaletteDirty = true;
    }

    void setRGB(uint8* pal, uint32 begin, uint32 end) {
        for(int i=begin;i < end;i++) {
            uint8 r = *pal++;
            uint8 g = *pal++;
            uint8 b = *pal++;
            setRGB(i, r, g, b);
        }
    }
}}

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

        sprintf_s(diskPath, sizeof(diskPath), "data/amiga/%s", path);

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

        sprintf_s(diskPath, sizeof(diskPath), "data/amiga/%s", path);
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