
#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

uint64_t printStringInCoord(uint32_t hexColor, char* s, uint64_t x, uint64_t y);
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void printByte(uint32_t hexColor, uint8_t string, uint64_t x, uint64_t y);
void printBitMap(uint32_t hexColor, uint8_t map[], uint64_t x, uint64_t y);
void printCharacter(uint32_t hexColor, char c, uint64_t x, uint64_t y);
uint64_t printStringLength(uint32_t hexColor, char* s, uint64_t count);
uint64_t printString(uint32_t hexColor, char* s);
void printInt(int num);
void printHex(uint64_t value);
void nextBlank();
void newLine();
uint64_t getCoords();
void clearScreen();
void setBGColor(uint32_t hexColor);
uint64_t getBGcolor();
uint8_t setScale(uint8_t newScale);
uint64_t getScale();
void drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
uint64_t getScreenSize();

#endif
