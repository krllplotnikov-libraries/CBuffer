#include <stdio.h>
#include <assert.h>
#include "../src/CBuffer.h"

static void test_basic(void){
	CBuffer_st cb;
	uint8_t out[8];
	assert(CBuffer_Init(&cb, 5));
	assert(CBuffer_IsEmpty(&cb));
	assert(CBuffer_Write(&cb, (const uint8_t*)"abc", 3) == 3);
	assert(CBuffer_Available(&cb) == 3 && CBuffer_Free(&cb) == 2);
	assert(CBuffer_Read(&cb, out, 2) == 2 && memcmp(out, "ab", 2) == 0);
	/* Запись с переходом через конец буфера */
	assert(CBuffer_Write(&cb, (const uint8_t*)"defgh", 5) == 4);
	assert(CBuffer_IsFull(&cb));
	assert(CBuffer_Peek(&cb, out, 8) == 5 && memcmp(out, "cdefg", 5) == 0);
	assert(CBuffer_Read(&cb, out, 8) == 5 && memcmp(out, "cdefg", 5) == 0);
	assert(CBuffer_IsEmpty(&cb));
	CBuffer_DeInit(&cb);
}

static void test_overwrite(void){
	CBuffer_st cb;
	uint8_t out[8];
	assert(CBuffer_Init(&cb, 4));
	CBuffer_WriteOverwrite(&cb, (const uint8_t*)"abc", 3);
	CBuffer_WriteOverwrite(&cb, (const uint8_t*)"de", 2);
	assert(CBuffer_Read(&cb, out, 8) == 4 && memcmp(out, "bcde", 4) == 0);
	CBuffer_WriteOverwrite(&cb, (const uint8_t*)"123456", 6);
	assert(CBuffer_Read(&cb, out, 8) == 4 && memcmp(out, "3456", 4) == 0);
	CBuffer_DeInit(&cb);
}

static void test_static_and_bytes(void){
	static uint8_t mem[3];
	CBuffer_st cb;
	uint8_t b;
	assert(CBuffer_InitStatic(&cb, mem, sizeof(mem)));
	assert(CBuffer_PutByte(&cb, 1) && CBuffer_PutByte(&cb, 2) && CBuffer_PutByte(&cb, 3));
	assert(!CBuffer_PutByte(&cb, 4));
	assert(CBuffer_GetByte(&cb, &b) && b == 1);
	assert(CBuffer_Skip(&cb, 10) == 2);
	assert(!CBuffer_GetByte(&cb, &b));
	CBuffer_DeInit(&cb);
}

int main(void){
	test_basic();
	test_overwrite();
	test_static_and_bytes();
	printf("All tests passed\n");
	return 0;
}
