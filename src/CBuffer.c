#include "CBuffer.h"

CBuffer_Error_et CBuffer_Init(CBuffer_st* cBuffer, CBuffer_Descriptor_st* descriptor){
	cBuffer->head = 0;
	cBuffer->tail = 0;
	cBuffer->isEmpty = 1;
	cBuffer->isFull = 0;
	cBuffer->freeSize = descriptor->size;
	cBuffer->descriptor.size = descriptor->size;
	cBuffer->descriptor.buffer = descriptor->buffer;

	return CBUFFER_OK;
}

CBuffer_Error_et CBuffer_Write(CBuffer_st* cBuffer, uint8_t* data, size_t size){
	if(cBuffer->freeSize < size){
		return CBUFFER_ERROR;
	}

	if(cBuffer->tail + size < cBuffer->descriptor.size){
		memcpy(&cBuffer->descriptor.buffer[cBuffer->tail], data, size);
		cBuffer->tail += size;
	}
	else{
		memcpy(&cBuffer->descriptor.buffer[cBuffer->tail], data, cBuffer->descriptor.size - cBuffer->tail);
		memcpy(&cBuffer->descriptor.buffer[0], &data[cBuffer->descriptor.size - cBuffer->tail], size - (cBuffer->descriptor.size - cBuffer->tail));
		cBuffer->tail = size - (cBuffer->descriptor.size - cBuffer->tail);
	}

	cBuffer->freeSize -= size;
	cBuffer->isEmpty = 0;

	if(cBuffer->freeSize == 0){
		cBuffer->isFull = 1;
	}

	return CBUFFER_OK;
}

CBuffer_Error_et CBuffer_Read(CBuffer_st* cBuffer, uint8_t* data, size_t size){
	if(cBuffer->descriptor.size - cBuffer->freeSize < size){
		return CBUFFER_ERROR;
	}

	if(cBuffer->head + size < cBuffer->descriptor.size){
		memcpy(data, &cBuffer->descriptor.buffer[cBuffer->head], size);
		cBuffer->head += size;
	}
	else{
		memcpy(data, &cBuffer->descriptor.buffer[cBuffer->head], cBuffer->descriptor.size - cBuffer->head);
		memcpy(&data[cBuffer->descriptor.size - cBuffer->head], &cBuffer->descriptor.buffer[0], size - (cBuffer->descriptor.size - cBuffer->head));
		cBuffer->head = size - (cBuffer->descriptor.size - cBuffer->head);
	}

	cBuffer->freeSize += size;
	cBuffer->isFull = 0;

	if(cBuffer->freeSize == cBuffer->descriptor.size){
		cBuffer->isEmpty = 1;
	}

	return CBUFFER_OK;
}
