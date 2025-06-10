// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY
#include <MemoryManagerADT.h>
#include <lib.h>
#include <videoDriver.h>
#include <stddef.h>

typedef struct buddyBlock {
	struct buddyBlock *left;
	struct buddyBlock *right;
	void *address;
	uint64_t size;
	uint32_t status;
	uint32_t index;
} buddyBlock_t;

typedef enum { EMPTY = 0,
			   FULL,
			   SPLIT } status_t;

typedef buddyBlock_t *node_t;

#define IS_POWER_OF_2(x) (!((x) & ((x) - 1)))

static void *mallocRec(node_t node, uint32_t size);
static uint8_t hasNoChildren(node_t node);
static void freeRec(node_t node, void *ptr);
static void setBlockState(node_t node);
static void setChildren(node_t node);

static node_t memoryManager;
static uint64_t usedMemory = 0;
static memoryStats_t memoryStats;

void createMemoryManager(void *startAddress, uint32_t memorySize) {
	memoryManager = (node_t) MM_ADDRESS;
	memoryManager->address = startAddress;
	memoryManager->index = 0;
	memoryManager->size = memorySize;
	memoryManager->status = EMPTY;
	memoryManager->left = NULL;
}

void *myMalloc(uint32_t blockSize) {
	if (blockSize < MIN_SIZE)
		blockSize = MIN_SIZE;
	else if (blockSize > memoryManager->size) {
		return NULL;
	}
	if (!IS_POWER_OF_2(blockSize)) {
		blockSize = align(blockSize);
	}
	return mallocRec(memoryManager, blockSize);
}

static void *mallocRec(node_t node, uint32_t size) {
	if (node->status == FULL) {
		return NULL;
	}
	if (node->left != NULL || node->right != NULL) {
		void *aux = mallocRec(node->left, size);
		if (aux == NULL) {
			return mallocRec(node->right, size);
		}
		setBlockState(node);
		return aux;
	}
	else {
		if (size > node->size) {
			return NULL;
		}
		if ((node->size / 2) >= size) {
			setChildren(node);
			void *ans = mallocRec(node->left, size);
			setBlockState(node);
			return ans;
		}
		node->status = FULL;
		usedMemory += size;
		return node->address;
	}
}

static void setBlockState(node_t node) {
	if (hasNoChildren(node)) {
		node->status = EMPTY;
	}
	else if (node->left->status == FULL && node->right->status == FULL) {
		node->status = FULL;
	}
	else if (node->left->status == FULL || node->right->status == FULL ||
			 node->left->status == SPLIT || node->right->status == SPLIT) {
		node->status = SPLIT;
	}
	else {
		node->status = EMPTY;
	}
}

static uint8_t hasNoChildren(node_t node) {
	return (node->left == NULL || node->right == NULL);
}

static void setChildren(node_t node) {
	uint32_t parentIdx = node->index;
	uint32_t leftIdx = parentIdx * 2 + 1;
	uint64_t childrenSize = ((uint64_t) (node->size) / 2);
	node->left = node + leftIdx;

	if ((uint64_t) node->left >= START_MM) {
		return;
	}
	node->left->index = leftIdx;
	node->left->size = childrenSize;
	node->left->address = node->address;
	node->left->status = EMPTY;

	unsigned int rightIdx = leftIdx + 1;
	node->right = node + rightIdx;
	if ((uint64_t) node->right >= START_MM) {
		return;
	}
	node->right->index = rightIdx;
	node->right->size = childrenSize;
	node->right->address = (void *) ((uint64_t) (node->address) + childrenSize);
	node->right->status = EMPTY;
}

void myFree(void *ptr) {
	freeRec(memoryManager, ptr);
}

static void freeRec(node_t node, void *ptr) {
	if (node == NULL) {
		return;
	}
	if (node->left != NULL || node->right != NULL) {
		if (node->right != 0 && (uint64_t) node->right->address > (uint64_t) ptr) {
			freeRec(node->left, ptr);
		}
		else {
			freeRec(node->right, ptr);
		}
		setBlockState(node);
		if (node->status == EMPTY) {
			node->right = NULL;
			node->left = NULL;
		}
	}
	else if (node->status == FULL) {
		if (node->address == ptr) {
			node->status = EMPTY;
			usedMemory -= node->size;
		}
	}
	return;
}

memoryStats_t *memDump(){
	memoryStats.usedMemory = usedMemory;
	memoryStats.totalMemory = memoryManager->size;
	memoryStats.freeMemory = memoryManager->size - usedMemory;
    memoryStats.isBuddy = 1;
    return &memoryStats;
}
#endif