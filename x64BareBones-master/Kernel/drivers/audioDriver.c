// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <audioDriver.h>

extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);

#define PIT_FREQUENCY 1193180

uint64_t playSound(uint32_t f) {
    uint32_t div = PIT_FREQUENCY / f;

    // Configure PIT
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t) (div));
    outb(0x42, (uint8_t) (div >> 8));

    // Play sound
    uint8_t aux = inb(0x61);
    if (aux != (aux | 3)) {
        outb(0x61, aux | 3);
    }
    return 1;
    
}

void stopSound() {
    uint8_t aux = inb(0x61) & 0xFC;
    outb(0x61, aux);
}