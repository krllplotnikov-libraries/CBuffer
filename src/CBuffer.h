#ifndef CBUFFER_H_
#define CBUFFER_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	CBUFFER_ERROR,
	CBUFFER_OK,
}CBuffer_Error_et;

typedef struct{
	size_t size;
	uint8_t* buffer;
}CBuffer_Descriptor_st;

typedef struct{
	CBuffer_Descriptor_st descriptor;
	size_t head;
	size_t tail;
	size_t freeSize;
	uint8_t isEmpty;
	uint8_t isFull;
}CBuffer_st;

#ifdef __cplusplus
extern "C" {
#endif

CBuffer_Error_et CBuffer_Init(CBuffer_st* cBuffer, CBuffer_Descriptor_st* descriptor);
CBuffer_Error_et CBuffer_Write(CBuffer_st* cBuffer, uint8_t* data, size_t size);
CBuffer_Error_et CBuffer_Read(CBuffer_st* cBuffer, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif
