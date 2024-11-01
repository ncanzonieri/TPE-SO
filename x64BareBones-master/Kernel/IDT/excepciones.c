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
#include <videoDriver.h>
#include <interrupts.h>
#include <sysCalls.h>

#define EXCEPTION_cero  "ERROR 0x00 Division by zero exception\n\n\n"
#define EXCEPTION_invalidOpcode "ERROR 0x06 Invalid Opcode exception\n\n\n"
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
    char rest;
    for(int i=0; i<64; i++, num/=16){
        rest=num%16;
        buffer[65-i]= (rest < 10 )? (rest + '0') : (rest - 10 + 'A');
    }
    buffer[1]='x';
    buffer[0]='0';
}

static void dumpRegisters(){
    loadRegisters();
    uint64_t * registers = getRegisters();
    char buffer[66];

    for(int i = 0; i < regsAmount; i++){
        hexaToAscii(registers[i], buffer);
        printString(WHITE, regsNames[i]); 
        printString(WHITE, ": ");
        printStringLength(WHITE, buffer,66);
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
    setScale(1);
    dumpRegisters();
    setScale(2);

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

