#include  "../include/library.h"

int cat(int argc, char **argv) {
	if (argc != 0) {
		printf("No se requieren argumentos\n");
		return -1;
	}
	int c;
	while ((c = getChar()) != EOF) {
		if (c){
			putChar(c, 0x00FFFFFF);
        }
	}

	return 0;
}