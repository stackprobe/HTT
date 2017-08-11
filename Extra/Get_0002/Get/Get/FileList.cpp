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

autoList<char *> *ls(char *dir, char *wCard)
{
	autoList<char *> *paths = new autoList<char *>();

	addCwd(dir);
	intptr_t h = _findfirst(wCard, &lastFindData);

	if(h != -1) // ? 成功 && ファイル・ディレクトリを見つけた。
	{
		do
		{
			char *name = lastFindData.name;

			if(!strcmp(name, ".") || !strcmp(name, ".."))
				continue;

			errorCase(strchr(name, '?')); // ? unicode を含む

#if 0
			if(lastFindData.attrib & _A_SUBDIR)
				if(!_stricmp(name, ".svn") || !_stricmp(name, "_svn") || !_stricmp(getEnd(name, 4), ".rum"))
					continue;
#endif

			paths->AddElement(combine(dir, name));
		}
		while(!_findnext(h, &lastFindData)); // ? 成功 && まだファイル・ディレクトリがある。

		_findclose(h);
	}
	unaddCwd();

	return paths;
}
autoList<char *> *lss(char *dir, char *wCard)
{
	autoList<char *> *paths = ls(dir, wCard);

	for(int index = 0; index < paths->GetCount(); index++)
	{
		char *path = paths->GetElement(index);

		if(existDir(path))
		{
			paths->AddElements_x(lss(path, wCard));
		}
	}
	return paths;
}

void updateFindData(char *path)
{
	intptr_t h = _findfirst(path, &lastFindData);
	errorCase(h == -1); // ? 失敗
	_findclose(h);
}
time_t getFileWriteTime(char *file)
{
	updateFindData(file);
	return lastFindData.time_write;
}
