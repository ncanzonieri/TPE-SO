#ifndef UNTITLED_MEMORY_MANAGER_H
#define UNTITLED_MEMORY_MANAGER_H

#include <stdint.h>

#define MEM_SIZE 1048576
#define MIN_SIZE 32

typedef struct memoryStats {
	uint64_t totalMemory;
	uint64_t usedMemory;
	uint64_t freeMemory;
} memoryStats_t;

void createMemoryManager(void *startAddress, uint32_t memorySize);
void *myMalloc(uint32_t size);
void myFree(void *ptr);
memoryStats_t *memDump();

#endif // UNTITLED_MEMORY_MANAGER_H