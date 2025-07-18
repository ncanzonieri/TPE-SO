include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_IDT=$(wildcard IDT/*.c)
SOURRCES_MM=$(wildcard memoryManagers/*.c)
SOURCES_PROCESS=$(wildcard Process/*.c)
SOURCES_SCHEDULER=$(wildcard Scheduler/*.c)
SOURCES_LIST=$(wildcard list/*.c)
SOURCES_SEMAPHORE=$(wildcard semaphores/*.c)
SOURCES_PIPE=$(wildcard pipes/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_MM=$(SOURRCES_MM:.c=.o)
OBJECTS_PROCESS=$(SOURCES_PROCESS:.c=.o)
OBJECTS_SCHEDULER=$(SOURCES_SCHEDULER:.c=.o)
OBJECTS_LIST=$(SOURCES_LIST:.c=.o)
OBJECTS_SEMAPHORE=$(SOURCES_SEMAPHORE:.c=.o)
OBJECTS_PIPE=$(SOURCES_PIPE:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_MM) $(OBJECTS_PROCESS) $(OBJECTS_SCHEDULER) $(OBJECTS_LIST) $(OBJECTS_SEMAPHORE) $(OBJECTS_PIPE)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_MM) $(OBJECTS_PROCESS) $(OBJECTS_SCHEDULER) $(OBJECTS_LIST) $(OBJECTS_SEMAPHORE) $(OBJECTS_PIPE)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_MM) $(OBJECTS_PROCESS) $(OBJECTS_SCHEDULER) $(OBJECTS_LIST) $(OBJECTS_SEMAPHORE) $(OBJECTS_PIPE)

%.o: %.c
ifeq ($(MM), BUDDY)
	$(GCC) $(GCCFLAGS) -DBUDDY -I./include -c $< -o $@
else
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@
endif

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o IDT/*.o memoryManagers/*.o Process/*.o Scheduler/*.o semaphores/*.o list/*.o pipes/*.o

.PHONY: all clean
