void *memAlloc(int size);
void *memRealloc(void *block, int size);
void memFree(void *block);
void *memCalloc(int size);

#define na_(TYPE_T, count) \
	((TYPE_T *)memCalloc(sizeof(TYPE_T) * (count)))

#define nb_(TYPE_T) \
	(na_(TYPE_T, 1))
