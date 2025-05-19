// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "MemoryManagerADT.h"
#include <stddef.h>
//#ifndef BUDDY_MM

#include <stdint.h>

#define BLOCK_SIZE 4096
#define MAX_BLOCKS 1024

static void *start;
static int current;
static uint64_t used_mem;
static void *freeBlocks[MAX_BLOCKS];
static mem_info_t mem_info;

void mm_init(void *m, uint32_t s) {
	start = m;
	current = 0;
	used_mem = 0;

	for (int i = 0; i < MAX_BLOCKS; i++) {
		freeBlocks[i] = start + (i * BLOCK_SIZE);
	}
}

void *mm_alloc(uint32_t blockSize) {
	if (blockSize > BLOCK_SIZE)
		return NULL;
	if (current < MAX_BLOCKS) {
		used_mem += BLOCK_SIZE;
		return freeBlocks[current++];
	}
	return NULL;
}

void mm_free(void *ptr) {
	used_mem -= BLOCK_SIZE;
	freeBlocks[--current] = ptr;
}


mem_info_t *mem_dump() {
	mem_info.total_mem = MAX_BLOCKS * BLOCK_SIZE;
	mem_info.used_mem = used_mem;
	mem_info.free_mem = (MAX_BLOCKS * BLOCK_SIZE) - used_mem;
	return &mem_info;
}


/*
#include "MemoryManagerADT.h"
#include <stddef.h>

static void* memory_start = NULL;
static size_t memory_size = 0;
static MemoryBlockHeader* free_list = NULL;

#define MIN_BLOCK_SIZE (sizeof(MemoryBlockHeader))


int64_t mm_init(void* start_addr, size_t size) {
    if (start_addr == NULL || size <= sizeof(MemoryBlockHeader)) {
        return -1;
    }
    memory_start = start_addr;
    memory_size = size;
    free_list = (MemoryBlockHeader*)start_addr;
    free_list->size = size - sizeof(MemoryBlockHeader);
    free_list->is_free = 1;
    free_list->next = NULL;
    return 0;
}

void* mm_alloc(size_t size) {
    if (size == 0 || free_list == NULL) {
        return NULL;
    }
    if (size % 8 != 0) {
        size = (size / 8 + 1) * 8;
    }
    if (size < MIN_BLOCK_SIZE) {
        size = MIN_BLOCK_SIZE;
    }
    MemoryBlockHeader* current = free_list;
    MemoryBlockHeader* prev = NULL;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (current->size >= size + sizeof(MemoryBlockHeader) + MIN_BLOCK_SIZE) {
                MemoryBlockHeader* new_block = (MemoryBlockHeader*)((char*)current + sizeof(MemoryBlockHeader) + size);
                new_block->size = current->size - size - sizeof(MemoryBlockHeader);
                new_block->is_free = 1;
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }
            current->is_free = 0;
            return (void*)((char*)current + sizeof(MemoryBlockHeader));
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

int64_t mm_free(void* ptr) {
    if (ptr == NULL || memory_start == NULL) {
        return -1;
    }
    MemoryBlockHeader* block = (MemoryBlockHeader*)((char*)ptr - sizeof(MemoryBlockHeader));
    if ((void*)block < memory_start || (void*)block >= (void*)((char*)memory_start + memory_size)) {
        return -1;
    }
    block->is_free = 1;
    MemoryBlockHeader* current = free_list;
    while (current != NULL && current->next != NULL) {
        if (current->is_free && current->next->is_free) {
            current->size += sizeof(MemoryBlockHeader) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
    return 0;
}

MemoryInfo mm_info(void) {
    MemoryInfo info = {0};
    if (memory_start == NULL) {
        return info;
    }
    info.total = memory_size;
    info.free = 0;
    info.blocks = 0;
    MemoryBlockHeader* current = free_list;
    while (current != NULL) {
        info.blocks++;
        if (current->is_free) {
            info.free += current->size;
        }
        current = current->next;
    }
    info.used = info.total - info.free - info.blocks * sizeof(MemoryBlockHeader);
    return info;
}

*/