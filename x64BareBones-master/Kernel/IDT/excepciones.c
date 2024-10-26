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

static void dumpRegisters(){
    loadRegisters();
    uint64_t * registers = getRegisters();
    char buffer[17];

    for(int i = 0; i < regsAmount; i++){
        itoaHex(registers[i], buffer);
        int zeroDigits = 16 - strlen(buffer);

        sys_write(1, regsNames[i], strlen(regsNames[i]), WHITE); 
        sys_write(1, ": 0x", 4, WHITE);
        for(int j = 0; j < zeroDigits; j++){
            sys_write(1, "0", 1, WHITE);
        }
        sys_write(1, itoaHex(registers[i], buffer), strlen(buffer), WHITE);
        sys_write(1, "\n", 1, WHITE);
    }
}

static void launchingException(char * message){
    //en esta funcion tengo que setear la pantalla azul, letras blancas
    //hay que implementar libreria de syscalls 
    uint64_t oldBgColor = sys_getBgColor();
    sys_setBgColor(BLUE_SCREEN);
    uint8_t oldScale = getScale();
    setScale(2);
    sys_clearScreen();
	sys_write(1, message, strlen(message), WHITE);
    dumpRegisters();

    char * continueMessage = "\nPress any key to relaunch shell...";
    sys_write(1, continueMessage , strlen(continueMessage), WHITE);

    int readBytes = 0;
    char c;
    _sti();
    while(readBytes == 0){
        readBytes = sys_read(0, &c, 1);
    }

    // Old scale and color
    sys_setBgColor(oldBgColor);
    sys_clearScreen();
    setScale(oldScale);
}

void exceptionDispatcher(int excepcion){
    char * except_message;
    if (excepcion == EXCEPCION_cero_identificador){
        except_message = EXCEPTION_cero;
    }else if(excepcion == EXCEPTION_invalidOpcode){
        except_message = EXCEPCION_invalidOpcode_identificador;
    }
    launchingException(except_message);
}