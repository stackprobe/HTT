#include "all.h"

char *SendFileFullPath;
int RemoveSendFileAtTermination;
char *ClientIP;

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

void WriteLog(char *line)
{
	errorCase(!line);
	WriteLog_x(strx(line));
}
void WriteLog_x(char *line)
{
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

#if 0 // old
			autoList<char *> *lines = readLines(GetAccessLogFile());

			for(int c = 0; c < 20; c++) // 適当な行数を先頭から順に削除
				if(lines->GetCount())
					memFree(lines->DesertElement(0));

			writeLines_cx(GetAccessLogFile(), lines);
#endif
		}
	}
}

// ----
