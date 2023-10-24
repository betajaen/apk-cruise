CC=gcc
OPTIONS=

PLATFORM = rtg
PROGRAM = ./install/cruise

CXXFLAGS := -I. -D__AMIGADATE__="\"$(DATESTR)\""
LDFLAGS  :=

ifeq ($(PLATFORM), sdl2)
	OBJ		 := apk/sdl2/main.cpp apk/sdl2/gfx.cpp apk/sdl2/memory.cpp apk/sdl2/file.cpp
	CC		 := gcc
	DELETE	 := rm -f
	CXXFLAGS += -g -lSDL2 -I/opt/homebrew/include -L/opt/homebrew/lib -std=c++17 -lc++
	LDFLAGS  :=
endif

ifeq ($(PLATFORM), rtg)
	OBJ		  := apk/amiga/entry.cpp apk/amiga/main.cpp apk/amiga/gfx.cpp  apk/amiga/memory.cpp apk/amiga/debug.cpp apk/amiga/file.cpp
	CC		  := /opt/amiga/bin/m68k-amigaos-g++
	DELETE	  := rm -f
	CXXFLAGS  += -std=c++17 -m68020 -Wall -noixemul -fno-exceptions -fno-rtti -fno-threadsafe-statics
	LDFLAGS   := -noixemul -noixemul -fno-exceptions -fno-rtti -fno-threadsafe-statics
endif

OBJ += \
	engine/engine.cpp \
	cruise/actor.cpp \
	cruise/background.cpp \
	cruise/backgroundIncrust.cpp \
	cruise/cell.cpp \
	cruise/cruise.cpp \
	cruise/cruise_main.cpp \
	cruise/ctp.cpp \
	cruise/dataLoader.cpp \
	cruise/delphine-unpack.cpp \
	cruise/font.cpp \
	cruise/function.cpp \
	cruise/gfxModule.cpp \
	cruise/linker.cpp \
	cruise/mainDraw.cpp \
	cruise/menu.cpp \
	cruise/mouse.cpp \
	cruise/object.cpp \
	cruise/overlay.cpp \
	cruise/perso.cpp \
	cruise/polys.cpp \
	cruise/saveload.cpp \
	cruise/script.cpp \
	cruise/stack.cpp \
	cruise/staticres.cpp \
	cruise/various.cpp \
	cruise/vars.cpp \
	cruise/volume.cpp

all: $(OBJ)
	$(DELETE) $(PROGRAM)
	$(CC) $(CXXFLAGS) $(OBJ) -o $(PROGRAM) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CXX) -c $< -o $@

clean .IGNORE:
	$(DELETE) *.o
	$(DELETE) $(PROGRAM)
