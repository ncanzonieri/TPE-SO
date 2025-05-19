#include <commands.h>
#include <syscalls.h>

void* myMalloc(uint64_t size) {
    return sys_malloc(size);
}

void myFree(void* ptr) {
    sys_free(ptr);
}