#pragma once

#include <cstdint>
#include "apk/types.h"

#define Common apk
#define Cruise apk

#define PACKED_STRUCT
#define WARN_UNUSED_RESULT

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
    constexpr int32 KEYCODE_INVALID = 0;
    constexpr int32 KEYCODE_x = 1;
    constexpr int32 KEYCODE_p = 2;
    constexpr int32 KEYCODE_f = 3;

    constexpr int32 KEYCODE_F9 = 30;
    constexpr int32 KEYCODE_F10 = 31;
    constexpr int32 KEYCODE_SPACE = 50;
    constexpr int32 KEYCODE_KP_PLUS = 60;
    constexpr int32 KEYCODE_KP_MINUS = 70;
    constexpr int32 KEYCODE_ESCAPE = 71;


    typedef int32 KeyCode;

    typedef int32 Language;
    typedef int32 Platform;
    typedef int32 EngineFeature;
    typedef void* NotHandled;
    typedef int32 Error;

    constexpr int32 EVENT_LBUTTONDOWN = 1;
    constexpr int32 EVENT_LBUTTONUP = 2;
    constexpr int32 EVENT_RBUTTONDOWN = 3;
    constexpr int32 EVENT_RBUTTONUP = 4;
    constexpr int32 EVENT_MOUSEMOVE = 5;
    constexpr int32 EVENT_KEYDOWN = 6;
    constexpr int32 EVENT_KEYUP = 7;
    constexpr int32 EVENT_QUIT = 8;
    constexpr int32 EVENT_RETURN_TO_LAUNCHER = 9;

    struct Event {
        int32 type;
        struct M {
            int16 x, y;
        } mouse;
        struct K {
            int32 keycode;
            bool hasFlags(int32) {
                return false;
            }
        } kbd;
    };

    class EventManager {
    public:
        bool pollEvent(Event& evt) {
            return false;
        }
    };

    class PauseToken {
    public:
        void clear() {
        }
    };

    typedef Array<String> StringArray;

    class PaletteManager {
    public:
        void setPalette(uint8* pal, uint32 start, uint32 end) {
        }
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

    class Serializer {
    public:
        Serializer(void*, void* f) {
        }
        bool err() {
            return false;
        }
        bool isSaving() {
            return false;
        }
        bool isLoading() {
            return false;
        }
        void syncBytes(void* data, uint32 length) {}
        void syncAsByte(uint8 byte) {}
        void syncAsSint16LE(int16 v) {}
        void syncAsSint32LE(int32 v) {}
        void syncAsSint16BE(int16 v) {}
        void syncAsSint32BE(int32 v) {}
        void syncAsUint16LE(int16 v) {}
        void syncAsUint32LE(int32 v) {}
        void syncAsUint16BE(int16 v) {}
        void syncAsUint32BE(int32 v) {}
        void finalize() {
        }
    };

    typedef Serializer InSaveFile;
    typedef Serializer OutSaveFile;

    class SaveFileManager {
    public:
        InSaveFile* openForLoading(const char*) {
            return NULL;
        }
        OutSaveFile* openForSaving(const char*) {
            return NULL;
        }
        void removeSavefile(const char*) {
        }
    };

    class OSystem {
    public:
        EventManager m_eventMgr;
        ConfigManager m_configMgr;
        PaletteManager m_paletteMgr;
        CursorManager m_cursorMgr;
        SaveFileManager m_saveFileMgr;

        void delayMillis(uint32 ms) {
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

        SaveFileManager* getSavefileManager() {
            return &m_saveFileMgr;
        }

        int32 getMillis() {
            return 0;
        }

        void updateScreen();

        void copyRectToScreen(uint8* data, uint32 pitch, uint32 x, uint32 y, uint32 w, uint32 h);

        static OSystem s_instance;
    };


    class Engine {
    public:
        Audio::Mixer* _mixer;
        Engine(OSystem*) { _mixer = new Audio::Mixer ();}
        virtual ~Engine() { delete _mixer; }
        virtual Error run() { return 0; }
        void setDebugger(void*) {}
        PauseToken pauseEngine() { return PauseToken(); }
        virtual bool hasFeature(EngineFeature f) const {
            return false;
        }
        virtual Error loadGameState(int slot) {
            return 0;
        }
        virtual Error saveGameState(int slot) {
            return 0;
        }
        virtual Error saveGameState(int slot, const String& desc, bool isAutoSave = false) {
            return 0;
        }
        virtual bool canLoadGameStateCurrently() {
            return false;
        }
        virtual bool canSaveGameStateCurrently() {
            return false;
        }
        virtual String getSaveStateName(int slot) const {
            return String();
        }

        virtual void syncSoundSettings() {
        }

        void initGraphics(int32 w, int32 h);

        bool shouldQuit() {
            return apk::isQuitRequested();
        }

        void saveGameDialog() {
        }

        void loadGameDialog() {
        }

        inline Common::Language getLanguage() const {
            return EN_ANY;
        }

        inline Common::Platform getPlatform() const {
            return 0;
        }

    };

    class RandomSource {
    public:
        RandomSource(const char*) {}
        int32 getRandomNumber(int i) {
            return 0;
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
                    free(data);
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
            return endian::from_be_int16(data);
        }

        int32 readSint32BE() {
            assert(m_buf.isEnd() == false);
            int32 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::from_be_int32(data);
        }

        uint16 readUint16BE() {
            assert(m_buf.isEnd() == false);
            uint16 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::from_be_uint16(data);
        }

        uint32 readUint32BE() {
            assert(m_buf.isEnd() == false);
            uint32 data = 0;
            assert(m_buf.read(&data, sizeof(data)) == sizeof(data));
            return endian::from_be_uint32(data);
        }
    };


    template<typename T>
    T ABS(T v) {
        return v < 0 ? -v : v;
    }

    template<typename T>
    T MIN(T lhs, T rhs) {
        return (lhs < rhs) ? lhs : rhs;
    }

    template<typename T>
    T MAX(T lhs, T rhs) {
        return (lhs > rhs) ? lhs : rhs;
    }

    template<typename T>
    T CLIP(T x, T min, T max) {
        if (x < min)
            x = min;
        else if (x > max)
            x = max;
        return x;
    }

    template<typename T>
    void SWAP(T& l, T& r) {
        T t = l;
        l = r;
        r = t;
    }

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

