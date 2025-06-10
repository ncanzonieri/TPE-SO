// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/library.h"

int wc(int argc, char **argv) {
	if (argc != 0) {
		printf("No se requieren argumentos\n");
		return -1;
	}

	int lines = 0;
	int c;
	while ((c = getChar()) != EOF) {
		if (c == '\n') {
			lines++;
		}
	}

	printf("\n%d\n", lines);

	return 0;
}