#include "all.h"

static int ProcMtxHdl;

static void CloseProcMtxHdl(void)
{
	mutexRelease(ProcMtxHdl);
	handleClose(ProcMtxHdl);
}
int main(int argc, char **argv)
{
#if 0 // test
	{
		static char text[18001];

		for(int c = 0; c < 1000; c++)
			strcpy(text + c * 18, "<br/>ファイル<br/>");

//		errorCase(printf("%s", text) < 0);
		cout("%s", text);
//		return 0;
	}
#endif

#if 0 // test
	{
		for(int c = 10009; c; c--) cout("%08x", getCryptoRand());
		cout("\n");
	}
#endif

	if(argIs("/U"))
	{
		UserAppUUID = nextArg();
		errorCase(m_isEmpty(UserAppUUID));
		UserAppUUID = makeUpper(UserAppUUID); // mutexNameは大文字・小文字区別する。(eventNameも大文字・小文字区別する！)
		cout("UserAppUUID: %s\n", UserAppUUID);
	}
	if(argIs("/T"))
	{
		int h = eventOpen(UserAppUUID ? c_Join3(EVENT_STOPSERVER, " ", UserAppUUID) : EVENT_STOPSERVER);
		eventSet(h);
		handleClose(h);
		goto endProc;
	}

#if 0 // 使わない。
	// 起動したことを通知するイベント
	{
		int h = eventOpen(UserAppUUID ? c_Join3(EVENT_SERVERSTARTED, " ", UserAppUUID) : EVENT_SERVERSTARTED);
		eventSet(h);
		handleClose(h);
	}
#endif

	// 多重起動防止ロック
	{
		ProcMtxHdl = mutexOpen(UserAppUUID ? c_Join3(PROC_MUTEX, " ", UserAppUUID) : PROC_MUTEX);

		if(!waitForMillis(ProcMtxHdl, 0)) // ? 多重起動
		{
			handleClose(ProcMtxHdl);
			return 1;
		}
		addFinalizer(CloseProcMtxHdl);
	}

	LoadConfig();

	updateMemory();
	int memFreeNg = lastMemoryFree < MemFreeBootTime;
	int memFreePercentNg = lastMemoryFreePercent < MemFreePercentBootTime;
	cout("memFreeNg: %d\n", memFreeNg);
	cout("memFreePercentNg: %d\n", memFreePercentNg);
	errorCase_m(memFreeNg && memFreePercentNg, xcout("物理メモリの空きが不足しています。\n%.3f GB以上の空き又は %d %%以上の空きが必要です。", MemFreeBootTime / 1000000000.0, MemFreePercentBootTime));
	updateDiskSpace_Dir(getTempRtDir());
	errorCase_m(lastDiskFree_User < DiskFreeBootTime, xcout("ハードディスクの空きが不足しています。\n%.3f GB以上の空きが必要です。", DiskFreeBootTime / 1000000000.0));

	int portno = 80;
//	char *serviceFile = "Service.dat";
	char *serviceFile = "Service.dat_"; // HTT.exeが引数無しで起動しちゃうのはマズいかなと。。

	if(argIs("/P"))
	{
		portno = s2i(nextArg());
	}
	if(argIs("/S"))
	{
		serviceFile = nextArg();
	}
	errorCase(hasArgs(1)); // ? has 不明なコマンド引数

	errorCase(portno < 1 || 65535 < portno);
	errorCase(!existFile(serviceFile));

	LoadServiceFile(serviceFile);
	SockServer(portno);

	removeTempRtDir();

endProc:
	termination(0);
	return 0; // dummy
}
