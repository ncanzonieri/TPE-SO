#ifdef BUDDY

#include <MemoryManagerADT.h>
#include <lib.h>
#include <videoDriver.h>
#include <stddef.h>

typedef struct BuddyBlock {
	struct BuddyBlock * prev;
	struct BuddyBlock * next;
	int level;
	char isFree;
} BuddyBlock;

typedef struct BuddyBlock * Block;

static Block splitBlock(Block block);
static Block createFreeBlock(void* startAddress, int level, Block next, Block prev);
static Block findBlock(int level);
static void mergeBlock(Block block);
static void addBlock(Block block);
static void removeBlock(Block block);


struct MemoryManagerCDT {
	uint64_t size;
	void* startAddress;
	uint64_t maxLevels;
	Block freeList[MAX_LEVEL];
}MemoryManagerCDT;

typedef struct MemoryManagerCDT * memManager;
static memoryStats_t memoryStats;

static memManager getMemoryManager() {
	return (memManager) MM_ADDRESS;
}

void createMemoryManager(void *startAddress, uint32_t memorySize) {
	memManager mem = getMemoryManager();
	mem->startAddress = startAddress;
	mem->size = memorySize;
	mem->maxLevels = log2(memorySize);

	if (mem->maxLevels > MAX_LEVEL) {
		printString(0xFF0000,"Error, tamaño de memoria demasiado grande");
	}
	else if (mem->maxLevels < MIN_LEVEL) {
		printString(0xFF0000, "Error, tamaño de memoria demasiado pequeño");
	}

	for (int i = 0; i < mem->maxLevels; i++) {
		mem->freeList[i] = NULL;
	}

	int level = mem->maxLevels - 1;
	mem->freeList[level] = createFreeBlock(startAddress, level, NULL, NULL);
    memoryStats.totalMemory = memorySize;
    memoryStats.usedMemory = 0;
    memoryStats.freeMemory = memorySize;
}

void *myMalloc(uint32_t size){
	int level = log2(size + sizeof(BuddyBlock));

	if (level < MIN_LEVEL) {
		level = MIN_LEVEL;
	}
	else if (level > MAX_LEVEL) {
		return NULL;
	}

	Block block = findBlock(level);
	if (block == NULL) {
		return NULL;
	}
    memoryStats.usedMemory += (1ULL << block->level);
    memoryStats.freeMemory -= (1ULL << block->level);
	return (void *) ((uint64_t) block + sizeof(BuddyBlock));
}

void myFree(void *ptr){
	if (ptr == NULL) {
		return;
	}

	Block block = (Block) ((uint64_t) ptr - sizeof(BuddyBlock));

	if (block->isFree) {
		return;
	}

	block->isFree = 1;
    memoryStats.usedMemory -= (1ULL << block->level);
    memoryStats.freeMemory += (1ULL << block->level);
	mergeBlock(block);
}

memoryStats_t *memDump(){
    memManager memoryManager = getMemoryManager();
	memoryStats.totalMemory = memoryManager->size;
    memoryStats.isBuddy = 1;
    return &memoryStats;
}

static Block createFreeBlock(void * startAddress, int level, Block next, Block prev) {
	Block block = (Block) startAddress;
	block->level = level;
	block->isFree = 1;
	block->next = next;
	block->prev = prev;
	return block;
}

static Block splitBlock(Block block) {
	memManager mem = getMemoryManager();
	int newLevel = block->level - 1;

	block->level = newLevel;

	Block buddy = (Block ) ((uint64_t) block + (1 << newLevel));

	buddy->level = newLevel;
	buddy->isFree = 1;
	buddy->next = mem->freeList[newLevel];
	buddy->prev = NULL;

	if (mem->freeList[newLevel] != NULL) {
		mem->freeList[newLevel]->prev = buddy;
	}
	mem->freeList[newLevel] = buddy;

	return block;
}

static Block findBlock(int level) {
	memManager mem = getMemoryManager();

	if (level > mem->maxLevels) {
		return NULL;
	}

	for (int i = level; i <= mem->maxLevels; i++) {
		if (mem->freeList[i] != NULL) {
			Block block = mem->freeList[i];

			mem->freeList[i] = block->next;
			if (block->next != NULL) {
				block->next->prev = NULL;
			}
			block->next = block->prev = NULL;

			while (i > level) {
				block = splitBlock(block);
				i--;
			}

			block->isFree = 0;
			return block;
		}
	}

	return NULL;
}

static void mergeBlock(Block block) {
	memManager mem = getMemoryManager();

	int level = block->level;

	uint64_t blockOffset = (uint64_t) block - (uint64_t) mem->startAddress;
	uint64_t buddyIndex = blockOffset / (1ULL << level);
	uint64_t buddyOffset = 0;

	if (buddyIndex % 2 == 0) {
		buddyOffset = blockOffset + (1ULL << level);
	}
	else {
		buddyOffset = blockOffset - (1ULL << level);
	}

	Block buddy = (Block) ((uint64_t) mem->startAddress + buddyOffset);

	if ((uint64_t) buddy < (uint64_t) mem->startAddress ||
		(uint64_t) buddy >= (uint64_t) mem->startAddress + mem->size) {
		addBlock(block);
		return;
	}

	if (buddy->isFree && buddy->level == level) {
		removeBlock(buddy);

		Block mergedBlock = (block < buddy) ? block : buddy;
		mergedBlock->level = level + 1;
//        memoryStats.usedMemory -= (1ULL << level);
//        memoryStats.freeMemory += (1ULL << level);
		mergeBlock(mergedBlock);
	}
	else {
		addBlock(block);
	}
}

static void addBlock(Block block) {
	memManager mem = getMemoryManager();
	int level = block->level;

	block->isFree = 1;
	block->next = mem->freeList[level];
	block->prev = NULL;

	if (mem->freeList[level] != NULL) {
		mem->freeList[level]->prev = block;
	}
	mem->freeList[level] = block;
}

static void removeBlock(Block block) {
	memManager mem = getMemoryManager();
	int level = block->level;

	if (block->prev != NULL) {
		block->prev->next = block->next;
	}
	else {
		mem->freeList[level] = block->next;
	}

	if (block->next != NULL) {
		block->next->prev = block->prev;
	}

	block->next = block->prev = NULL;
}

#endif