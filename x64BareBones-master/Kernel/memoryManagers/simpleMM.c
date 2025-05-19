#include "MemoryManagerADT.h"

typedef struct MemoryManagerCDT {
    char* startingAddress;
    char *nextAddress;
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    memoryManager->nextAddress = managedMemory;
    memoryManager->startingAddress = managedMemory;
    return memoryManager;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
    if(memoryToAllocate + (const size_t) memoryManager->nextAddress > MEM_FOR_MM + (const size_t) memoryManager->startingAddress){
        return 0;
    }
    char *allocation = memoryManager->nextAddress;

    memoryManager->nextAddress += memoryToAllocate;

    return (void *) allocation;
} 

/*
#ifndef BUDDY_SYSTEM

#include "memory_manager.h"
#include <stddef.h>
#include <stdint.h>

#define MEMORY_START 0x600000 // Después de SampleDataModule.bin
#define MEMORY_SIZE (0x100000) //
#define HEADER_SIZE sizeof(BlockHeader) // Tamaño del encabezado

// Encabezado de cada bloque de memoria
typedef struct BlockHeader {
    uint64_t size;      // Tamaño del bloque (sin incluir el encabezado)
    int is_free;        // 1 = libre, 0 = ocupado
    struct BlockHeader *next; // Puntero al siguiente bloque
} BlockHeader;

// Estructura del memory manager
typedef struct MemoryManagerCDT {
    void *start_address;    // Inicio de la memoria gestionada
    uint64_t total_memory;  // Tamaño total
    uint64_t used_memory;   // Memoria usada
    BlockHeader *first_block; // Primer bloque de la lista
} MemoryManagerCDT;

// Funciones internas
static void merge_free_blocks(MemoryManagerADT mm);

// Inicializa el memory manager
MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT mm = (MemoryManagerADT)memoryForMemoryManager;
    mm->start_address = (void *)MEMORY_START;
    mm->total_memory = MEMORY_SIZE;
    mm->used_memory = 0;

    // Inicializa el primer bloque (toda la memoria está libre)
    BlockHeader *initial_block = (BlockHeader *)MEMORY_START;
    initial_block->size = MEMORY_SIZE - HEADER_SIZE;
    initial_block->is_free = 1;
    initial_block->next = NULL;
    mm->first_block = initial_block;

    return mm;
}

// Aloca memoria
void *allocMemory(MemoryManagerADT const restrict mm, const size_t memoryToAllocate) {
    // Buscar el primer bloque libre que sea lo suficientemente grande
    BlockHeader *current = mm->first_block;

    while (!current) {
        if (current->is_free && current->size >= memoryToAllocate) {
            // Marcar como ocupado
            current->is_free = 0;
            mm->used_memory += memoryToAllocate;
            size_t oldSize = current->size;
            current->size = memoryToAllocate;
            size_t offset = HEADER_SIZE + memoryToAllocate;
            if(!current->next && current + offset < MEMORY_START + MEMORY_SIZE){
                current->next = current + offset;
                current->next->is_free = 1;
                current->next->size = oldSize - offset;
            }
            // Devolver la dirección después del encabezado
            return (void *)(current + 1);
        }
        current = current->next;
    }

    return NULL; // No hay bloques libres
}

if (current->size > memoryToAllocate + HEADER_SIZE) {
                BlockHeader *new_block = (BlockHeader *)((char *)current + HEADER_SIZE + memoryToAllocate);
                new_block->size = current->size - memoryToAllocate - HEADER_SIZE;
                new_block->is_free = 1;
                new_block->next = current->next;
                current->next = new_block;
                current->size = memoryToAllocate;
            }

// Libera memoria
void freeMemory(MemoryManagerADT const restrict mm, void *ptr) {
    if (ptr == NULL || ptr < mm->start_address || ptr >= (mm->start_address + mm->total_memory)) {
        return; // Puntero inválido
    }

    // Encontrar el encabezado (antes del puntero)
    BlockHeader *block = (BlockHeader *)ptr - 1;

    // Validar que sea un bloque válido
    if (block->is_free) {
        return; // Double-free
    }

    // Marcar como libre
    block->is_free = 1;
    mm->used_memory -= block->size;

    // Combinar bloques libres adyacentes
    merge_free_blocks(mm);
}

// Obtiene el estado de la memoria
void getMemoryStatus(MemoryManagerADT const restrict mm, memory_status_t *status) {
    status->total_memory = mm->total_memory;
    status->used_memory = mm->used_memory;
    status->free_memory = mm->total_memory - mm->used_memory;
}

// Combina bloques libres adyacentes
static void merge_free_blocks(MemoryManagerADT mm) {
    BlockHeader *current = mm->first_block;

    while (current != NULL && current->next != NULL) {
        if (current->is_free && current->next->is_free) {
            // Combinar bloques
            current->size += current->next->size + HEADER_SIZE;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

#endif

*/