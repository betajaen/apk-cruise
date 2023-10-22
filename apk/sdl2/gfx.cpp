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
#include <SDL2/SDL.h>

namespace apk {

    extern bool s_FastMode;
    extern uint32 s_FastModeTime;
    extern Array<Event> s_Event;
    extern bool s_quitRequested;
    SDL_Window* s_screen = NULL;
    constexpr int32 kScreenScale = 3;
    SDL_Surface* s_screenSurface = NULL;
    byte* s_virtualSurface = NULL;
    SDL_Color s_virtualPalette[256] = { 0 };
    bool s_virtualPaletteDirty = false;
    uint32 s_width = 0, s_height = 0, s_widthHeight = 0;

    namespace gfx {


    void createScreen(const char* title, uint16 width, uint16 height, uint8 depth) {
        SDL_assert(s_screen == NULL);

        s_screen = SDL_CreateWindow(title,
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

        fetchEvents();

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

}


    void fetchEvents() {
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
                    else if (evt.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        Event evt;
                        evt.type = EVENT_PAUSE;
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
    }

}

