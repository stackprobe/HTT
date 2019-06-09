#include "all.h"

/*
	unsigned attrib;
		_A_ARCH
		_A_HIDDEN
		_A_NORMAL
		_A_RDONLY
		_A_SUBDIR
		_A_SYSTEM

	time_t time_create;
	time_t time_access;
	time_t time_write;
	_fsize_t size;
	char name[_MAX_PATH];
*/
struct _finddata_t lastFindData;

void updateFindData(char *path)
{
	intptr_t h = _findfirst(path, &lastFindData);
	errorCase(h == -1); // ? Ž¸”s
	_findclose(h);
}
time_t getFileWriteTime(char *file)
{
	updateFindData(file);
	return lastFindData.time_write;
}
