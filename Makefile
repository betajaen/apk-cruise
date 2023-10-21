CC=gcc
OPTIONS=

PLATFORM = sdl2
PROGRAM = ./install/cruise

CFLAGS = -I. -Iapk -Ialt -D__AMIGADATE__="\"$(DATESTR)\""

ifeq ($(PLATFORM), sdl2)
	OBJ		:= apk/sdl2.cpp
	CC		= gcc
	DELETE	= rm -f
	CFLAGS	+= -g -lSDL2 -I/opt/homebrew/include -L/opt/homebrew/lib -std=c++17 -lc++
endif

OBJ += \
	alt/alt.cpp \
	cruise/actor.cpp \
	cruise/background.cpp \
	cruise/backgroundIncrust.cpp \
	cruise/cell.cpp \
	cruise/cruise.cpp \
	cruise/cruise_main.cpp \
	cruise/ctp.cpp \
	cruise/dataLoader.cpp \
	cruise/debugger.cpp \
	cruise/decompiler.cpp \
	cruise/delphine-unpack.cpp \
	cruise/font.cpp \
	cruise/function.cpp \
	cruise/gfxModule.cpp \
	cruise/linker.cpp \
	cruise/mainDraw.cpp \
	cruise/menu.cpp \
	cruise/metaengine.cpp \
	cruise/mouse.cpp \
	cruise/object.cpp \
	cruise/overlay.cpp \
	cruise/perso.cpp \
	cruise/polys.cpp \
	cruise/saveload.cpp \
	cruise/script.cpp \
	cruise/sound.cpp \
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
