// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <library.h>
#include <stdint.h>
#include <syscalls.h>


//  --------------------- ZOOM IN & OUT -------------------------
#define MAX_ZOOM 3
#define MIN_ZOOM 1

static void zoomAux(int inc){
    uint64_t scale=sys_getFontInfo()+inc;
    if(scale > MAX_ZOOM || scale < MIN_ZOOM){
        clearIfNotEnoughSpace(1);
        printf("Escala ya esta al limite.\n");
    }else{
        sys_setFontScale(scale);
        clearIfNotEnoughSpace(1);
        printf("Nueva escala: %d\n", scale);
    }
    
}

int zoom(int argc, char* argv[]){
    if(argc != 1){
        printf("Uso correcto: zoom <in|out>\n");
        return 1;
    }
    if(strcmp(argv[0], "in") == 0){
        zoomAux(1);
    }else if(strcmp(argv[0], "out") == 0){
        zoomAux(-1);
    }else{
        printf("Uso correcto: zoom <in|out>\n");
        return 1;
    }
    return 0;
}