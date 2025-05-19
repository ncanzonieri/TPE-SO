 /*
 #ifndef MEMORY_MANAGER_ADT_H
 #define MEMORY_MANAGER_ADT_H

 #include <stdint.h>
 #include <stddef.h>

 #define START_ADDRESS ((void*)0xF00000) // Dirección de inicio de la memoria a administrar
 #define MEMORY_SIZE 0x100000 // Tamaño total de la memoria (1 MB)

// Estructura para el encabezado de cada bloque de memoria
  
 typedef struct MemoryBlockHeader {
     size_t size;                         // Tamaño del bloque (sin incluir el header)
     uint8_t is_free;                     // 1 si está libre, 0 si está ocupado
     struct MemoryBlockHeader* next;      // Puntero al siguiente bloque
 } MemoryBlockHeader;

 typedef struct {
    size_t total;
    size_t used;
    size_t free;
    size_t blocks;
} MemoryInfo;


 uint64_t mm_init(void* start_addr, size_t size);


 void* mm_alloc(size_t size);

 int64_t mm_free(void* ptr);


MemoryInfo mm_info(void);

#endif /* MEMORY_MANAGER_ADT_H 
*/


#ifndef UNTITLED_MEMORY_MANAGER_H
#define UNTITLED_MEMORY_MANAGER_H

#include <stdint.h>

#define START_ADDRESS ((void*)0xF00000) // Dirección de inicio de la memoria a administrar
#define MEMORY_SIZE 0x100000 // Tamaño total de la memoria (1 MB)
#define MEM_SIZE 1048576
#define MIN_SIZE 32

typedef struct mem_info {
	uint64_t total_mem;
	uint64_t used_mem;
	uint64_t free_mem;
} mem_info_t;

void mm_init(void *m, uint32_t s);
void *mm_alloc(uint32_t size);
void mm_free(void *ptr);
mem_info_t *mem_dump();

#endif // UNTITLED_MEMORY_MANAGER_H
