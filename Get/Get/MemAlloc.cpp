#include "all.h"

void *memAlloc(int size)
{
	errorCase(size < 0);
	void *block = malloc(size);
	errorCase(!block);

	return block;
}
void *memRealloc(void *block, int size)
{
	errorCase(size < 0);
	block = realloc(block, size);
	errorCase(!block);

	return block;
}
void memFree(void *block)
{
	free(block);
}
void *memCalloc(int size)
{
	void *block = memAlloc(size);
	memset(block, 0x00, size);
	return block;
}
void *memClone(void *block, int size)
{
	void *newBlock = memAlloc(size);
	memcpy(newBlock, block, size);
	return newBlock;
}
