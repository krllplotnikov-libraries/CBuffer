#include <stdio.h>
#include <stdlib.h>

#include "CBuffer.h"

#define CBUFFER_SIZE 8

uint8_t buffer1[CBUFFER_SIZE] = {0};
uint8_t buffer2[CBUFFER_SIZE] = {0};

CBuffer_st cBuffer1 = {0};
CBuffer_Descriptor_st cBufferDescriptor1 = {
		.size = CBUFFER_SIZE,
		.buffer = buffer1
};

CBuffer_st cBuffer2 = {0};
CBuffer_Descriptor_st cBufferDescriptor2 = {
		.size = CBUFFER_SIZE,
		.buffer = buffer2
};

void Test_1();
void printCBuffer1Info();

int main(void){

	CBuffer_Init(&cBuffer1, &cBufferDescriptor1);

	Test_1();

	return EXIT_SUCCESS;
}

void Test_1(){
	printf("Test 1: ");
	uint8_t dataToWrite[CBUFFER_SIZE] = {0};
	uint8_t dataToRead[CBUFFER_SIZE] = {0};
	for(size_t i = 1; i <= CBUFFER_SIZE; i++){
		for(size_t j = 0; j < i; j++){
			dataToWrite[j] = j + 1;
		}

		CBuffer_Write(&cBuffer1, dataToWrite, i);
		printCBuffer1Info();

		CBuffer_Read(&cBuffer1, dataToRead, i);
		printCBuffer1Info();

		if(memcmp(dataToWrite, dataToRead, i)){
			printf("Fail\n");
			return;
		}

		memset(dataToWrite, 0, CBUFFER_SIZE);
		memset(dataToRead, 0, CBUFFER_SIZE);
	}
	printf("Pass\n");
}

void printCBuffer1Info(){
	printf("CBuffer1: Size - %d\n", cBuffer1.descriptor.size);
	printf("CBuffer1: Free size - %d\n", cBuffer1.freeSize);
	printf("CBuffer1: Head - %d\n", cBuffer1.head);
	printf("CBuffer1: Tail - %d\n", cBuffer1.tail);
	printf("CBuffer1: isEmpty - %d\n", cBuffer1.isEmpty);
	printf("CBuffer1: isFull - %d\n", cBuffer1.isFull);
	printf("CBuffer1: Data\n");
	for(size_t i = 0; i < CBUFFER_SIZE; i++){
		printf("%02X ", buffer1[i]);
	}
	printf("\r\n");
}
