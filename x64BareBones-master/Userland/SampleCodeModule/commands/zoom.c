#include <commands.h>
#include <library.h>
#include <stdint.h>
// char * commands[MAX_COMMANDS] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "snake", "zoomIn", "zoomOut"};

//  --------------------- ZOOM IN & OUT -------------------------
#define MAX_ZOOM 3
#define MIN_ZOOM 1

void zoomIn(){
    return;
   /*
      int scale;
    callGetScale(&scale);
    if( scale >= MAX_ZOOM){
        printf("Maximum zoom out level reached.\n");
    }else{
        call_zoomIn();
        printf("Zoomed in to scale: %d\n", scale - 1);
    }
*/
}

void zoomOut(){
    return;
    /*
    int scale;
    callGetScale(&scale);

    if (scale <= MIN_ZOOM) {
        printf("Maximum zoom out level reached.\n");
    } else {

        call_zoomOut();
        printf("Zoomed out to scale: %d\n", scale - 1);
    }
    */
}


