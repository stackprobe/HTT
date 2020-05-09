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
			strcpy(text + c * 18, "<br/>�t�@�C��<br/>");

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
		UserAppUUID = makeUpper(UserAppUUID); // mutexName�͑啶���E��������ʂ���B(eventName���啶���E��������ʂ���I)
		cout("UserAppUUID: %s\n", UserAppUUID);
	}
	if(argIs("/T"))
	{
		int h = eventOpen(UserAppUUID ? c_Join3(EVENT_STOPSERVER, " ", UserAppUUID) : EVENT_STOPSERVER);
		eventSet(h);
		handleClose(h);
		goto endProc;
	}

#if 0 // �g��Ȃ��B
	// �N���������Ƃ�ʒm����C�x���g
	{
		int h = eventOpen(UserAppUUID ? c_Join3(EVENT_SERVERSTARTED, " ", UserAppUUID) : EVENT_SERVERSTARTED);
		eventSet(h);
		handleClose(h);
	}
#endif

	// ���d�N���h�~���b�N
	{
		ProcMtxHdl = mutexOpen(UserAppUUID ? c_Join3(PROC_MUTEX, " ", UserAppUUID) : PROC_MUTEX);

		if(!waitForMillis(ProcMtxHdl, 0)) // ? ���d�N��
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
	errorCase_m(memFreeNg && memFreePercentNg, xcout("�����������̋󂫂��s�����Ă��܂��B\n%.3f GB�ȏ�̋󂫖��� %d %%�ȏ�̋󂫂��K�v�ł��B", MemFreeBootTime / 1000000000.0, MemFreePercentBootTime));
	updateDiskSpace_Dir(getTempRtDir());
	errorCase_m(lastDiskFree_User < DiskFreeBootTime, xcout("�n�[�h�f�B�X�N�̋󂫂��s�����Ă��܂��B\n%.3f GB�ȏ�̋󂫂��K�v�ł��B", DiskFreeBootTime / 1000000000.0));

	int portno = 80;
//	char *serviceFile = "Service.dat";
	char *serviceFile = "Service.dat_"; // HTT.exe�����������ŋN�������Ⴄ�̂̓}�Y�����ȂƁB�B

	if(argIs("/P"))
	{
		portno = s2i(nextArg());
	}
	if(argIs("/S"))
	{
		serviceFile = nextArg();
	}
	errorCase(hasArgs(1)); // ? has �s���ȃR�}���h����

	errorCase(portno < 1 || 65535 < portno);
	errorCase(!existFile(serviceFile));

	LoadServiceFile(serviceFile);
	SockServer(portno);

	removeTempRtDir();

endProc:
	termination(0);
	return 0; // dummy
}
