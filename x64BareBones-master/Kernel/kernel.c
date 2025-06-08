#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <time.h>
#include <audioDriver.h>
#include <MemoryManagerADT.h>
#include <scheduler.h>
#include <process.h>
#include <semaphores.h>
#include <pipes.h>

#define MEM_FOR_MM 0x100000
//#define START_MM 0x600000
#define START_MM 0xF00000

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();  //puntero a una funcion que no recibe 
							// parametros y devuelve int 


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = { sampleCodeModuleAddress, sampleDataModuleAddress };
	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);	
	return getStackBase();
	
}
void idle() {
	while (1) {	_hlt();	}
}


int main() 
{
	_cli();
	load_idt();
	createMemoryManager((void*) START_MM, MEM_FOR_MM);
	createScheduler();
	createSemManager();
	createPipeManager();
	int fds[] = {STDIN, STDOUT};
	createProcess("init    ", MIN_PRIORITY, 1, (ProcessEntry) &idle, NULL, 0, fds);
	createProcess("Shell   ", MAX_PRIORITY, 1, (ProcessEntry) sampleCodeModuleAddress, NULL, 0, fds);
	
	_sti();
	while(1);

	return 0;
}
