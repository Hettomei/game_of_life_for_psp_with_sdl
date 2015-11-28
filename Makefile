TARGET = jeux_de_la_vie
OBJS = main.o

CFLAGS = -O2 -G0 -Wall -D PSP
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lstdc++ -lSDL_image -lSDL_ttf -ljpeg -lpng -lfreetype -lz
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = jeux de la vie01
PSP_EBOOT_ICON = ICON0.PNG

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
CFLAGS += $(shell $(PSPBIN)/sdl-config --cflags)
CXXFLAGS += $(shell $(PSPBIN)/sdl-config --cflags)
LIBS += $(shell $(PSPBIN)/sdl-config --libs)
include $(PSPSDK)/lib/build.mak

all:
	mkdir jeux_de_la_vie
	cp EBOOT.PBP ICON0.PNG jeux_de_la_vie/
	rm -f *.o *.SFO EBOOT.PBP *.elf
