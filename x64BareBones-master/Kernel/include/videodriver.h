#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void printByte(uint32_t hexCollor, uint8_t string, uint64_t x, uint64_t y);
void printBitMap(uint32_t hexCollor, uint8_t map[], uint64_t x, uint64_t y);
void printCharacter(uint32_t hexCollor, char c, uint64_t x, uint64_t y);
void printString(uint32_t hexCollor, char* s);
void nextBlank();
void newLine();

#endif
