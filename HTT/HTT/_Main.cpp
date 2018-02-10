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
	errorCase_m(lastMemoryFree < 200000000, "物理メモリの空きが不足しています。\n200MB以上の空きが必要です。"); // ? under 200MB
	errorCase_m(80 < lastMemoryLoad, "物理メモリの空きが不足しています。\n20%以上の空きが必要です。"); // ? over 80%
	updateDiskSpace(getTempRtDir()[0]);
	errorCase_m(lastDiskFree < DiskFreeBootTime, "ハードディスクの空きが不足しています。\n2GB以上の空きが必要です。"); // ? under 2GB

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
