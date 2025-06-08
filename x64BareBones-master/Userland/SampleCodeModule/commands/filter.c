#include "../include/library.h"

static int isVowel(char c) {
	return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' ||
		   c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == '\n';
}

int filter(int argc, char **argv) {
	if (argc != 0) {
		printf("No se requieren argumentos\n");
		return -1;
	}

	int c;
	while ((c = getChar()) != EOF) {
		if (isVowel(c)){
			putChar(c, 0x00FFFFFF);
        }
	}

	return 0;
}
