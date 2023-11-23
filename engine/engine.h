#pragma once

#include <cstdint>
#include "apk/apk.h"
#include "apk/gfx.h"

#define Common apk
#define Cruise apk

#define PACKED_STRUCT __attribute__((__packed__))
#define WARN_UNUSED_RESULT

#define CRUISE_OPTIMIZE_DRAW_SPRITE 0

namespace Graphics {
    typedef void* Surface;
}

namespace Audio {
    class Mixer {
    public:
    };
}

namespace OPL {
    class OPL {
    public:
        void start(void*) {
        }
        bool init() {
            return false;
        }
        void writeReg(int i, int i_1) {
        }
    };

    class Config {
    public:
        static OPL* create() {
            return nullptr;
        }
    };
}

namespace apk {

    constexpr int32 gDebugLevel = 0;
    constexpr int32 kSupportsReturnToLauncher = 0;
    constexpr int32 kSupportsLoadingDuringRuntime = 1;
    constexpr int32 kSupportsSavingDuringRuntime = 2;
    constexpr int32 kUnknownError = 1;
    constexpr int32 kNoError = 0;
    constexpr int32 kNoGameDataFoundError = 2;
    constexpr int32 kWritingFailed = 3;
    constexpr int32 kReadingFailed = 4;


    constexpr int32 EN_ANY= 0;
    constexpr int32 FR_FRA= 1;
    constexpr int32 DE_DEU= 2;
    constexpr int32 IT_ITA= 3;
    constexpr int32 ES_ESP= 4;

    constexpr int32 KBD_CTRL = 0;
    constexpr int32 KEYCODE_INVALID = APKK_INVALID;
    constexpr int32 KEYCODE_x = APKK_X;
    constexpr int32 KEYCODE_p = APKK_P;
    constexpr int32 KEYCODE_f = APKK_F;

    constexpr int32 KEYCODE_F1 = APKK_F1;
    constexpr int32 KEYCODE_F2 = APKK_F2;
    constexpr int32 KEYCODE_F3 = APKK_F3;
    constexpr int32 KEYCODE_F4 = APKK_F4;
    constexpr int32 KEYCODE_F5 = APKK_F5;
    constexpr int32 KEYCODE_F6 = APKK_F6;
    constexpr int32 KEYCODE_F7 = APKK_F7;
    constexpr int32 KEYCODE_F8 = APKK_F8;
    constexpr int32 KEYCODE_F9 = APKK_F9;
    constexpr int32 KEYCODE_F10 = APKK_F10;

    constexpr int32 KEYCODE_SPACE = APKK_SPACE;
    constexpr int32 KEYCODE_KP_PLUS = APKK_NP_PLUS;
    constexpr int32 KEYCODE_KP_MINUS = APKK_NP_MINUS;
    constexpr int32 KEYCODE_ESCAPE = APKK_ESCAPE;


    typedef int32 KeyCode;

    typedef int32 Language;
    typedef int32 Platform;
    typedef int32 EngineFeature;
    typedef void* NotHandled;
    typedef int32 Error;


    class EventManager {
    public:
        void fetchEvents() {
            //::apk::fetchEvents();
        }
        bool pollEvent(Event& evt) {
            return false;
            //return ::apk::pollEvents(evt);
        }
    };

    class PauseToken {
    public:
        bool isPaused = true;
        void clear() {
            isPaused = false;
        }
    };

    class PaletteManager {
    public:
        bool ignoreUpdates = false;
        void setPalette(uint8* pal, uint32 start, uint32 end);
    };

    class ConfigManager {
    public:
        bool hasKey(const char* name) {
            return false;
        }
        int32 getInt(const char* name) {
            return 0;
        }
        bool getBool(const char* name) {
            return false;
        }
    };


    class CursorManager {
    public:
        void showMouse(bool) {
        }
        void replaceCursor(byte byte_1[256],
            int i,
            int i_1,
            const int i_2,
            const int i_3,
            int i_4) {
        }
        void replaceCursorPalette(const byte*, uint32, uint32) {
        }
        bool isVisible() {
            return false;
        }
    };

    class InSerializer {
        ReadFile m_File;
    public:
         InSerializer() {
        }

        ~InSerializer() {
            finalize();
        }

        bool open(const char* path) {
            return m_File.open(path);
        }

        bool isLoading() const {
            return true;
        }

        bool isSaving() const {
            return false;
        }

        void syncBytes(void* data, uint32 length) {
            m_File.read(data, length);
        }

        void syncValue(bool& v) {
             m_File.read(&v, sizeof(v));
        }

        void syncValue(uint8& v) {
             m_File.read(&v, sizeof(v));
        }

        template<typename T>
        void syncAsByte(T& v) {
             uint8 tmp;
             m_File.read(&tmp, sizeof(tmp));
             v = (T) tmp;
         }

        template<typename T>
        void syncValue(T& v) {
             m_File.read(&v, sizeof(v));
        }

        template<typename T>
        void syncAsSint16LE(T& v) {
             int16 tmp;
             m_File.read(&tmp, sizeof(tmp));
             v = (T) endian::pod<int16, endian::Little, endian::Native>(tmp);
        }

        template<typename T>
        void syncAsSint32LE(T& v) {
             int32 tmp;
             m_File.read(&tmp, sizeof(tmp));
             v = (T) endian::pod<int32, endian::Little, endian::Native>(tmp);
        }


        template<typename T>
        void syncAsUint16LE(T& v) {
             uint16 tmp;
             m_File.read(&tmp, sizeof(tmp));
             v = (T) endian::pod<uint16, endian::Little, endian::Native>(tmp);
        }

        template<typename T>
        void syncAsUint32LE(T& v) {
             uint32 tmp;
             m_File.read(&tmp, sizeof(tmp));
             v = (T) endian::pod<uint32, endian::Little, endian::Native>(tmp);
        }


        void finalize() {
            m_File.close();
        }
    };

    class OutSerializer {
        AppendFile m_File;
    public:
        OutSerializer() {
        }

        ~OutSerializer() {
            finalize();
        }

        bool open(const char* path) {
            return m_File.open(path);
        }

        bool isLoading() const {
            return false;
        }

        bool isSaving() const {
            return true;
        }

        void syncBytes(void* data, uint32 length) {
            m_File.write(data, length);
        }

        template<typename T>
        void syncAsByte(T& v) {
             uint8 tmp = (uint8) v;
             m_File.write(&tmp, sizeof(tmp));
        }

        void syncValue(uint8& v) {
             m_File.write(&v, sizeof(v));
        }

        void syncValue(bool& v) {
             m_File.write(&v, sizeof(v));
        }

        template<typename T>
        void syncValue(T& v) {
             m_File.write(&v, sizeof(v));
        }


        template<typename T>
        void syncAsSint16LE(T& v) {
             int16 tmp = endian::pod<int16, endian::Native, endian::Little>((int16) v);
             m_File.write(&tmp, sizeof(tmp));
        }

        template<typename T>
        void syncAsSint32LE(T& v) {
             int32 tmp = endian::pod<int16, endian::Native, endian::Little>((int32) v);
             m_File.write(&tmp, sizeof(tmp));
        }

        template<typename T>
        void syncAsUint16LE(T& v) {
             uint16 tmp = endian::pod<uint16, endian::Native, endian::Little>((uint16) v);
             m_File.write(&tmp, sizeof(tmp));
        }

        template<typename T>
        void syncAsUint32LE(T& v) {
             uint32 tmp = endian::pod<uint32, endian::Native, endian::Little>((uint32) v);
             m_File.write(&tmp, sizeof(tmp));
        }

        void finalize() {
            m_File.close();
        }
    };


    class OSystem {
    public:
        EventManager m_eventMgr;
        ConfigManager m_configMgr;
        PaletteManager m_paletteMgr;
        CursorManager m_cursorMgr;

        void delayMillis(uint32 ms) {
            apk::delayMs(ms);
        }

        EventManager* getEventManager() {
            return &m_eventMgr;
        }

        PaletteManager* getPaletteManager() {
            return &m_paletteMgr;
        }

        CursorManager* getCursorManager() {
            return &m_cursorMgr;
        }

        int32 getMillis() {
            return apk::getMs();
        }

        void updateScreen();

        void copyRectToScreen(uint8* data, uint32 pitch, uint32 x, uint32 y, uint32 w, uint32 h);
        void copyToScreen(uint8* data, uint32 size);
        void box(uint32 left, uint32 top, uint32 right, uint32 bottom);
        static OSystem s_instance;
    };


    class PCSound {
    public:
        PCSound() {}
        void loadMusic(const char *name) {}
        void playMusic() {}
        void stopMusic() {}
        void removeMusic() {}
        void fadeOutMusic() {}
        void startNote(int, int, int);
        void playSound(const uint8 *data, int size, int volume) {}
        void stopSound(int channel) {}
        void syncSounds() {}
        void stopChannel(int) {}
        bool songLoaded() { return true; }
        bool songPlayed() { return true; }
        bool isPlaying() { return false; }
        const char* musicName() { return ""; }
        void setNumOrders(int) {}
        int numOrders() { return 0; }
        void setPattern(int, int) {}
        void fadeSong() {}
        bool musicLooping() { return false; }
        void musicLoop(int) {}
        int getVolume() { return 0; }
        void setVolume(int) {}
        template<typename T>
        void doSync(T&) {}
    };

