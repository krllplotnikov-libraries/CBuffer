#ifndef CBUFFER_H
#define CBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Циклический (кольцевой) буфер байтов.
 *
 * head  - индекс, куда будет записан следующий байт
 * tail  - индекс, откуда будет прочитан следующий байт
 * count - количество байт, находящихся в буфере (позволяет различать
 *         состояния "пуст" и "полон", когда head == tail)
 * size  - ёмкость буфера в байтах
 */
typedef struct{
	size_t head;
	size_t tail;
	size_t size;
	size_t count;
	uint8_t* buffer;
	bool ownsMemory;	/* true - память выделена через malloc и будет освобождена в DeInit */
}CBuffer_st;

/* Инициализация с динамическим выделением памяти. Возвращает false при ошибке. */
bool CBuffer_Init(CBuffer_st* cBuffer, size_t size);
/* Инициализация с пользовательской (например, статической) памятью. */
bool CBuffer_InitStatic(CBuffer_st* cBuffer, uint8_t* memory, size_t size);
/* Освобождение ресурсов. После вызова структуру можно инициализировать заново. */
void CBuffer_DeInit(CBuffer_st* cBuffer);

/* Запись до size байт. Записывает столько, сколько помещается; возвращает число записанных байт. */
size_t CBuffer_Write(CBuffer_st* cBuffer, const uint8_t* data, size_t size);
/* Запись с перезаписью самых старых данных при переполнении. Возвращает число записанных байт (всегда size). */
size_t CBuffer_WriteOverwrite(CBuffer_st* cBuffer, const uint8_t* data, size_t size);
/* Чтение до size байт с удалением из буфера. Возвращает число прочитанных байт. */
size_t CBuffer_Read(CBuffer_st* cBuffer, uint8_t* data, size_t size);
/* Чтение до size байт без удаления из буфера. Возвращает число скопированных байт. */
size_t CBuffer_Peek(const CBuffer_st* cBuffer, uint8_t* data, size_t size);
/* Удаление до size байт без копирования. Возвращает число удалённых байт. */
size_t CBuffer_Skip(CBuffer_st* cBuffer, size_t size);

/* Побайтовые операции. Возвращают false, если буфер полон / пуст. */
bool CBuffer_PutByte(CBuffer_st* cBuffer, uint8_t byte);
bool CBuffer_GetByte(CBuffer_st* cBuffer, uint8_t* byte);

/* Очистка буфера (данные отбрасываются, память не освобождается). */
void CBuffer_Clear(CBuffer_st* cBuffer);

/* Информационные функции. */
size_t CBuffer_Available(const CBuffer_st* cBuffer);	/* байт доступно для чтения */
size_t CBuffer_Free(const CBuffer_st* cBuffer);		/* байт свободно для записи */
size_t CBuffer_Capacity(const CBuffer_st* cBuffer);	/* общая ёмкость */
bool CBuffer_IsEmpty(const CBuffer_st* cBuffer);
bool CBuffer_IsFull(const CBuffer_st* cBuffer);

#ifdef __cplusplus
}
#endif

#endif /* CBUFFER_H */
