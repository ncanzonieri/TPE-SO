#include <commands.h>
#include <library.h>
#include <syscalls.h>
#define WHITE 0xffffff
#define GREEN 0x66FF66
#define STDOUT_FD 1

#define INSTRUCTIONS 16
void help(){
    char * helpingInstructions [] ={"------------------- Help -------------------\n", 
    "COMMANDS:\n",
    "\tregisters\t\t\tDisplay the current register values.\n",
    "\tsnake\t\t\tPlay the snake game.\n",
    "\tzoomIn\t\t\tScales up.\n",
    "\tzoomOut\t\t\tScales down.\n",
    "\ttime\t\t\tActual time in Buenos Aires\n",
    "\tdate\t\t\tActual date in Buenos Aires\n",
    "\tagro\t\t\tAgropecuario Carlos Casares\n",
    "\tclear\t\t\tClears the screen\n",
    "\ttestMM\t\t\tTest the memory manager\n",
    "\tmem\t\t\tPrints the memory state\n",
    "EXCEPTIONS:\n",
    "\tdivx0\t\t\tHandles division by zero exception.\n",
    "\tinvalid\t\t\tHandles invalid operation codes.\n",
    "----------------------------------------------\n"
    };
    clearIfNotEnoughSpace(INSTRUCTIONS);
    for(int i = 0; i<INSTRUCTIONS; i++){
        sys_write(STDOUT_FD,helpingInstructions[i],strlen(helpingInstructions[i])+1, WHITE);
    }
 
}