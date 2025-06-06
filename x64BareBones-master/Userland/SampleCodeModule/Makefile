include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_COMMANDS=$(wildcard commands/*.c)
SOURCES_LIB=$(wildcard lib/*.c)
SOURVES_POWERSHELL=$(wildcard powershell/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_TEST=$(wildcard tests/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_COMMANDS=$(SOURCES_COMMANDS:.c=.o)
OBJECTS_LIB=$(SOURCES_LIB:.c=.o)
OBJECTS_POWERSHELL=$(SOURVES_POWERSHELL:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_TEST=$(SOURCES_TEST:.c=.o)

LOADER_SRC=_loader.c
LOADER_OBJECT=$(LOADER_SRC:.c=.o)
STATICLIBS=

all: $(MODULE)

$(MODULE): $(LOADER_OBJECT) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_COMMANDS) $(OBJECTS_LIB) $(OBJECTS_TEST) $(OBJECTS_POWERSHELL) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(LOADER_OBJECT) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_COMMANDS) $(OBJECTS_LIB) $(OBJECTS_POWERSHELL) $(OBJECTS_TEST) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf $(LOADER_OBJECT) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_COMMANDS) $(OBJECTS_LIB) $(OBJECTS_POWERSHELL) $(OBJECTS_TEST) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADER_OBJECT):
	$(GCC) $(GCCFLAGS) $(LOADER_SRC) -c -o $(LOADER_OBJECT)

clean:
	rm -rf *.o asm/*.o commands/*.o lib/*.o powershell/*.o tests/*.o

.PHONY: all clean print
