// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <videoDriver.h>
#include <fontDriver.h>
#include <stdint.h>
#include <stdint.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

static uint64_t coordenadaX=0;
static uint64_t coordenadaY=16;
static uint32_t bgColor=0x00000000;
static uint8_t scale=1;

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *)(uintptr_t)VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void printScaledPixel(uint32_t hexColor, uint64_t x, uint64_t y){
	for(int i=0; i<scale; i++){
		for(int j=0; j<scale; j++){
			putPixel(hexColor, x+i, y+j);
		}
	}
}

void printByte(uint32_t hexCollor, uint8_t string, uint64_t x, uint64_t y){
	uint8_t base = 1;
	for(int i=7*scale; i>=0; i-=scale, base=base<<1){
		printScaledPixel((string & base)>0 ? hexCollor : bgColor, x+i, y);
	}
}
 
void printBitMap(uint32_t hexCollor, uint8_t map[], uint64_t x, uint64_t y){
	for(int i=0; i<=15*scale; i+=scale){
		printByte(hexCollor,map[i/scale],x,y+i);
	}
}

void printCharacter(uint32_t hexCollor, char c, uint64_t x, uint64_t y){
	printBitMap(hexCollor, getFontChar(c), x, y);
}

uint64_t printStringInCoord(uint32_t hexCollor, char* s, uint64_t x, uint64_t y){
	uint64_t auxX=coordenadaX;
	uint64_t auxY=coordenadaY;
	coordenadaX=0;
	coordenadaY=0;
	uint64_t ans =printString(hexCollor,s);
	coordenadaX=auxX;
	coordenadaY=auxY;
	return ans;
}

uint64_t printStringLength(uint32_t hexCollor, char* s, uint64_t count){
	uint64_t counter=0;
	for(;*s != 0 && counter<count; s++, counter++){
		switch(*s){
			case '\n':
				newLine();
				break;
			case '\t':
				if(coordenadaX+ 4*8 >= VBE_mode_info->width){
					newLine();
				}else{
					for(int i=0; i<4; i++){
						printCharacter(hexCollor, ' ', coordenadaX, coordenadaY);
						nextBlank();
					}
				}
				break;
			case '\b':
				if(coordenadaX<8*scale){
					if(coordenadaY>=16*scale){
						coordenadaY-=16*scale;
						coordenadaX=VBE_mode_info->width-8*scale;
						coordenadaX-=coordenadaX%(8*scale);
					}
				}else{
					coordenadaX-=8*scale;
				}
				printCharacter(hexCollor, ' ', coordenadaX, coordenadaY);
				break;
			default:
				printCharacter(hexCollor, *s, coordenadaX, coordenadaY);
				nextBlank();
				break;
		}
	}
	return counter;
}

uint64_t printString(uint32_t hexCollor, char* s){
	uint64_t counter=0;
	while(*s != 0){
		switch(*s){
			case '\n':
				newLine();
				break;
			case '\t':
				if(coordenadaX+ 4*8 >= VBE_mode_info->width){
					newLine();
				}else{
					for(int i=0; i<4; i++){
						printCharacter(hexCollor, ' ', coordenadaX, coordenadaY);
						nextBlank();
					}
				}
				break;
			case '\b':
				if(coordenadaX<8*scale){
					if(coordenadaY>=16*scale){
						coordenadaY-=16*scale;
						coordenadaX=VBE_mode_info->width-8*scale;
						coordenadaX-=coordenadaX%(8*scale);
					}
				}else{
					coordenadaX-=8*scale;
				}
				printCharacter(hexCollor, ' ', coordenadaX, coordenadaY);
				break;
			default:
				printCharacter(hexCollor, *s, coordenadaX, coordenadaY);
				nextBlank();
				break;
		}
		s++;
		counter++;
	}
	return counter;
}

void printInt(int num) {
    char buffer[256] = {0};
    int i = 0;
    if (num == 0) {
        buffer[i++] = '0';
    } else if (num < 0) {
        buffer[i++] = '-';
        num = -num;
    }
    while (num > 0) {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    }
    buffer[i] = 0;
	int length = i;
    i--;
    int j = 0;
    while (i > j) {
        char aux = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = aux;
        i--;
        j++;
    }
    printString(0xFFFFFF, buffer);
    coordenadaX += (length) * 8 * scale;
    if (coordenadaX >= VBE_mode_info->width) {
        newLine();
    }
}

void printHex(uint64_t value) {
    char buffer[256] = {0};
    int i = 0;
    buffer[i++] = '0';
    buffer[i++] = 'x';
    if (value == 0) {
        buffer[i++] = '0';
    } else {
        while (value > 0) {
            uint64_t nibble = value & 0xF;
            buffer[i++] = (nibble < 10) ? (char)('0' + nibble) : (char)('A' + (nibble - 10));
            value >>= 4;
        }
    }
    buffer[i] = 0;
    int start = 2;
    int end = i - 1;
    while (end > start) {
        char aux = buffer[end];
        buffer[end] = buffer[start];
        buffer[start] = aux;
        end--;
        start++;
    }
    buffer[i + 1] = '\n';
    buffer[i + 2] = 0;
    printString(0xFFFFFF, buffer);
    coordenadaX += (i + 2) * 8 * scale;
    if (coordenadaX >= VBE_mode_info->width) {
        newLine();
    }
}

void nextBlank(){
	if(coordenadaX+8*scale >= VBE_mode_info->width){
		newLine();
	}else{
		coordenadaX+=8*scale;
	}
}

void newLine(){
	coordenadaY+=16*scale;
	coordenadaX=0;
}

uint64_t getCoords(){
	return (coordenadaY << 32) | ((uint32_t) coordenadaX);
}

void drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t width, uint64_t height){
	for(int i=0; i<height && y+i < VBE_mode_info->height; i++){
		for(int j=0; j<width && x+j < VBE_mode_info->width; j++){
			putPixel(hexColor,x+j,y+i);
		}
	}
}

void clearScreen(){
	drawRectangle(bgColor, 0, 0, VBE_mode_info->width, VBE_mode_info->height );
	coordenadaX=0;
	coordenadaY=0;
}

void setBGColor(uint32_t hexCollor){
	bgColor=hexCollor;
}

uint64_t getBGcolor(){
	return (uint32_t) bgColor;
}

uint8_t setScale(uint8_t newScale){
	if(newScale>0 && newScale<=3){
		scale=newScale;
		return 1;
	}
	return 0;
}

uint64_t getScale(){
	return (uint64_t) scale;
}

uint64_t getScreenSize(){
	return ((uint64_t) VBE_mode_info->width << 32) | VBE_mode_info->height;
}
