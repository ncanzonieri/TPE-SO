#include <time.h>
#include <stdint.h>
#include <keyboard2.h>
#include <videodriver.h>
#define TIMERTICK 0
#define KEYBOARD 1

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case TIMERTICK:
			int_20();
			break;
		case KEYBOARD: 
			int_21();
	}
	return;
}

void int_20() {
	timer_handler();
	char buffer[20]={0};
	printStringInCoord(0x00FFFFFF, getTime(buffer),0,0);
}

void int_21(){
	keyboard_handler();
}