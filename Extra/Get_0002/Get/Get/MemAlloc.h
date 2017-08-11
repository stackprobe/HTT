void *memAlloc(int size);
void *memRealloc(void *block, int size);
void memFree(void *block);
void *memCalloc(int size);
void *memClone(void *block, int size);

#define na(TYPE_T, count) \
	((TYPE_T *)memCalloc(sizeof(TYPE_T) * (count)))

#define nb(TYPE_T) \
	(na(TYPE_T, 1))
