#include "all.h"

static oneObject(autoList<void (*)(void)>, new autoList<void (*)(void)>(), GetFinalizers)

void addFinalizer(void (*func)(void))
{
	GetFinalizers()->AddElement(func);
}
void (*unaddFinalizer(void))(void)
{
	return GetFinalizers()->UnaddElement();
}

void termination(int errorlevel)
{
	while(GetFinalizers()->GetCount())
	{
		GetFinalizers()->UnaddElement()();
	}
	exit(errorlevel);
}
void error2(char *source, int lineno, char *function, char *message)
{
	{
		static int once;

		if(once)
			exit(2);

		once = 1;
	}

	uint lastError = (uint)GetLastError();

	cout("HTT Error %s %d %s %u\n", source, lineno, function, lastError);

	if(message)
		cout("message: %s\n", message);

	MessageBox(
		NULL,
		message ? message : xcout("An error has occurred @ %s %d %s %u", source, lineno, function, lastError),
		"HTT Error",
		MB_OK | MB_ICONSTOP | MB_TOPMOST
		);

	termination(1);
}

void cout(char *format, ...)
{
	va_list marker;

	va_start(marker, format);
	errorCase(vprintf(format, marker) < 0);
	va_end(marker);
}
char *xcout(char *format, ...)
{
	char *buffer;
	int size;
	va_list marker;

	va_start(marker, format);

	for(size = strlen(format) + 128; ; size *= 2)
	{
		buffer = (char *)memAlloc(size + 20);
		int retval = _vsnprintf(buffer, size + 10, format, marker);
		buffer[size + 10] = '\0'; // �����I�ɕ���B

		if(0 <= retval && retval <= size)
			break;

		memFree(buffer);
		errorCase(128 * 1024 * 1024 < size); // anti-overflow
	}
	va_end(marker);

	return buffer;
}

static int ArgIndex = 1;

int hasArgs(int count)
{
	return count <= __argc - ArgIndex;
}
int argIs(char *spell)
{
	if(ArgIndex < __argc)
	{
		if(!_stricmp(__argv[ArgIndex], spell))
		{
			ArgIndex++;
			return 1;
		}
	}
	return 0;
}
char *getArg(int index)
{
	errorCase(index < 0 || __argc - ArgIndex <= index);
	return __argv[ArgIndex + index];
}
char *nextArg(void)
{
	char *arg = getArg(0);

	ArgIndex++;
	return arg;
}
int getArgIndex(void)
{
	return ArgIndex;
}
void setArgIndex(int index)
{
	errorCase(index < 0 || __argc < index); // index == __argc �͑S���ǂݏI��������
	ArgIndex = index;
}

char *getSelfFile(void)
{
	static char *fileBuff;

	if(!fileBuff)
	{
		const int BUFFSIZE = 1000;
		const int MARGINSIZE = 10;

		fileBuff = (char *)memAlloc(BUFFSIZE + MARGINSIZE);
		errorCase(!GetModuleFileName(NULL, fileBuff, BUFFSIZE)); // ? ���s

		errorCase(strlen(fileBuff) < 8); // �ł��Z���� "C:\\a.exe"
		errorCase(!isalpha(fileBuff[0]));
		errorCase(memcmp(fileBuff + 1, ":\\", 2));
		errorCase(_stricmp(strchr(fileBuff, '\0') - 4, ".exe"));

		fileBuff = strr(fileBuff);
	}
	return fileBuff;
}
char *getSelfDir(void)
{
	static char *dirBuff;

	if(!dirBuff)
		dirBuff = getDir(getSelfFile());

	return dirBuff;
}

// sync > @ My_mkdir

