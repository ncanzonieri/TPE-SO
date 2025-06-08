#include <commands.h>
#include <syscalls.h>
#include <library.h>
#include <test_utils.h>

void* myMalloc(uint64_t size) {
    return sys_malloc(size);
}

void myFree(void* ptr) {
    sys_free(ptr);
}

int memoryDump(int argc, char* argv[]) {
    uint64_t* memoryInfo = (uint64_t*) sys_memoryDump();
    uint64_t totalMemory = memoryInfo[0];
    uint64_t usedMemory = memoryInfo[1];
    uint64_t freeMemory = memoryInfo[2];

    printf("Total Memory: %d bytes\n", totalMemory);
    printf("Used Memory: %d bytes\n", usedMemory);
    printf("Free Memory: %d bytes\n", freeMemory);
    return 0;
}