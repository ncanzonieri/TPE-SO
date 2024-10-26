// font.h

#ifndef FONT_H
#define FONT_H

#include <stdint.h>
/*
#define FONT_WIDTH  8       
#define FONT_HEIGHT 16       
#define ASCII_MIN   32      // ' '
#define ASCII_MAX   126     // '~'
*/

uint8_t getWidth(void);
uint8_t getHeight(void);
uint8_t* getFontChar(char c);

#endif // FONT_H