    class RandomSource {
    public:
        int seed = 3458345;
        RandomSource(const char*) {}
        int32 getRandomNumber(int i) {
            seed ^= seed >> 13;
            seed ^= seed << 21;
            seed ^= seed >> 11;
            return seed % (i+1);
        }
    };

    enum class DisposeAfterUse {
        NO = 0,
        YES = 1
    };

    class MemoryReadStream {
        MemoryBuffer m_buf;
        bool m_dispose;
    public:

        MemoryReadStream(uint8* buffer, uint32 size, DisposeAfterUse dispose = DisposeAfterUse::NO) {
            m_buf = MemoryBuffer(buffer, size);
            m_dispose = (dispose == DisposeAfterUse::YES);
        }

        ~MemoryReadStream() {
            if (m_dispose) {
                byte* data = m_buf.getData();
                if (data) {
                    apk_deallocate(data);
                }
            }
        }

        void skip(uint32 len) {
            m_buf.seek(len, kSEEK_CUR);
        }

        uint32 read(void* data, uint32 len) {
            assert(m_buf.isEnd() == false);
            return m_buf.read(data, len);
        }

        byte readByte() {
            assert(m_buf.isEnd() == false);
            byte data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return data;
        }

        int16 readSint16BE() {
            assert(m_buf.isEnd() == false);
            int16 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::pod<int16, endian::Big>(data);
        }

        int32 readSint32BE() {
            assert(m_buf.isEnd() == false);
            int32 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::pod<int32, endian::Big>(data);
        }

        uint16 readUint16BE() {
            assert(m_buf.isEnd() == false);
            uint16 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::pod<uint16, endian::Big>(data);
        }

        uint32 readUint32BE() {
            assert(m_buf.isEnd() == false);
            uint32 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::pod<uint32, endian::Big>(data);
        }
    };



    inline void fill(uint8* start, uint8* end, uint8 col) {
        memset(start, col, start - end);
    }

    extern OSystem* g_system;
}
namespace GUI {
    class Debugger {
    };
}

#define g_system (&OSystem::s_instance)
#define ConfMan (OSystem::s_instance.m_configMgr)
#define CursorMan (OSystem::s_instance.m_cursorMgr)

#define READ_UINT32(X)       apk::endian::peek<uint32, endian::Native>(X)
#define READ_UINT16(X)       apk::endian::peek<uint16, endian::Native>(X)
#define READ_INT32(X)        apk::endian::peek<int32,  endian::Native>(X)
#define READ_INT16(X)        apk::endian::peek<int16,  endian::Native>(X)
#define READ_BE_UINT32(X)    apk::endian::peek<uint32, endian::Big>(X)
#define READ_BE_UINT16(X)    apk::endian::peek<uint16, endian::Big>(X)
#define READ_BE_INT32(X)     apk::endian::peek<int32,  endian::Big>(X)
#define READ_BE_INT16(X)     apk::endian::peek<int16,  endian::Big>(X)
#define READ_LE_UINT32(X)    apk::endian::peek<uint32, endian::Little>(X)
#define READ_LE_UINT16(X)    apk::endian::peek<uint16, endian::Little>(X)
#define READ_LE_INT32(X)     apk::endian::peek<int32,  endian::Little>(X)
#define READ_LE_INT16(X)     apk::endian::peek<int16,  endian::Little>(X)
#define WRITE_UINT32(MEM, X)      apk::endian::poke<uint32, endian::Native>(MEM, X)
#define WRITE_UINT16(MEM, X)      apk::endian::poke<uint16, endian::Native>(MEM, X)
#define WRITE_INT32(MEM, X)       apk::endian::poke<int32, endian::Native>(MEM, X)
#define WRITE_INT16(MEM, X)       apk::endian::poke<int16, endian::Native>(MEM, X)
#define WRITE_BE_UINT32(MEM, X)      apk::endian::poke<uint32, endian::Native, endian::Big>(MEM, X)
#define WRITE_BE_UINT16(MEM, X)      apk::endian::poke<uint16, endian::Native, endian::Big>(MEM, X)
#define WRITE_BE_INT32(MEM, X)       apk::endian::poke<int32, endian::Native, endian::Big>(MEM, X)
#define WRITE_BE_INT16(MEM, X)       apk::endian::poke<int16, endian::Native, endian::Big>(MEM, X)
#define WRITE_LE_UINT32(MEM, X)      apk::endian::poke<uint32, endian::Native, endian::Little>(MEM, X)
#define WRITE_LE_UINT16(MEM, X)      apk::endian::poke<uint16, endian::Native, endian::Little>(MEM, X)
#define WRITE_LE_INT32(MEM, X)       apk::endian::poke<int32, endian::Native, endian::Little>(MEM, X)
#define WRITE_LE_INT16(MEM, X)       apk::endian::poke<int16, endian::Native, endian::Little>(MEM, X)
