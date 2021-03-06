#include "all.h"

char *SendFileFullPath;
int RemoveSendFileAtTermination;
char *ClientIP;
int TightFlag; // { 0, "LMH" } == { none, low, middle, hi }

void HTT_Disconnect(void)
{
	if(SendFileFullPath)
//		removeFile(SendFileFullPath); // オープンされていると削除出来ない！
		RemoveSendFileAtTermination = 1;
}

// ---- Mutex ----

static int GetLogMutex(void);

static void CloseLogMutex(void)
{
	LOGPOS();
	handleClose(GetLogMutex());
}
static int GetLogMutex(void)
{
	static int h;

	if(!h) // ? 未初期化
	{
		static char *MUTEX_NAME = "{4e08b31a-9280-4ee0-9f9a-2ef462589893}"; // shared_uuid

		LOGPOS();
		h = mutexOpen(MUTEX_NAME);
		addFinalizer(CloseLogMutex);
	}
	return h;
}

static void ReleaseLogMutex(void)
{
	LOGPOS();
	mutexRelease(GetLogMutex());
}
void WaitLogMutex(void)
{
	if(WriteLog_Disabled)
	{
		LOGPOS();
		return;
	}

	LOGPOS();
	waitForever(GetLogMutex());
	addFinalizer(ReleaseLogMutex);
}

// ---- Log ----

static char *GetAccessLogFile(void)
{
	static char *file;

	if(!file)
		file = combine(getSelfDir(), "AccessLog.dat");

	return file;
}
static char *GetAccessLogFile_2nd(void)
{
	static char *file;

	if(!file)
		file = combine(getSelfDir(), "AccessLog0.dat");

	return file;
}

int WriteLog_Disabled;

void WriteLog(char *line)
{
	errorCase(!line);
	WriteLog_x(strx(line));
}
void WriteLog_x(char *line)
{
	if(WriteLog_Disabled)
	{
		memFree(line);
		return;
	}

	toJToken(line);
	cout("LOG: %s\n", line);
	writeAddLine(GetAccessLogFile(), line);
	memFree(line);

	// サイズ調整
	{
		__int64 fileSize = getFileSize(GetAccessLogFile());

		if(100000 < fileSize) // ? 上限オーバー
		{
			removeFile(GetAccessLogFile_2nd()); // 存在しなくてもエラーにならない！
			moveFile(GetAccessLogFile(), GetAccessLogFile_2nd());
		}
	}
}

// ----
