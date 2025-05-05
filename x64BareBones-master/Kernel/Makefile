include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_IDT=$(wildcard IDT/*.c)
SOURRCES_MM=$(wildcard memorymanagers/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_MM=$(SOURRCES_MM:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_MM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_MM)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_MM)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o IDT/*.o

.PHONY: all clean
