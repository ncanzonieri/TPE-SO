
#include <time.h>
#include <commands.h>
#include <string.h>


#define MAX 15
#define MAX_DIM 256

#define DELETE 8
#define CERO 0
#define ESC 27
#define TAB 9
#define MAX_COMMANDS 9
#define GREEN 0x66FF66 // Font Scale
#define ERROR -1


void welcome();
void getCommands();
static void startShell(char * v);
static int belongs(char * v);
static void runCommands(int index);

static void (* runFuncts[])() = {registers, divx0, codOpInvalid, help, snake, time, snake, zoomIn, zoomOut};



#define DEFAULT_SCALE 1

void welcome(){
    clear();
    setFont(DEFAULT_SCALE);
    char * localDate = getDay();
    printf("Today's date is: %s\n", localDate);
}

void getCommands(){
    char v[MAX_DIM] = {0};
    char copy[MAX_DIM] = {0};
    int index = 0; 
    char c;
    while(1){
        putSteve(); // username
		while(1){
            if( c != ESC){ // 27
				v[index++] = c;
				callWrite(c);
            }
            else if( c == DELETE ){ // 8
				if( index > CERO)
					index--;
					callDel(); // systemcall a delete 
			} 
            else if(c == TAB){
                int tab = 4;
	            for( int i=0; i <tab; i++){
                    v[index++] = ' ';
			        callWrite(c);
		        }
            }else{
                v[index++] = c;
                callWrite(c);
            }
        }
		v[index] = 0;
        strcpy(v, copy);
		startShell(copy);
    }
}

static void startShell(char * v){
    if( v == 0){ // es raro este if
    }
    runCommands( belongs(v));
}


static int belongs(char * v){
    char * commands[MAX_COMMANDS] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "zoomIn", "zoomOut"};
    for( int i=0; i < MAX_COMMANDS; i++){
        if( strcmp(v, commands[i]) == 0){
            return i;
        }
    }
return -1;
}


static void runCommands(int index){
    if( index == ERROR){
        call_sysError(); // dps cambiamos las funciones, osea printf("Error not found")
    }
    runFuncts[index]; 

}

static void putSteve(){
    printString(GREEN, "steve $");
}