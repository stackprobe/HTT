#include "all.h"

int IPCountMax = 20;
int ZanteiConnectCountMax_L = 47000;
int ZanteiConnectCountMax_M = 48000;
int ZanteiConnectCountMax_H = 49000;
int ConnectCountMax         = 50000;
uint64 DiskFreeBootTime = 2000000000;
uint64 DiskFreeYellow   = 1500000000;
uint64 DiskFreeRed      = 1000000000;
uint64 MemFreeBootTime  = 2000000000;
uint64 MemFreeYellow    = 1500000000;
uint64 MemFreeRed       = 1000000000;
uint MemFreePercentBootTime = 20;
uint MemFreePercentYellow   = 15;
uint MemFreePercentRed      = 10;
uint BindINetAddr;
int TimeWaitEnabled = 1;
int TimeWaitSec = 120;
int TimeWaitMax = 15000;
int TimeWaitRatePower = 3;
int TimeWaitSleepMillisMax = 50;
int RecvServiceNameTimeoutSec = 60;
char *ConfigTmpDir;
int SoftStopServerTimeoutSec = 3;

static int ConfLineNo;

#define ERROR_MSG_LEAD "HTT設定ファイル HTT.conf の読み込みに失敗しました。\n書式に問題があるか、ファイルが壊れています。\nreason-phrase : "

static char *ReadConfLine(FILE *fp, int nullOk = 0)
{
	for(; ; )
	{
		char *line = readLine(fp);

		if(!line)
		{
			if(nullOk)
				return NULL;

			error_m(ERROR_MSG_LEAD "read eof");
		}
		ConfLineNo++;

		if(line[0] == '\0') // 空行
			continue;

		if(line[0] == ';') // コメント行
			continue;

		cout("confLine(%d): %s\n", ConfLineNo, line);
		return line;
	}
}
static int ToInt_x(char *line, int minval, int maxval)
{
	errorCase(maxval < minval);

	int value;

	if(!_stricmp(line, "IMAX"))
		value = IMAX;
	else
		value = atoi(line);

	errorCase_m(!m_isRange(value, minval, maxval), ERROR_MSG_LEAD "out of range");

	memFree(line);
	return value;
}

