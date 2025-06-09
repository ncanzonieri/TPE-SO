#ifndef UNTITLED_MEMORY_MANAGER_H
#define UNTITLED_MEMORY_MANAGER_H

#include <stdint.h>

#define MM_ADDRESS 0x600000
#define MEM_SIZE 0x100000
#define MIN_SIZE 32
#define MIN_LEVEL 1
#define MAX_LEVEL 24

typedef struct memoryStats {
	uint64_t totalMemory;
	uint64_t usedMemory;
	uint64_t freeMemory;
	uint64_t isBuddy;
} memoryStats_t;

void createMemoryManager(void *startAddress, uint32_t memorySize);
void *myMalloc(uint32_t size);
void myFree(void *ptr);
memoryStats_t *memDump();

#endif // UNTITLED_MEMORY_MANAGER_H