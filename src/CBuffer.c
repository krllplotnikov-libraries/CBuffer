#include "CBuffer.h"

static void CBuffer_Reset(CBuffer_st* cBuffer){
	cBuffer->head = 0;
	cBuffer->tail = 0;
	cBuffer->count = 0;
}

/* Копирует size байт из буфера, начиная с позиции pos, с учётом перехода через конец. */
static void CBuffer_CopyOut(const CBuffer_st* cBuffer, size_t pos, uint8_t* data, size_t size){
	size_t first = cBuffer->size - pos;
	if(first > size){
		first = size;
	}
	memcpy(data, &cBuffer->buffer[pos], first);
	memcpy(data + first, cBuffer->buffer, size - first);
}

/* Копирует size байт в буфер, начиная с позиции pos, с учётом перехода через конец. */
static void CBuffer_CopyIn(CBuffer_st* cBuffer, size_t pos, const uint8_t* data, size_t size){
	size_t first = cBuffer->size - pos;
	if(first > size){
		first = size;
	}
	memcpy(&cBuffer->buffer[pos], data, first);
	memcpy(cBuffer->buffer, data + first, size - first);
}

bool CBuffer_Init(CBuffer_st* cBuffer, size_t size){
	if(cBuffer == NULL || size == 0){
		return false;
	}
	cBuffer->buffer = (uint8_t*)malloc(size);
	if(cBuffer->buffer == NULL){
		cBuffer->size = 0;
		cBuffer->ownsMemory = false;
		CBuffer_Reset(cBuffer);
		return false;
	}
	cBuffer->size = size;
	cBuffer->ownsMemory = true;
	CBuffer_Reset(cBuffer);
	return true;
}

bool CBuffer_InitStatic(CBuffer_st* cBuffer, uint8_t* memory, size_t size){
	if(cBuffer == NULL || memory == NULL || size == 0){
		return false;
	}
	cBuffer->buffer = memory;
	cBuffer->size = size;
	cBuffer->ownsMemory = false;
	CBuffer_Reset(cBuffer);
	return true;
}

void CBuffer_DeInit(CBuffer_st* cBuffer){
	if(cBuffer == NULL){
		return;
	}
	if(cBuffer->ownsMemory){
		free(cBuffer->buffer);
	}
	cBuffer->buffer = NULL;
	cBuffer->size = 0;
	cBuffer->ownsMemory = false;
	CBuffer_Reset(cBuffer);
}

size_t CBuffer_Write(CBuffer_st* cBuffer, const uint8_t* data, size_t size){
	if(cBuffer == NULL || cBuffer->buffer == NULL || data == NULL){
		return 0;
	}
	size_t freeSpace = cBuffer->size - cBuffer->count;
	if(size > freeSpace){
		size = freeSpace;
	}
	if(size == 0){
		return 0;
	}
	CBuffer_CopyIn(cBuffer, cBuffer->head, data, size);
	cBuffer->head = (cBuffer->head + size) % cBuffer->size;
	cBuffer->count += size;
	return size;
}

size_t CBuffer_WriteOverwrite(CBuffer_st* cBuffer, const uint8_t* data, size_t size){
	if(cBuffer == NULL || cBuffer->buffer == NULL || data == NULL){
		return 0;
	}
	size_t written = size;
	/* Если данных больше ёмкости, в буфере останутся только последние size байт */
	if(size > cBuffer->size){
		data += size - cBuffer->size;
		size = cBuffer->size;
	}
	size_t freeSpace = cBuffer->size - cBuffer->count;
	if(size > freeSpace){
		CBuffer_Skip(cBuffer, size - freeSpace);
	}
	CBuffer_Write(cBuffer, data, size);
	return written;
}

size_t CBuffer_Peek(const CBuffer_st* cBuffer, uint8_t* data, size_t size){
	if(cBuffer == NULL || cBuffer->buffer == NULL || data == NULL){
		return 0;
	}
	if(size > cBuffer->count){
		size = cBuffer->count;
	}
	if(size == 0){
		return 0;
	}
	CBuffer_CopyOut(cBuffer, cBuffer->tail, data, size);
	return size;
}

size_t CBuffer_Skip(CBuffer_st* cBuffer, size_t size){
	if(cBuffer == NULL || cBuffer->buffer == NULL){
		return 0;
	}
	if(size > cBuffer->count){
		size = cBuffer->count;
	}
	cBuffer->tail = (cBuffer->tail + size) % cBuffer->size;
	cBuffer->count -= size;
	return size;
}

size_t CBuffer_Read(CBuffer_st* cBuffer, uint8_t* data, size_t size){
	size_t read = CBuffer_Peek(cBuffer, data, size);
	CBuffer_Skip(cBuffer, read);
	return read;
}

bool CBuffer_PutByte(CBuffer_st* cBuffer, uint8_t byte){
	return CBuffer_Write(cBuffer, &byte, 1) == 1;
}

bool CBuffer_GetByte(CBuffer_st* cBuffer, uint8_t* byte){
	return CBuffer_Read(cBuffer, byte, 1) == 1;
}

void CBuffer_Clear(CBuffer_st* cBuffer){
	if(cBuffer == NULL){
		return;
	}
	CBuffer_Reset(cBuffer);
}

size_t CBuffer_Available(const CBuffer_st* cBuffer){
	return (cBuffer == NULL) ? 0 : cBuffer->count;
}

size_t CBuffer_Free(const CBuffer_st* cBuffer){
	return (cBuffer == NULL) ? 0 : cBuffer->size - cBuffer->count;
}

size_t CBuffer_Capacity(const CBuffer_st* cBuffer){
	return (cBuffer == NULL) ? 0 : cBuffer->size;
}

bool CBuffer_IsEmpty(const CBuffer_st* cBuffer){
	return CBuffer_Available(cBuffer) == 0;
}

bool CBuffer_IsFull(const CBuffer_st* cBuffer){
	return cBuffer != NULL && cBuffer->size != 0 && cBuffer->count == cBuffer->size;
}
