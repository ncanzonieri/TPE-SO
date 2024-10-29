#include <commands.h>
#include <library.h>
#include <stdint.h>
#include <syscalls.h>
// char * commands[MAX_COMMANDS] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "snake", "zoomIn", "zoomOut"};

//  --------------------- ZOOM IN & OUT -------------------------
#define MAX_ZOOM 3
#define MIN_ZOOM 1

void zoomIn(){
    int scale;
    sys_setFontScale(scale);
    if( scale >= MAX_ZOOM){
        printf("Maximum zoom out level reached.\n");
    }else{
        sys_setFontScale(scale+1); // tendria aumentar
        printf("Zoomed in to scale: %d\n", scale - 1);
    }

}

void zoomOut(){
    return;
    int scale;
    sys_setFontScale(scale);
    if (scale <= MIN_ZOOM) {
        printf("Maximum zoom out level reached.\n");
    } else {
        sys_setFontScale(scale-1);
        printf("Zoomed out to scale: %d\n", scale - 1);
    }
}

