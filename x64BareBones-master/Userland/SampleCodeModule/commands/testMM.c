#include <stdio.h>
#include <stdint.h>
#include "../tests/test_mm.h"

void testMM(){
    uint64_t argc;
    char *argv[1];
    argv[0] = "100000";
    argc = 1;
    test_mm(argc, argv);
    return;
}