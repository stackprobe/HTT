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
	errorCase_m(lastMemoryFree < 200000000, "�����������̋󂫂��s�����Ă��܂��B\n200MB�ȏ�̋󂫂��K�v�ł��B"); // ? under 200MB
	errorCase_m(80 < lastMemoryLoad, "�����������̋󂫂��s�����Ă��܂��B\n20%�ȏ�̋󂫂��K�v�ł��B"); // ? over 80%
	updateDiskSpace(getTempRtDir()[0]);
	errorCase_m(lastDiskFree < DiskFreeBootTime, "�n�[�h�f�B�X�N�̋󂫂��s�����Ă��܂��B\n2GB�ȏ�̋󂫂��K�v�ł��B"); // ? under 2GB

	int portno = 80;
	char *serviceFile = "..\\..\\TestService.dat";

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
