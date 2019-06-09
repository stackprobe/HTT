#include "all.h"

static oneObject(autoList<void (*)(void)>, new autoList<void (*)(void)>(), GetFinalizers)

void addFinalizer(void (*func)(void))
{
	GetFinalizers()->AddElement(func);
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

// sync > @ cout

void cout(char *format, ...)
{
	va_list marker;

	va_start(marker, format);

	if(!strcmp(format, "%s"))
	{
		coutLongText(va_arg(marker, char *));
	}
	else if(!strcmp(format, "%s\n"))
	{
		coutLongText(va_arg(marker, char *));
		coutLongText("\n");
	}
	else
		coutLongText_x(vxcout(format, marker));

	va_end(marker);
}
char *xcout(char *format, ...)
{
	char *ret;
	va_list marker;

	va_start(marker, format);
	ret = vxcout(format, marker);
	va_end(marker);

	return ret;
}
char *vxcout(char *format, va_list marker)
{
	char *buffer;

	for(int size = strlen(format) + 100; ; size *= 2)
	{
		int ret;

#define MARGIN 10

		buffer = (char *)memAlloc(size + MARGIN * 2);
		ret = _vsnprintf(buffer, size + MARGIN, format, marker);

#undef MARGIN

		if(0 <= ret && ret <= size)
			break;

		memFree(buffer);
		errorCase(128 * 1024 * 1024 < size); // ANTI OVER-FLOW
	}
	return strr(buffer);
}
void coutLongText(char *text)
{
	char format[6]; // max: "%.99s"
	char *p;
	char *q;
	uint d;

	for(p = text; *p; p = q)
	{
#define PRINT_LMT 98

		for(q = p; *q && (uint)q - (uint)p < PRINT_LMT; q += _ismbblead(*q) ? 2 : 1)
		{}

#undef PRINT_LMT

		d = (uint)q - (uint)p;

		errorCase(sprintf(format, "%%.%us", d) < 0);
		errorCase(printf(format, p) != d);
	}
}
void coutLongText_x(char *text)
{
	coutLongText(text);
	memFree(text);
}

// < sync

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

char *getSelfFile(void)
{
	static char *fileBuff;

	if(!fileBuff)
	{
		const int BUFFSIZE = 1000;
		const int MARGINSIZE = 10;

		fileBuff = (char *)memAlloc(BUFFSIZE + MARGINSIZE);
		errorCase(!GetModuleFileName(NULL, fileBuff, BUFFSIZE)); // ? 失敗

		errorCase(strlen(fileBuff) < 8); // 最も短くて "C:\\a.exe"
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
	・WHTTR.exe(HTT_RPC)を起動する。
		WHTTR.exeがHTT.exeを起動する。このとき...
			WHTTR.exe起動時にWindowsDefenderのSmartScreenが表示されること。
			AntiWindowsDefenderSmartScreen()によってHTT.exeがWHTTR.exeによって読み込み・削除・書き出しされること。
	・WindowsDefenderのリアルタイム保護をオフにする。<-- 必須ではないかも。
	・デスクトップをロックして放置する。
	この手順で CreateDirectory, _mkdir が稀に失敗する。<-- 1分毎の空実行時の作業ディレクトリの作成。作成可能なはず。
	失敗した場合 MD で1回リトライすれば成功する模様。
	Sleepと5回のリトライは念の為。
*/
static int My_mkdir(char *dir) // ret: ? 失敗
{
#if 1
	if(CreateDirectory(dir, NULL) == 0) // ? 失敗
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
	if(CreateDirectory(dir, NULL) == 0) // ? 失敗
	{
		return 1;
	}
	return 0;
#else
	if(_mkdir(dir)) // ? 失敗
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
		dir = getFullPath(dir, getSelfDir()); // TMP, TEMP はフルパスだと思うけど、念のため。あと dir 複製のため。
		cout("TMP_DIR: %s\n", dir);
		errorCase_m(!existDir(dir), "環境変数 TMP または TEMP のパスに問題があります。");

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
char *makeTempDir(char *suffix)
{
	char *out = makeTempPath(suffix);
	createDir(out);
	return out;
}

#define NowTick() \
	GetTickCount64()

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
char *getTimeStamp(time_t t) // t: 0 == 現時刻
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
