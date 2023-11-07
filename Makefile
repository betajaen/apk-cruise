CC=gcc
OPTIONS=

PLATFORM = sdl2
PROGRAM_DIR = ./install/
PROGRAM_NAME = cruise
PROGRAM = $(PROGRAM_DIR)$(PROGRAM_NAME)

CXXFLAGS := -I. -D__AMIGADATE__="\"$(DATESTR)\""
LDFLAGS  :=

SDL2_RUN     := (cd $(PROGRAM_DIR) && $(PROGRAM_NAME))

ifeq ($(PLATFORM), sdl2)
	OBJ		 := apk/sdl2/main.cpp apk/sdl2/gfx.cpp apk/sdl2/memory.cpp apk/sdl2/file.cpp apk/sdl2/compat.cpp
	CC		 := gcc
	DELETE	 := rm -f
	CXXFLAGS += -g -lSDL2 -I/opt/homebrew/include -L/opt/homebrew/lib -std=c++17 -fno-exceptions -fno-rtti -fno-threadsafe-statics
	LDFLAGS  :=
endif

AMIGA_OBJ       := apk/amiga/entry.cpp apk/amiga/compat.cpp apk/amiga/main.cpp  apk/amiga/memory.cpp apk/amiga/debug.cpp apk/amiga/file.cpp apk/amiga/requester.cpp apk/amiga/window.cpp
AMIGA_CC		:= /opt/amiga/bin/m68k-amigaos-gcc
AMIGA_DELETE	:= rm -f
AMIGA_CXXFLAGS  := -std=c++17 -m68020 -Wall -noixemul -fno-exceptions -fno-rtti -fno-threadsafe-statics
AMIGA_LDFLAGS   := -noixemul -noixemul -fno-exceptions -fno-rtti -fno-threadsafe-statics
AMIGA_RUN 		:= 	fs-uae \
					--model=A1200 \
					--cpu=68030 \
					--zorro_iii_memory=16384 \
					--kickstart_file=./install/amigaos-3.2-a1200.rom \
					--hard_drive_0=./install/amigaos-3.2-a1200.hdf \
					--hard_drive_1=./install/ \
					--hard_drive_1_label="Cruise" \
					--floppy_drive_volume=0 \
					--floppy_drive_volume_empty=0 \
					--smoothing=0 \
					--graphics_card=uaegfx \
					--fssa=0 \
					--texture_filter=nearest \
					--window_x=3001 \
					--window_y=-200 \
					--window_width=1400 \
					--window_height=1024 \
					--scale=2

ifeq ($(PLATFORM), rtg)
	OBJ		  += $(AMIGA_OBJ)
	OBJ       += apk/amiga/gfx_rtg.cpp
	CC		  := $(AMIGA_CC)
	DELETE	  := $(AMIGA_DELETE)
	CXXFLAGS  += $(AMIGA_CXXFLAGS)
	LDFLAGS   := $(AMIGA_LDFLAGS)
endif

ifeq ($(PLATFORM), aga)
	OBJ		  += $(AMIGA_OBJ)
	OBJ       += apk/amiga/gfx_aga.cpp
	CC		  := $(AMIGA_CC)
	DELETE	  := $(AMIGA_DELETE)
	CXXFLAGS  += $(AMIGA_CXXFLAGS)
	LDFLAGS   := $(AMIGA_LDFLAGS)
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

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(DELETE) $(PROGRAM) || exit 1
	if $(CC) $(CXXFLAGS) $(OBJ) -o $(PROGRAM) $(LDFLAGS); then \
		echo "Compiled to $(PROGRAM)"; \
	else \
	  	echo "Compilation failed."; \
	fi

%.o: %.cpp
	$(CC) $(CXX) -c $< -o $@

clean .IGNORE:
	$(DELETE) *.o
	$(DELETE) $(PROGRAM)

run:
	@HUNK_COOKIE="000003F3"; \
	if hexdump -n 4 -e '4/1 "%02X"' $(PROGRAM) | grep -q "$$HUNK_COOKIE"; then \
		$(AMIGA_RUN); \
	else \
		$(SDL2_RUN); \
	fi

.PHONY: $(PROGRAM) run run_amiga