#include <commands.h>
#include <library.h>
#define GREEN 0x66FF66
#define STDOUT_FD 1

#define INSTRUCTIONS 13
void help(){
    char * helpingInstructions [INSTRUCTIONS] ={"------------------- Help -------------------\n", 
    "COMMANDS:\n",
    "\tregisters\t\t\tDisplay the current register values.\n",
    "\tExceptions:\n",
    "\t\tdivx0\t\t\t\tHandles division by zero exception.\n",
    "\t\tInvalid\t\t\tHandles invalid operation codes.\n",
    "\tGame Commands:\n",
    "\t\tsnake\t\t\t\tPlay the snake game.\n",
    "\t\tzoomIn\t\t\t\tZoom in the game view.\n",
    "\t\tzoomOut\t\t\t\tZoom out the game view.\n",
    "\t\tactualTime\t\t\t\tActual time in Buenos Aires",
    "\t\tAgro\t\t\t\tAgropecuario Carlos Casares\n",
    "----------------------------------------------\n"
     };

    for(int i = 0; i<INSTRUCTIONS; i++){
        sys_write(STDOUT_FD,helpingInstructions[i],strlen(helpingInstructions[i]), GREEN);
    }
 
}