void LoadConfig(void)
{
	// ---- set default ----

	BindINetAddr = htonl(INADDR_ANY);

	// ----

	LOGPOS();

	char *confFile = (char *)memAlloc(strlen(getSelfFile()) + 100);
	strcpy(confFile, getSelfFile());
	strcpy(strchr(confFile, '\0') - 3, "conf");

	if(existFile(confFile))
	{
		LOGPOS();

		FILE *fp = fileOpen(confFile, "rt");
		ConfLineNo = 0;

		// ---- read items ----

		IPCountMax              = ToInt_x(ReadConfLine(fp),  20, IMAX);
		ConnectCountMax         = ToInt_x(ReadConfLine(fp),   1, IMAX);
		DiskFreeBootTime        = ToInt_x(ReadConfLine(fp), 100, IMAX);
		DiskFreeYellow          = ToInt_x(ReadConfLine(fp), 100, (int)DiskFreeBootTime);
		DiskFreeRed             = ToInt_x(ReadConfLine(fp), 100, (int)DiskFreeYellow);
		MemFreeBootTime         = ToInt_x(ReadConfLine(fp), 100, IMAX);
		MemFreePercentBootTime  = ToInt_x(ReadConfLine(fp),   1, 100);
		MemFreeYellow           = ToInt_x(ReadConfLine(fp), 100, (int)MemFreeBootTime);
		MemFreePercentYellow    = ToInt_x(ReadConfLine(fp),   1, MemFreePercentBootTime);
		MemFreeRed              = ToInt_x(ReadConfLine(fp), 100, (int)MemFreeYellow);
		MemFreePercentRed       = ToInt_x(ReadConfLine(fp),   1, MemFreePercentYellow);

		{
			char *line = ReadConfLine(fp);

			if(!_stricmp(line, "INADDR_ANY"))
			{
				LOGPOS();
				BindINetAddr = htonl(INADDR_ANY);
			}
			else
			{
				LOGPOS();
				BindINetAddr = inet_addr(line);
			}
			memFree(line);
		}

		TimeWaitEnabled           = ToInt_x(ReadConfLine(fp), 0, 1);
		TimeWaitSec               = ToInt_x(ReadConfLine(fp), 1, 3600);
		TimeWaitMax               = ToInt_x(ReadConfLine(fp), 1, IMAX);
		TimeWaitRatePower         = ToInt_x(ReadConfLine(fp), 0, 10);
		TimeWaitSleepMillisMax    = ToInt_x(ReadConfLine(fp), 0, 2000);
		RecvServiceNameTimeoutSec = ToInt_x(ReadConfLine(fp), 2, 3600);

		{
			char *line = ReadConfLine(fp);

			if(!_stricmp(line, "AUTO"))
			{
				LOGPOS();
				memFree(line);
			}
			else
			{
				LOGPOS();
				ConfigTmpDir = line;

				/*
					? ! 非ルート・フルパス
				*/
				errorCase(!isalpha(ConfigTmpDir[0]));
				errorCase(ConfigTmpDir[1] != ':');
				errorCase(ConfigTmpDir[2] != '\\');
				errorCase(!ConfigTmpDir[3]);

				errorCase(!existDir(ConfigTmpDir));
			}
		}

		SoftStopServerTimeoutSec = ToInt_x(ReadConfLine(fp), 0, IMAX);

		// ----

		errorCase(strcmp_xc(ReadConfLine(fp), "\\e")); // ? not Terminator
		errorCase(ReadConfLine(fp, 1)); // ? not EOF
		fileClose(fp);

		if(ConnectCountMax < 20)
		{
			ZanteiConnectCountMax_L = ConnectCountMax;
			ZanteiConnectCountMax_M = ConnectCountMax;
			ZanteiConnectCountMax_H = ConnectCountMax;
		}
		else
		{
			ZanteiConnectCountMax_L = m_d2i(ConnectCountMax * 0.85);
			ZanteiConnectCountMax_M = m_d2i(ConnectCountMax * 0.9);
			ZanteiConnectCountMax_H = m_d2i(ConnectCountMax * 0.95);
		}

		{
			const __int64 MEGA = 1000000;

			DiskFreeBootTime *= MEGA;
			DiskFreeYellow   *= MEGA;
			DiskFreeRed      *= MEGA;

			MemFreeBootTime *= MEGA;
			MemFreeYellow   *= MEGA;
			MemFreeRed      *= MEGA;
		}
	}
	memFree(confFile);

	cout("IPCountMax: %d\n", IPCountMax);
	cout("ZanteiConnectCountMax_L: %d\n", ZanteiConnectCountMax_L);
	cout("ZanteiConnectCountMax_M: %d\n", ZanteiConnectCountMax_M);
	cout("ZanteiConnectCountMax_H: %d\n", ZanteiConnectCountMax_H);
	cout("ConnectCountMax: %d\n", ConnectCountMax);
	cout("DiskFreeBootTime: %I64u\n", DiskFreeBootTime);
	cout("DiskFreeYellow: %I64u\n", DiskFreeYellow);
	cout("DiskFreeRed: %I64u\n", DiskFreeRed);
	cout("MemFreeBootTime: %I64u\n", MemFreeBootTime);
	cout("MemFreeYellow: %I64u\n", MemFreeYellow);
	cout("MemFreeRed: %I64u\n", MemFreeRed);
	cout("MemFreePercentBootTime: %u\n", MemFreePercentBootTime);
	cout("MemFreePercentYellow: %u\n", MemFreePercentYellow);
	cout("MemFreePercentRed: %u\n", MemFreePercentRed);
	cout("BindINetAddr: %08x\n", BindINetAddr);
	cout("TimeWaitEnabled: %d\n", TimeWaitEnabled);
	cout("TimeWaitSec: %d\n", TimeWaitSec);
	cout("TimeWaitMax: %d\n", TimeWaitMax);
	cout("TimeWaitRatePower: %d\n", TimeWaitRatePower);
	cout("TimeWaitSleepMillisMax: %d\n", TimeWaitSleepMillisMax);
	cout("RecvServiceNameTimeoutSec: %d\n", RecvServiceNameTimeoutSec);
	cout("ConfigTmpDir: %s\n", ConfigTmpDir ? ConfigTmpDir : "<AUTO>");
	cout("SoftStopServerTimeoutSec: %d\n", SoftStopServerTimeoutSec);
}
