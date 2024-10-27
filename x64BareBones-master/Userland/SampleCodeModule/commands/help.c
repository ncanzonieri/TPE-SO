#include <commands.h>
#include <library.h>

void help(){


    printf("------------------- Help -------------------\n");
    printf("COMMANDS:\n");
    printf("\tRegisters\t\t\tDisplay the current register values.\n");
    printf("\tExceptions:\n");
    printf("\t\tdivx0\t\t\t\tHandles division by zero exception.\n");
    printf("\t\tcodOpInvalid\t\t\tHandles invalid operation codes.\n");
    printf("\tGame Commands:\n");
    printf("\t\tsnake\t\t\t\tPlay the snake game.\n");
    printf("\t\tzoomIn\t\t\t\tZoom in the game view.\n");
    printf("\t\tzoomOut\t\t\t\tZoom out the game view.\n");
    printf("----------------------------------------------\n");

}