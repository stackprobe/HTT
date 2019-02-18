#include "all.h"

int existPath(char *path)
{
	return !m_isEmpty(path) && !_access(path, 0);
}
int existFile(char *file)
{
	return existPath(file) && !(GetFileAttributes(file) & FILE_ATTRIBUTE_DIRECTORY);
}
int existDir(char *dir)
{
	return existPath(dir) && GetFileAttributes(dir) & FILE_ATTRIBUTE_DIRECTORY;
}

void trimPath(char *path)
{
	escapeYen(path);
	trimSequ(path, '/');
	unescapeYen(path);
}
char *combine(char *dir, char *file)
{
	if(dir[0] && dir[1] == ':' && dir[2] == '\0')
	{
		static char buff[] = "?:.";

		buff[0] = dir[0];
		dir = buff;
	}
	char *dirFile = xcout("%s\\%s", dir, file);

	trimPath(dirFile);
	return dirFile;
}
char *combine_cx(char *dir, char *file)
{
	char *out = combine(dir, file);
	memFree(file);
	return out;
}
char *combine_xc(char *dir, char *file)
{
	char *out = combine(dir, file);
	memFree(dir);
	return out;
}
char *combine_xx(char *dir, char *file)
{
	char *out = combine_xc(dir, file);
	memFree(file);
	return out;
}
char *getDir(char *path)
{
	path = strx(path);
	escapeYen(path);
	char *p = strrchr(path, '/');

	if(p)
	{
		*p = '\0';
		unescapeYen(path);

		// ? ÉãÅ[ÉgÉfÉBÉåÉNÉgÉä
		if(
			path[0] &&
			path[1] == ':' &&
			path[2] == '\0'
			)
		{
			path[2] = '\\';
			path[3] = '\0';
		}
	}
	else
		strz(path, ".");

	return path;
}
char *getDir_x(char *path)
{
	char *out = getDir(path);
	memFree(path);
	return out;
}

void setCwd(char *dir)
{
	errorCase(m_isEmpty(dir));
	errorCase(_chdir(dir)); // ? é∏îs
}
void setCwd_x(char *dir)
{
	setCwd(dir);
	memFree(dir);
}
char *getCwd(void)
{
	char *dirBuff = _getcwd(NULL, 0);
	errorCase(m_isEmpty(dirBuff));
	char *dir = strx(dirBuff);
	free(dirBuff);
	return dir;
}
static oneObject(autoList<char *>, new autoList<char *>(), GetCwdStack)

void addCwd(char *dir)
{
	GetCwdStack()->AddElement(getCwd());
	setCwd(dir);
}
void addCwd_x(char *dir)
{
	addCwd(dir);
	memFree(dir);
}
void unaddCwd(void)
{
	setCwd_x(GetCwdStack()->UnaddElement());
}

// sync > @ My_mkdir

static void My_mkdir_WrLog(char *message)
{
#if 0
	char *logFile = xcout("%s_MKDIR.log", getSelfFile());
	FILE *logFp;

	logFp = fileOpen(logFile, "at");
	writeLine(logFp, message);
	fileClose(logFp);

	memFree(logFile);
#endif
}
static void My_mkdir_WrLog_x(char *message)
{
	My_mkdir_WrLog(message);
	memFree(message);
}
static int My_mkdir(char *dir) // ret: ? é∏îs
{
#if 1
	if(CreateDirectory(dir, NULL) == 0) // ? é∏îs
	{
		My_mkdir_WrLog_x(xcout("CreateDirectory() failed \"%s\" %u @ %I64d", dir, GetLastError(), time(NULL)));

		Sleep(100);

		for(int c = 0; ; c++)
		{
			My_mkdir_WrLog("*1");

			if(existDir(dir))
				break;

			My_mkdir_WrLog("*2");

			if(5 <= c)
			{
				My_mkdir_WrLog("*2.5");
				return 1;
			}
			My_mkdir_WrLog("*3");

			Sleep(100);

			{
				char *command = xcout("MD \"%s\"", dir);
				system(command);
				memFree(command);
			}

			Sleep(100);

			My_mkdir_WrLog("*4");
		}
		My_mkdir_WrLog("*5");
	}
	return 0;
#elif 1
	if(CreateDirectory(dir, NULL) == 0) // ? é∏îs
	{
		return 1;
	}
	return 0;
#else
	if(_mkdir(dir)) // ? é∏îs
	{
		return 1;
	}
	return 0;
#endif
}

// < sync

void createFile(char *file)
{
	errorCase(m_isEmpty(file));
	fileClose(fileOpen(file, "wb"));
}
void createDir(char *dir)
{
	errorCase(m_isEmpty(dir));
	errorCase(My_mkdir(dir)); // ? é∏îs
}

void removeFile(char *file)
{
	errorCase(m_isEmpty(file));
	remove(file);
}
void removeDir(char *dir)
{
	errorCase(m_isEmpty(dir));
	_rmdir(dir);
}
void clearDir(char *dir)
{
	for(int c = 0; c < 175; c++)
	{
		autoList<char *> *paths = lss(dir);

		if(paths->GetCount() == 0)
		{
			delete paths;
			break;
		}
		if(1 <= c)
		{
			LOGPOS();
			Sleep(c); // 1 + 2 + 3 + ... + 174 == 15225
		}
		while(paths->GetCount())
		{
			char *path = paths->UnaddElement();

			remove(path);
			_rmdir(path);

			memFree(path);
		}
		delete paths;
	}
}
void forceRemoveDir(char *dir)
{
	clearDir(dir);

	for(int c = 1; c < 175; c++)
	{
		_rmdir(dir);

		if(!existPath(dir))
			break;

		LOGPOS();
		Sleep(c); // 1 + 2 + 3 + ... + 174 == 15225
	}
}

char *getFullPath(char *path, char *baseDir)
{
	errorCase(m_isEmpty(path));
	errorCase(m_isEmpty(baseDir));

	addCwd(baseDir);
	path = _fullpath(NULL, path, 0);
	unaddCwd();

	errorCase(m_isEmpty(path));

	char *retPath = strx(path);
	free(path);
	return retPath;
}
