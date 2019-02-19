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
char *combine_xx(char *dir, char *file)
{
	char *out = combine_cx(dir, file);
	memFree(dir);
	return out;
}
char *combine_xc(char *dir, char *file)
{
	char *out = combine(dir, file);
	memFree(dir);
	return out;
}
char *getDir(char *path)
{
	path = strx(path);
	escapeYen(path);
	char *p = strrchr(path, '/');

	if(p)
	{
		if(path[0] && path[1] == ':' && path + 2 == p) // ���[�g�f�B���N�g���΍�
			p++;

		*p = '\0';
		unescapeYen(path);
	}
	else
		strz(path, ".");

	return path;
}

void setCwd(char *dir)
{
	errorCase(m_isEmpty(dir));
	errorCase(_chdir(dir)); // ? ���s
}
void setCwd_x(char *dir)
{
	setCwd(dir);
	memFree(dir);
}
char *getCwd(void)
{
	char *dirBuff = _getcwd(NULL, 0);
	errorCase(m_isEmpty(dirBuff)); // ? ���s || ""
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
	cout("%s\n", message);

	{
		char *logFile = xcout("%s_MKDIR.log", getSelfFile());
		FILE *logFp;

		logFp = fileOpen(logFile, "at");
		writeLine(logFp, message);
		fileClose(logFp);

		memFree(logFile);
	}
}
static void My_mkdir_WrLog_x(char *message)
{
	My_mkdir_WrLog(message);
	memFree(message);
}
/*
	2019.2.18
	�EWHTTR.exe(HTT_RPC)���N������B
		WHTTR.exe��HTT.exe���N������B���̂Ƃ�...
			WHTTR.exe�N������WindowsDefender��SmartScreen���\������邱�ƁB
			AntiWindowsDefenderSmartScreen()�ɂ����HTT.exe��WHTTR.exe�ɂ���ēǂݍ��݁E�폜�E�����o������邱�ƁB
	�EWindowsDefender�̃��A���^�C���ی���I�t�ɂ���B<-- �K�{�ł͂Ȃ������B
	�E�f�X�N�g�b�v�����b�N���ĕ��u����B
	���̎菇�� CreateDirectory, _mkdir ���H�Ɏ��s����B<-- 1�����̋���s���̍�ƃf�B���N�g���̍쐬�B�쐬�\�Ȃ͂��B
	���s�����ꍇ MD ��1�񃊃g���C����ΐ�������͗l�B
	Sleep��5��̃��g���C�͔O�ׁ̈B
*/
static int My_mkdir(char *dir) // ret: ? ���s
{
#if 1
	if(CreateDirectory(dir, NULL) == 0) // ? ���s
	{
		My_mkdir_WrLog_x(xcout("CreateDirectory() failed \"%s\" %u @ %I64d", dir, GetLastError(), time(NULL)));
		My_mkdir_WrLog("*1");

		for(int c = 0; ; c++)
		{
			Sleep(100);
			My_mkdir_WrLog("*2");

			if(existDir(dir))
				break;

			My_mkdir_WrLog("*3");

			if(5 <= c)
			{
				My_mkdir_WrLog("*3.5");
				return 1;
			}
			My_mkdir_WrLog("*4");
			Sleep(100);

			{
				char *command = xcout("MD \"%s\"", dir);
				system(command);
				memFree(command);
			}
		}
		My_mkdir_WrLog("*5");
	}
	return 0;
#elif 1
	if(CreateDirectory(dir, NULL) == 0) // ? ���s
	{
		return 1;
	}
	return 0;
#else
	if(_mkdir(dir)) // ? ���s
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
	errorCase(My_mkdir(dir)); // ? ���s
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
	autoList<char *> *paths = lss(dir);

	while(paths->GetCount())
	{
		char *path = paths->UnaddElement();

		remove(path);
		_rmdir(path);

		memFree(path);
	}
	delete paths;
}
void forceRemoveDir(char *dir)
{
	clearDir(dir);
	_rmdir(dir);
}

/*
	moveFile()
		�t�@�C�� - �����ł�
		�f�B���N�g�� - �����f�B���N�g����
*/
void moveFile(char *srcFile, char *destFile)
{
	if(rename(srcFile, destFile))
	{
		error();
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
char *getExt(char *path)
{
	char *ext = strrchr(path, '.');

	if(ext)
		ext++;
	else
		ext = strchr(path, '\0');

	return ext;
}

static int IsWindowsReserveNode(char *node)
{
	if(!_stricmp(node, "AUX")) return 1;
	if(!_stricmp(node, "CON")) return 1;
	if(!_stricmp(node, "NUL")) return 1;
	if(!_stricmp(node, "PRN")) return 1;

	if(
		!my_strnicmp(node, "COM", 3) ||
		!my_strnicmp(node, "LPT", 3)
		)
		if(
			'1' <= node[3] && node[3] <= '9' &&
			node[4] == '\0'
			)
			return 1;

	// �O���[�]�[�� {

	if(!_stricmp(node, "CLOCK$")) return 1;
	if(!_stricmp(node, "CONFIG$")) return 1;
	if(!_stricmp(node, "COM0")) return 1;
	if(!_stricmp(node, "LPT0")) return 1;

	/*
	if(
		!my_strnicmp(node, "COM", 3) ||
		!my_strnicmp(node, "LPT", 3)
		)
		if(
			'1' <= node[3] && node[3] <= '9' &&
			'0' <= node[4] && node[4] <= '9' &&
			node[5] == '\0'
			)
			return 1;
			*/

	// }

	return 0;
}
static int IsPathChar(int chr)
{
	if(
		'\x20' <= chr && chr <= '\x7e' ||
		'\xa1' <= chr && chr <= '\xdf'
		)
		if(!strchr("\"*/:<>?\\|", chr)) return 1;

	return 0;
}
char *makeFairLocalPath(char *localPath)
{
	autoList<char *> *nodes = tokenize(localPath, ".");

	for(int index = 0; index < nodes->GetCount(); index++)
	{
		char *node = nodes->GetElement(index);

		for(char *p = node; *p; p++)
		{
			if(isJChar(p))
			{
				p++;
			}
			else if(IsPathChar(*p))
			{
				1; // noop
			}
			else
			{
				*p = '#'; // �s���ȕ����̑���
			}
		}
		trimEdge(node, ' ');

		if(IsWindowsReserveNode(node))
		{
			node[0] = '\0';
		}
		if(!node[0])
		{
			strz(node, "$_");
		}
		nodes->SetElement(index, node);
	}
	return untokenize_xc(nodes, ".");
}
