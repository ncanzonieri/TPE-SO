#include <getCurrentTime.h>
#include <time.h>
#include <commands.h>

#include <string.h>
#define MAX 15



// Hay que acomodarlo bien pero dps lo hacemos



static void homeScreen(){
    clear();

}


#define GREEN 0x66FF66

void welcome(){
    char * localDate = getDay();
    printf("Today's date is: %s\n", localDate);
    printf("Enter 'help' to see the help options.");
    printString(GREEN, "Steve $"); // call syscall write
    
}


#define MAX_DIM 256
#define DELETE 8
#define CERO 0
#define ESC 27
#define TAB 9
#define MAX_COMMANDS 9

// ---------------- COMMANDS ----------------------

void (* runFuncts[])() = {registers, divx0, codOpInvalid, help, snake, time, snake, zoomIn, zoomOut};

// ----------------GET, Start - SHELL -----------------------


void getCommands(){
    char c;
    while(1){
        char v[MAX_DIM] = {0};
        int index = 0; 
		while( (c = getchar()) != '\n'){
			if( c != ESC){
				if( c == TAB){
					int tab = 4;
					for( int i=0; i <tab; i++){
						v[index++] = ' ';
						callWrite(c);
					}
				}
				v[index++] = c;
				callWrite(c);
			}if( c == DELETE ){
				if( index > CERO)
					index--;
					callDel(); // systemcall a delete 
			} 
		}
		v[index] = 0;
		startShell(v);
    }
}

#define ERROR -1

void startShell(char * v){
    if( v == 0){ // es raro este if
    }
    runCommands( belongs(v));
}


int belongs(char * v){
    char * commands[MAX_COMMANDS] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "snake", "zoomIn", "zoomOut"};
    for( int i=0; i < MAX_COMMANDS; i++){
        if( strcmp(v, commands[i]) == 0){
            return i;
        }
    }
return -1;
}


void runCommands(int index){
    if( index == ERROR){
        call_sysError(); // dps cambiamos las funciones, osea printf("Error not found")
    }
    runFuncts[index]; 

}