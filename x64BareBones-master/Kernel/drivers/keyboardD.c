#include <stdint.h>

#define BUFFER_DIM 256 
#define REGISTERS_DIM 16
// los define van en el .h dps lo acomodamos 
// ----------------- DEFINES ------------------
#define ESC 27
#define CTRL 17
#define ENTER '\n'
#define TAB '\t'
#define DELETE '\b'
#define SPACE ' '
#define KEYBOARD_DIM 59
#define RIGHT_SHIFT 0
#define LEFT_SHIFT 0
#define LEFT_ALT 0
// --------------- MORE DEFINES ---------------
//---------- Pressed ------------
#define ESC_P 1
#define CTRL_P 29
#define LEFT_SHIFT_P 42
#define LEFT_ALT_P 56
#define RIGHT_SHIFT_P 54
#define CAPS_LOCK_P 58
// -------- Released ------------
#define RELEASED 59
#define LEFT_SHIFT_R 170
#define LEFT_ALT_R 184
#define RIGHT_SHIFT_R 182



// ------------- FLAGS ----------------
static char isShiftP = 0; // shift pressed
static char isAltP = 0; // alt pressed
static char isCapsLockP = 0; // caps lock pressed
static char isCTRLP = 0; // CTRL pressed

// podriamos hacer una struct para estos 4 vars
// buffer para las teclas quw entran
// o lo dejamos asi?
    static char read = 0;
    static char write = 0;
    static char bufferDim=0; 
    static char buffer[BUFFER_DIM];


extern u_int8_t scanKey();
extern u_int64_t registers();




static char keyboardM[KEYBOARD_DIM][2] = {
    {0, 0}, {ESC, ESC}, {'1', '!'}, {'2', '@'}, {'3', '#'},
    {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'},
    {'9', '('}, {'0', ')'}, {'-', '_'}, {'-', '+'}, {DELETE, DELETE}, 
    {TAB, TAB},
    {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'},
    {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'},
    {'p', 'P'}, {'[', '{'}, {']', '}'}, 
    {ENTER, ENTER}, {CTRL, CTRL}, 
    {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'},
    {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'},
    {'\'', '\"'}, {167, '~'}, {LEFT_SHIFT, LEFT_SHIFT}, // left shift
    {'\\', '|'}, {'z', 'Z'},
    {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'},
    {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, 
    {RIGHT_SHIFT, RIGHT_SHIFT}, 
    {0, 0}, // (keypad) * pressed
    {LEFT_ALT, LEFT_ALT},
    {SPACE, SPACE} 
};

static void checkKeyP(u_int8_t key){
    if (key == LEFT_ALT_P) {
        isAltP = 1;
    } else if (key == CTRL_P) {
        return;
    } else if (key == LEFT_ALT_P || key == RIGHT_SHIFT_P) {
        isShiftP = 1; 
    } else if (key == CAPS_LOCK_P) {
        isCapsLockP = 0; // toggle caps lock // antes era 1 - isCapsLockP
    }
}

static void checkKeyR(uint8_t key){
    if (key >= RELEASED)
        return;
    else if (key == LEFT_ALT_R || key == RIGHT_SHIFT_R)
        isShiftP = 0; // apago flag
    else if (key == LEFT_ALT_R) 
        isAltP = 0; // apago flag alt pressed
    
    
}

static char flagRegister = 0;

void keyboardHanlder(){ // lo llama desde IrqKeyboard (IDT)
    u_int8_t key = ScanKey(); // esta se hace en asm, 
    checkKeyP(key);
    checkKeyR(key);
    if( isCTRLP && ( toAscii(key) == 'p') ){
        callRegisters(flagRegister); 
    }
}

// ----------- PRINT REGISTERS --------------
// Se deberá disponer de una funcionalidad que permita obtener el valor de los registros
// del procesador en cualquier momento que se desee.
static void callRegisters(char flagRegister){
    registers(); // tenemos ver como vamos a usar los registros en asm para ver dps como usar el for
    for( int i=0; i<REGISTERS_DIM; i++){

    }
}