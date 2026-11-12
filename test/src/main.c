#include <stdio.h>
#include <stdlib.h>

#include "CBuffer.h"

CBuffer_st cBuffer = {0};

int main(void) {

	CBuffer_Init(&cBuffer, 256);

	printf("Hello\r\n");

	return EXIT_SUCCESS;
}
