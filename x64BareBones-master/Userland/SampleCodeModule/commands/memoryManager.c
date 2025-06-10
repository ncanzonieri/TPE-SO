// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <syscalls.h>
#include <library.h>
#include <test_utils.h>

void* myMalloc(uint64_t size) {
    return (void *) sys_malloc(size);
}

void myFree(void* ptr) {
    sys_free((uint64_t) ptr);
}

int memoryDump(int argc, char* argv[]) {
    uint64_t* memoryInfo = (uint64_t*) sys_memoryDump();
    uint64_t totalMemory = memoryInfo[0];
    uint64_t usedMemory = memoryInfo[1];
    uint64_t freeMemory = memoryInfo[2];
    printf("%s\n", memoryInfo[3] ? "Buddy system" : "Simple system");
    printf("Total Memory: %p bytes\n", totalMemory);
    printf("Used Memory:  %p bytes\n", usedMemory);
    printf("Free Memory:  %p bytes\n", freeMemory);
    return 0;
}