static void My_mkdir_WrLog(char *message)
{
	cout("%s\n", message);

#if 0
	{
		char *logFile = xcout("%s_MKDIR.log", getSelfFile());
		FILE *logFp;

		logFp = fileOpen(logFile, "at");
		writeLine(logFp, message);
		fileClose(logFp);

		memFree(logFile);
	}
#endif
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

static char *TempRtDir;

char *getTempRtDir(void)
{
	static char *&dir = TempRtDir;

	if(!dir)
	{
		dir = ConfigTmpDir;

		if(!dir)
			dir = getenv("TMP");

		if(!dir)
		{
			dir = getenv("TEMP");
			errorCase(!dir);
		}
		dir = getFullPath(dir, getSelfDir()); // TMP, TEMP �̓t���p�X���Ǝv�����ǁA�O�̂��߁B���� dir �����̂��߁B
		cout("TMP_DIR: %s\n", dir);
		errorCase_m(!existDir(dir), "���ϐ� TMP �܂��� TEMP �̃p�X�ɖ�肪����܂��B");

		dir = combine_xx(dir, makeUpper(UserAppUUID ? UserAppUUID : APP_UUID));
		remove(dir); // 2bs
		My_mkdir(dir);
		errorCase(!existDir(dir));
		clearDir(dir);
	}
	return dir;
}
void removeTempRtDir(void)
{
	static char *&dir = TempRtDir;

	LOGPOS();
	if(dir)
	{
		LOGPOS();
		forceRemoveDir(dir);
		memFree(dir);
		dir = NULL;
	}
	LOGPOS();
}
char *makeTempPath(char *suffix)
{
	for(; ; )
	{
		char *pw = makePw36();
		char *path = combine_cx(getTempRtDir(), xcout("%s%s", pw, suffix));
		memFree(pw);

		if(!existPath(path))
			return path;

		memFree(path);
	}
}
char *makeTempFile(char *suffix)
{
	char *out = makeTempPath(suffix);
	createFile(out);
	return out;
}
char *makeTempDir(char *suffix)
{
	char *out = makeTempPath(suffix);
	createDir(out);
	return out;
}

#if 0 // for Test
static DWORD GetTickCount_TEST(void)
{
	static int initOnce;
	static uint baseTick;

	if(!initOnce)
	{
		initOnce = 1;
		baseTick = UINT_MAX - 10000 - GetTickCount();
	}
	return baseTick + GetTickCount();
}
#define GetTickCount() GetTickCount_TEST()
#endif
#if 1
#define NowTick() \
	GetTickCount64()
#else
static uint64 NowTick(void)
{
	uint currTick = GetTickCount();
	static uint lastTick;
	static uint64 baseTick;
	uint64 retTick;
	static uint64 lastRetTick;

	if(currTick < lastTick) // ? �J�E���^���߂��� -> �I�[�o�[�t���[�����H
	{
		uint diffTick = lastTick - currTick;

		// memo: ������ LOGPOS �͍ċA���Ă��Ȃ��B@ 2017.4.18

		if(UINT_MAX / 2 < diffTick) // �I�[�o�[�t���[���낤�B
		{
			LOGPOS();
			baseTick += (uint64)UINT_MAX + 1;
		}
		else // �I�[�o�[�t���[���H
		{
			LOGPOS();
			baseTick += diffTick; // �O��Ɠ����߂�l�ɂȂ�悤�ɒ�������B
		}
	}
	lastTick = currTick;
	retTick = baseTick + currTick;
	errorCase(retTick < lastRetTick); // 2bs
	lastRetTick = retTick;
	return retTick;
}
#endif
uint64 getNowTick()
{
	return NowTick();
}
time_t now(void)
{
	return (time_t)(NowTick() / 1000);
}
time_t getTime(void)
{
	static time_t baseTime = -1;
	static time_t baseNow;

	if(baseTime == -1) // ? not inited
	{
		baseTime = time(NULL);
		baseNow = now();
	}
	return baseTime + now() - baseNow;
}
char *getTimeStamp(time_t t) // t: 0 == ������
{
	static char timeStamp[25];
	char *p;

	if(!t)
		t = time(NULL);

	p = ctime(&t); // "Wed Jan 02 02:03:55 1980\n"

	if(!p) // ? invalid t
		p = "Thu Jan 01 00:00:00 1970";

	memcpy(timeStamp, p, 24);
	return timeStamp; // "Wed Jan 02 02:03:55 1980"
}
