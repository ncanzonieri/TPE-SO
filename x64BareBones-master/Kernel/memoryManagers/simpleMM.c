// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef BUDDY_MM

#include <MemoryManagerADT.h>
#include <stddef.h>
#include <stdint.h>

#define BLOCK_SIZE 4096
#define MAX_BLOCKS 1024

static void *first;
static int nextFreeIndex;
static uint64_t usedMemory;
static void *freeBlocks[MAX_BLOCKS];
static memoryStats_t memoryStats;

void createMemoryManager(void *startAddress, uint32_t memorySize) {
	first = startAddress;
	nextFreeIndex = 0;
	usedMemory = 0;

	for (int i = 0; i < MAX_BLOCKS; i++) {
		freeBlocks[i] = first + (i * BLOCK_SIZE);
	}
}

void *myMalloc(uint32_t blockSize) {
	if (blockSize > BLOCK_SIZE)
		return NULL;
	if (nextFreeIndex < MAX_BLOCKS) {
		usedMemory += BLOCK_SIZE;
		return freeBlocks[nextFreeIndex++];
	}
	return NULL;
}

void myFree(void *ptr) {
	usedMemory -= BLOCK_SIZE;
	freeBlocks[--nextFreeIndex] = ptr;
}

memoryStats_t *memDump() {
	memoryStats.totalMemory = MAX_BLOCKS * BLOCK_SIZE;
	memoryStats.usedMemory = usedMemory;
	memoryStats.freeMemory = (MAX_BLOCKS * BLOCK_SIZE) - usedMemory;
	return &memoryStats;
}

#endif