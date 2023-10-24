CC=gcc
OPTIONS=

PLATFORM = sdl2
PROGRAM = ./install/cruise

CFLAGS = -I. -D__AMIGADATE__="\"$(DATESTR)\""

ifeq ($(PLATFORM), sdl2)
	OBJ		:= apk/sdl2/main.cpp apk/sdl2/gfx.cpp apk/sdl2/memory.cpp apk/sdl2/file.cpp
	CC		= gcc
	DELETE	= rm -f
	CFLAGS	+= -g -lSDL2 -I/opt/homebrew/include -L/opt/homebrew/lib -std=c++17 -lc++ -DSKIP_PROTECTION
endif

ifeq ($(PLATFORM), rtg)
	OBJ		:= apk/amiga/entry.cpp apk/amiga/main.cpp
	CC		:= /opt/amiga/bin/m68k-amigaos-gcc
	DELETE	:= rm -f
	CFLAGS  += -std=c++17 -m68020
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
	$(CC) -o $(PROGRAM) $(OBJ) $(CFLAGS) || exit 1

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ || exit 1

clean .IGNORE:
	$(DELETE) *.o
	$(DELETE) $(PROGRAM)
