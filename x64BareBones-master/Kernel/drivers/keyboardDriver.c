#include <keyboardDriver.h>
#include <sysCalls.h>
#include <scheduler.h>

extern uint8_t getKeyCode();
extern void loadRegisters();
extern uint64_t * getRegisters();


static char convertScancode(uint8_t scancode);
static void handleSpecialKeys(uint8_t scancode);
static int isBufferEmpty();
static char insertCharIntoBuffer(char c);
static char removeCharFromBuffer();
static int isBufferFull();
//static void updateRegisters();

/*
 * Keycode matrix:
 * in index zero the char without shift, in index 1 the char with shift
 * 0 if the key doesn't have a char representation
 */
static char keycodeMap[KEYS_AMOUNT][2] = {
        {0, 0}, {ESC, ESC}, {'1', '!'}, {'2', '@'}, {'3', '#'},
        {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'},
        {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'},
        {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'},
        {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'},
        {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0},
        {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'},
        {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'},
        {'\'', '\"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'},
        {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'},
        {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0},
        {0, 0}, {' ', ' '}
};

// Circular buffer for keyboard input
#define BUFFER_SIZE 256

typedef struct TBuffer {
    char data[BUFFER_SIZE];
    int readIndex;
    int writeIndex;
    int count;
} TBuffer;

static TBuffer buffer = { .readIndex = 0, .writeIndex = 0, .count = 0 };

static struct {
    uint8_t shift : 1;
    uint8_t capsLock : 1;
    uint8_t ctrl : 1;
} keyFlags = {0, 0, 0};

static volatile uint64_t cpuRegisters[REGS_AMOUNT];
static uint64_t registersAvailable = 0;

int64_t keyboard_handler() { // lo llama desde IrqKeyboard (IDT)
    uint8_t scancode = getKeyCode();
    handleSpecialKeys(scancode);
    char asciiChar = convertScancode(scancode);

    if(keyFlags.ctrl /*&& (asciiChar == 'r' || asciiChar == 'R')*/) {
        if(asciiChar == 'r' || asciiChar == 'R') {
            registersAvailable = 1;
            return registersAvailable;
        } else if (asciiChar == 'c' || asciiChar == 'C') {
            _sys_write(1, "^C", 2, 0x00FFFFFF);
            killForegroundProcess();
        }else if (asciiChar == 'd' || asciiChar == 'D') {
            _sys_write(1, "^D", 2, 0x00FFFFFF);
            // send eof
        }
        
        //loadRegisters();
    } else if (asciiChar) {
        insertCharIntoBuffer(asciiChar);
    }
    return 0;
    
}

// Obtiene un carácter del buffer del teclado
char getKeyboardChar() {
    return removeCharFromBuffer();
}

// Maneja el estado de las teclas especiales
static void handleSpecialKeys(uint8_t scancode) {
    switch(scancode) {
        case LCTRL:
            keyFlags.ctrl = 1;
            break;
        case LCTRL_RELEASE:
            keyFlags.ctrl = 0;
            break;
        case LSHIFT: case RSHIFT:
            keyFlags.shift = 1;
            break;
        case LSHIFT + RELEASE_OFFSET: case RSHIFT + RELEASE_OFFSET:
            keyFlags.shift = 0;
            break;
        case CAPSLOCK:
            keyFlags.capsLock ^= 1; // Alterna el estado de Caps Lock
            break;
        default:
            break;
    }
}

static char convertScancode(uint8_t scancode) {
    if (scancode >= KEYS_AMOUNT) return 0;

    char asciiChar = keycodeMap[scancode][keyFlags.shift];
    
    if (keyFlags.capsLock && asciiChar >= 'a' && asciiChar <= 'z') {
        asciiChar -= 32; // Convierte a mayúscula si Caps Lock está activo
    }
    return asciiChar;
}

static int isBufferFull() {
    return buffer.count == BUFFER_SIZE;
}

static int isBufferEmpty() {
    return buffer.readIndex == buffer.writeIndex;
}

static char insertCharIntoBuffer(char c) {
    if (isBufferFull()) return 0;
    buffer.data[buffer.writeIndex] = c;
    buffer.writeIndex = (buffer.writeIndex + 1) % BUFFER_SIZE;
    buffer.count++;
    return 1;
}

static char removeCharFromBuffer() {
    if (isBufferEmpty()) return 0;
    char c = buffer.data[buffer.readIndex];
    buffer.readIndex = (buffer.readIndex + 1) % BUFFER_SIZE;
    buffer.count--;
    return c;
}

//void updateRegisters() {
//    uint64_t * r = getRegisters();
//    for(int i = 0; i < REGS_AMOUNT; i++) {
//        registers[i] = r[i];
//    }
//}

uint64_t putRegisters(uint64_t * destination) {
      if(!registersAvailable) return 0;
    uint64_t* srcRegisters = getRegisters();
    for(int i = 0; i < REGS_AMOUNT; i++) {
        destination[i] = srcRegisters[i];
    }
    return 1;
}