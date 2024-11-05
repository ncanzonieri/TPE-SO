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

#define EXCEPCION_cero_identificador 0
#define EXCEPCION_invalidOpcode_identificador 6
#define WHITE 0x00FFFFFF
#define BLUE_SCREEN 0x0000007B //color de background 
static char* errorMessages[2]={"ERROR 0x00 Division by zero exception\n\n\n","ERROR 0x06 Invalid Opcode exception\n\n\n"};
static int regsAmount = 20;
static void dumpRegisters();
static void launchingException(char * msg);

extern void loadRegisters();
extern uint64_t * getRegisters();

static char * regsNames[] = {  
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   ", "SS    ", "CS    ", "RFLAGS"
};

static void hexaToAscii(uint64_t num, char* buffer){
    char rest;
    for(int i=0; i<16; i++, num/=16){
        rest=num%16;
        buffer[17-i]= (rest < 10 )? (rest + '0') : (rest - 10 + 'A');
    }
    buffer[1]='x';
    buffer[0]='0';
}

static void dumpRegisters(){
    uint64_t * registers = getRegisters();
    char buffer[18];

    for(int i = 0; i < regsAmount; i++){
        hexaToAscii(registers[i], buffer);
        printString(WHITE, regsNames[i]); 
        printString(WHITE, ": ");
        printStringLength(WHITE, buffer,18);
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
    registersFilled=1;
    setScale(1);
    dumpRegisters();
    setScale(2);

    printString(WHITE, "\nPress any key to relaunch shell...");

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
    char * except_message=errorMessages[excepcion/6];
    launchingException(except_message);
}

