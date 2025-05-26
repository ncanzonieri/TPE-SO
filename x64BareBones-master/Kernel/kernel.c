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

int idle(uint64_t argc, char **argv)
{
	// This is the idle process, it does nothing and waits for other processes to run.
	while (1) {
		// Optionally, you can add a sleep or yield here to prevent busy-waiting.
		_hlt();  // Sleep for 1000 milliseconds (1 second)
	}
	return 0;  // Should never reach here
}


int main() 
{	
	createMemoryManager((void*) START_MM, MEM_FOR_MM);
	load_idt();
	initScheduler();
	createProcess("init", 3, 0, (ProcessEntry) &idle, NULL, 0);
	createProcess("shell", 3, 0, (ProcessEntry) sampleCodeModuleAddress, NULL, 0);
	//deberíamos enviar un puntero a la función de inicialización del módulo
	//createProcess("init", PRIORITY_1, NULL, 0, (main_function) &idle, fd);
	//createProcess("Shell", PRIORITY_4, NULL, 0, (main_function) sampleCodeModuleAddress, fd);
	return ((EntryPoint)sampleCodeModuleAddress)();
}
