#include <commands.h>
#include <library.h>
#include <stdint.h>
#include <syscalls.h>
// char * commands[MAX_COMMANDS] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "snake", "zoomIn", "zoomOut"};

//  --------------------- ZOOM IN & OUT -------------------------
#define MAX_ZOOM 3
#define MIN_ZOOM 1

static void zoomAux(int inc){
    uint64_t scale=sys_getFontInfo()+inc;
    if(scale > MAX_ZOOM || scale < MIN_ZOOM){
        clearIfNotEnoughSpace(1);
        printf("Already at the scale limit.\n");
    }else{
        sys_setFontScale(scale);
        clearIfNotEnoughSpace(1);
        printf("Now set to scale: %d\n", scale);
    }
    
}

void zoomIn(){
    zoomAux(1);
}

void zoomOut(){
    zoomAux(-1);
}

