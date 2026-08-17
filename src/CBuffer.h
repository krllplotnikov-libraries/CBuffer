#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	size_t head;
	size_t tail;
	size_t size;
	uint8_t* buffer;
}CBuffer_st;

void CBuffer_Init(CBuffer_st* cBuffer, size_t size);
void CBuffer_DeInit(CBuffer_st* cBuffer);
void CBuffer_Write(CBuffer_st* cBuffer, uint8_t* data, size_t size);
void CBuffer_Read(CBuffer_st* cBuffer, uint8_t* data, size_t size);
