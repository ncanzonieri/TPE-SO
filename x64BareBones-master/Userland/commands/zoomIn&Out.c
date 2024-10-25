#include <commands.h>

// char * commands[MAX_COMMANDS] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "snake", "zoomIn", "zoomOut"};


//  --------------------- ZOOM IN & OUT -------------------------


#define MAX_ZOOM 3
#define MIN_ZOOM 1


void zoomIn(){
    int scale;
    callGetScale(&scale);
    if( scale >= MAX_ZOOM){
        printf("Maximum zoom out level reached.\n");
    }else{
        call_zoomIn();
        print("Zoomed in to scale: %d\n", scale - 1);
    }

}

void zoomOut(){
    int scale;
    callGetScale(&scale);

    if (scale <= MIN_ZOOM) {
        printf("Maximum zoom out level reached.\n");
    } else {

        call_zoomOut();
        printf("Zoomed out to scale: %d\n", scale - 1);
    }
}


