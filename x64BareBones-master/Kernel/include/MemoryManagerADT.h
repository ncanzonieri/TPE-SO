#ifndef UNTITLED_MEMORY_MANAGER_H
#define UNTITLED_MEMORY_MANAGER_H

#include <stdint.h>

#define MEM_SIZE 1048576
#define MIN_SIZE 32

typedef struct mem_info {
	uint64_t total_mem;
	uint64_t used_mem;
	uint64_t free_mem;
} mem_info_t;

void createMemoryManager(void *m, uint32_t s);
void *myMalloc(uint32_t size);
void myFree(void *ptr);
mem_info_t *memDump();

#endif // UNTITLED_MEMORY_MANAGER_H