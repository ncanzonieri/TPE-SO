//manejo de excepciones 

//me va a tener que mostrar pantalla azul 
//si se produce alguna de las dos excepciones pedidas

//TPE DICE: El kernel deberá poder manejar dos tipos de excepciones:
//división por cero y código de
//operación inválido. Ante estas excepciones se deberá 
//desplegar información sobre el
//tipo de error, instruction pointer y registros en 
//el momento del error. El sistema debe
//poder recuperarse posteriormente de la excepción, regresando a la Shell. 

#include <stdint.h>
#include <string.h>
#include <videodriver.h>
#include <interrupts.h>
#include <sysCalls.h>

#define EXCEPTION_cero  "ERROR 0x00 Division by zero exception\n\n"
#define EXCEPTION_invalidOpcode "ERROR 0x06 Invalid Opcode exception\n\n"
#define EXCEPCION_cero_identificador 0
#define EXCEPCION_invalidOpcode_identificador 6
#define WHITE 0x00FFFFFF
#define BLUE_SCREEN 0x0000007B //color de background 

static int regsAmount = 17;
static void dumpRegisters();
static void launchingException(char * msg);

extern void loadRegisters();
extern uint64_t * getRegisters();

static char * regsNames[] = {  //mismo orden que en FILLSNAPSHOT (interrupts.asm)
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   "
};

static void hexaToAscii(uint64_t num, char* buffer){
    int rest;
    char aux[66]={'0'};
    for(int i=0; num>0; i++, num/=16){
        rest=num%16;
        aux[i]= (rest < 10 )? (rest + '0') : (rest - 10 + 'A');
    }
    aux[64]='x';
    aux[65]='0';
    for(int i=0; i<66; i++){
        buffer[i]=aux[65-i];
    }
}

static void dumpRegisters(){
    loadRegisters();
    uint64_t * registers = getRegisters();
    char buffer[66];

    for(int i = 0; i < regsAmount; i++){
        hexaToAscii(registers[i], buffer);
        int zeroDigits = 16 - strlen(buffer);

        printString(WHITE, regsNames[i]); 
        printString(WHITE, ": 0x");
        for(int j = 0; j < zeroDigits; j++){
            printString(WHITE, "0");
        }
        printString(WHITE, buffer);
        newLine();
    }
}

static void launchingException(char * message){
    //en esta funcion tengo que setear la pantalla azul, letras blancas
    //hay que implementar libreria de syscalls 
    uint64_t oldBgColor = getBGcolor();
    setBGColor(BLUE_SCREEN);
    uint8_t oldScale = getScale();
    setScale(2);
    clearScreen();
	printString(WHITE, message);
    dumpRegisters();

    char * continueMessage = "\nPress any key to relaunch shell...";
    printString(WHITE, continueMessage);

    int readBytes = 0;
    uint8_t * c={0};
    _sti();
    while(readBytes == 0){
        readBytes = sys_read(0, c, 1);
    }

    // Old scale and color
    setBGColor(oldBgColor);
    clearScreen();
    setScale(oldScale);
}

void exceptionDispatcher(int excepcion){
    char * except_message;
    if (excepcion == EXCEPCION_cero_identificador){
        except_message = EXCEPTION_cero;
    }else if(excepcion == EXCEPCION_invalidOpcode_identificador){
        except_message = EXCEPTION_invalidOpcode;
    }
    launchingException(except_message);
}

int strlen(const char * s) {
    int i = 0;
    while(s[i] != '\0') {
        if(s[i] == '\t') {
            i+=4;
        }
        i++;
    }
    return i;
}