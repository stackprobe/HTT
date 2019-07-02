/*
	�����̔�r
		TargetTime < now() �� now() �� TargetTime �ɂȂ����i�܂��͌o�߂����j���ǂ����̔���Ƃ���B

	�L�[����
		ESC - �I��
		0 - �S�Ă̐ؒf��҂��Ă���I��
		r or R - ResetPeak()
*/

#include "all.h"

#define BUFFSIZE 1000000

#define IP_FILE "IP.httdat"
#define RECV_FILE "Recv.httdat"
#define SEND_FILE "Send.httdat"
#define TIGHT_FILE "Tight.httdat"

/*
	�T�[�r�X����̉����p
*/
#define WAIT_0_FILE "Wait_0.httdat"
#define WAIT_1_FILE "Wait_1.httdat"

typedef struct Connect_st
{
	int Sock;
	char *WorkDir;
	char *IP;
	time_t NextServiceExecTime;
	Service_t *Service;
}
Connect_t;

static void *Buffer;
static autoList<Connect_t *> *ConnectList;
static int StopServerEvent;
static char *ServiceName;
static int SockSignaled;
static int SockWaitMillisCount;
static int SockWaitMillis;
static int SoftStopServerFlag;
static time_t SoftStopServerTimeoutTime;
static autoList<Connect_t *> *PCCI_DisconnectList;

static int IsTight(void)
{
	int tight = GetResStatus() != RS_GREEN || ZanteiConnectCountMax_L < ConnectList->GetCount();

	if(tight)
		cout("RESOURCES TIGHT! @ %s\n", getTimeStamp());

	return tight;
}
static char *GetTightLevel(void)
{
	char *level;

	if(ZanteiConnectCountMax_H < ConnectList->GetCount())
		level = "H"; // High
	else if(ZanteiConnectCountMax_M < ConnectList->GetCount())
		level = "M"; // Middle
	else
		level = "L"; // Low

	cout("GetTightLevel()_level: %s\n", level);
	return level;
}
static void Disconnect(Connect_t *c)
{
	shutdown(c->Sock, SD_BOTH);
	closesocket(c->Sock);

	forceRemoveDir(c->WorkDir);
	DecrementIPCount(c->IP);

	memFree(c->WorkDir);
	memFree(c->IP);
	memFree(c);

	PostDisconnectClient();
}
static void DeleteFileDataBegin(char *file, int delSize)
{
	__int64 size = getFileSize(file);

	if(size == delSize)
	{
		createFile(file);
		return;
	}
	FILE *rfp = fileOpen(file, "rb");
	FILE *wfp = fileOpen(file, "r+b");

	fileSeek(rfp, SEEK_SET, delSize);

	for(__int64 remain = size - delSize; 0 < remain; )
	{
		int rwSize = (int)m_min(remain, BUFFSIZE);

		fileRead(rfp, Buffer, rwSize);
		fileWrite(wfp, Buffer, rwSize);

		remain -= rwSize;
	}
	fileClose(rfp);
	fileClose(wfp);

	setFileSize(file, size - delSize);
}
static void BeforeServiceExecSystem(void)
{
	cout("_spawnv �̑���� system ���g�p���܂��B�����x���Ȃ�܂��B@ %I64d\n", now());
}
static int Transmit(Connect_t *c) // ret: ? �ؒf
{
	int size = SockTransmit(c->Sock, Buffer, BUFFSIZE, SockWaitMillis, 0);

	if(size == -1) // ? �ؒf || �G���[
		return 1;

	addCwd(c->WorkDir);
	int retval = 0;

	if(size)
	{
		writeBlock(RECV_FILE, Buffer, size, 1);
		c->NextServiceExecTime = -1;
		SockSignaled = 1;
	}
	if(c->NextServiceExecTime < now())
	{
		if(!c->Service)
		{
			__int64 recvFSz = getFileSize(RECV_FILE);

			if(ServiceNameLenMin + 1 <= recvFSz) // ? ��M�T�C�Y�� (�ŒZ�T�[�r�X�� + closer) �ȏ� -> �L���ȃT�[�r�X�������Ă���\��������B
			{
				size = (int)m_min(recvFSz, ServiceNameLenMax + 1);
				readBlock(RECV_FILE, ServiceName, size);

				for(int index = 0; index < size; index++)
				{
					int chr = ServiceName[index];

					if('\x21' <= chr && chr <= '\x7e') // ? �T�[�r�X���Ɏg�p�ł��镶��
					{
						1; // noop
					}
					else // �T�[�r�X�������B
					{
						ServiceName[index] = '\0';
						c->Service = GetService(ServiceName);

						if(!c->Service) // ? not found
						{
							retval = 1;
							goto endFunc;
						}
						goto connected;
					}
				}
				if(ServiceNameLenMax + 1 <= recvFSz) // ? ��M�T�C�Y�� (�Œ��T�[�r�X�� + closer) �ȏ� -> ������v���Ȃ��B
				{
					retval = 1;
					goto endFunc;
				}
			}
			/*
				�^�C�g���A2�b <- �ڑ����Ă��������Ȃ��ڑ������邽�߁B
			*/
			if(getFileWriteTime(IP_FILE) + (IsTight() || SoftStopServerFlag ? 2 : RecvServiceNameTimeoutSec) < time(NULL))
			{
				retval = 1;
				goto endFunc;
			}
		}
		else
		{
connected:
			if(IsTight())
				writeLine(TIGHT_FILE, GetTightLevel());
			else
				removeFile(TIGHT_FILE);

			if(ServiceArgsHasProblem)
			{
				BeforeServiceExecSystem();
				system(c->Service->Command);
			}
			else
			{
				Service_t *s = c->Service;

				_spawnv(
					_P_WAIT,
					s->Arguments->GetElement(0),
					s->Arguments->ElementAt(0)
					);
			}

			if(!existFile(SEND_FILE))
			{
				retval = 1;
				goto endFunc;
			}
			if(!existFile(IP_FILE))
				writeLine(IP_FILE, c->IP); // 2bs
//				writeLine(IP_FILE, "Why did you kill me?"); // 2bs

			if(!existFile(RECV_FILE))
				createFile(RECV_FILE); // 2bs
		}

		{
			int delay = 2;

			if(existFile(WAIT_0_FILE))
			{
				delay = 0;
				removeFile(WAIT_0_FILE);
			}
			if(existFile(WAIT_1_FILE))
			{
				delay = 1;
				removeFile(WAIT_1_FILE);
			}
			c->NextServiceExecTime = now() + delay;
		}
	}
	__int64 sendFSz = getFileSize(SEND_FILE);

	if(1 <= sendFSz)
	{
		size = (int)m_min(BUFFSIZE, sendFSz);
		readBlock(SEND_FILE, Buffer, size);
		size = SockTransmit(c->Sock, Buffer, size, SockWaitMillis, 1);

		if(size == -1) // ? �ؒf || �G���[
		{
			retval = 1;
			goto endFunc;
		}
		if(size)
		{
			DeleteFileDataBegin(SEND_FILE, size);
			c->NextServiceExecTime = -1;
			SockSignaled = 1;
		}
	}
endFunc:
	unaddCwd();
	return retval;
}
static int IsKeepServer(void) // ret: ? �T�[�o�[�p��
{
	int stopServerReq = 0;

	if(SoftStopServerFlag)
	{
		time_t currTime = now();

		{
			static time_t lastCheckedTime;

			if(lastCheckedTime < currTime)
			{
				cout("�S�Ă̐ؒf��҂��Ă���I�����܂��B%I64d �� %I64d (%d)\n", currTime, SoftStopServerTimeoutTime, ConnectList->GetCount());
				lastCheckedTime = currTime;
			}
		}

		if(ConnectList->GetCount() <= 0)
		{
			cout("�S�Đؒf�����̂ŏI�����܂��B\n");
			return 0;
		}
		if(SoftStopServerTimeoutTime < currTime)
		{
			cout("�S�Đؒf���Ă��܂��񂪃^�C���A�E�g�Ȃ̂ŏI�����܂��B%d\n", ConnectList->GetCount());
			return 0;
		}
	}
	for(; ; )
	{
		switch(getHitKey(-1))
		{
			case -1:
				goto endKeyLoop;

			case 0x1b:
				return 0;

			case '0':
				stopServerReq = 1;

			case 'r':
			case 'R':
				ResetPeak();
				break;

			default:
				break;
		}
	}
endKeyLoop:
	if(!SoftStopServerFlag && (waitForMillis(StopServerEvent, 0) || stopServerReq)) // ? ��~�C�x���g�������B
	{
		cout("�S�Ă̐ؒf��҂��Ă���I�����܂�...\n");

		if(ConnectList->GetCount() <= 0)
			return 0;

		SoftStopServerFlag = 1;
		SoftStopServerTimeoutTime = now() + SoftStopServerTimeoutSec;
	}
	return 1;
}
static void PCCInterval_RED(void)
{
	LOGPOS();
}
static void PCCInterval(void)
{
	LOGPOS();

	if(1 <= ConnectList->GetCount())
	{
		int index = getCryptoRand() % ConnectList->GetCount(); // �����_���ɂǂꂩ�P��

		cout("PCCI: %d %d\n", index, ConnectList->GetCount());
		SockWaitMillis = 0;

		if(Transmit(ConnectList->GetElement(index))) // ? �ؒf
		{
			PCCI_DisconnectList->AddElement(ConnectList->FastDesertElement(index));
		}
	}
	LOGPOS();
}
static void PostPCCInterval(void)
{
	cout("PostPCCI: %d\n", PCCI_DisconnectList->GetCount());

	while(1 <= PCCI_DisconnectList->GetCount())
	{
		Disconnect(PCCI_DisconnectList->UnaddElement());
	}
	LOGPOS();
}
void SockServer(int portno)
{
	Buffer = memAlloc(BUFFSIZE);
	ConnectList = new autoList<Connect_t *>();
	StopServerEvent = eventOpen(UserAppUUID ? c_Join3(EVENT_STOPSERVER, " ", UserAppUUID) : EVENT_STOPSERVER);
	ServiceName = (char *)memAlloc(ServiceNameLenMax + 1);
	SockSignaled = 0;
	SockWaitMillisCount = 0;
	SockWaitMillis = 0;
	PCCI_DisconnectList = new autoList<Connect_t *>();
	ResetPeak();

	SockStartup();

	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	errorCase(sock == -1); // ? ���s

	struct sockaddr_in sa;
	memset(&sa, 0x00, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = BindINetAddr;//htonl(INADDR_ANY);
	sa.sin_port = htons((unsigned short)portno);

	int retval = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	errorCase_m(retval != 0, "�w�肳�ꂽ�|�[�g�ԍ��͎g�p���ł��B\n�|�[�g�ԍ���ύX���ĉ������B"); // ? ���s

	retval = listen(sock, SOMAXCONN);
	errorCase(retval != 0); // ? ���s

	time_t nextPeriodExecTime = -1;
	time_t nextCheckTime;
	ResStatus_t rs;
	int connFlag;

	while(IsKeepServer()) // ? not ��~
	{
		rs = GetResStatus();
		connFlag = 0;

		if(rs == RS_RED)
		{
forceDisconnection:
			cout("FORCED DISCONNECTION! @ %s\n", getTimeStamp());

			// �˔��I�ȃ��\�[�X�̕N���ł͂Ȃ����`�F�b�N
			{
				for(int t = 20; t; t--)
				{
					Sleep(100);

					if(GetResStatus() != RS_RED)
					{
						cout("REJECTED %d\n", t);
						goto endForceDisconnection;
					}
				}
			}
			LOGPOS(); // FD_START

			while(ConnectList->GetCount())
			{
				Disconnect(ConnectList->UnaddElement());
			}
			for(time_t endTime = now() + 2; now() <= endTime; )
			{
				retval = SockWait(sock, 0, 0);
				errorCase(retval == -1); // ? ���s

				if(retval) // ? �ڑ�����
				{
					PreConnectClient(PCCInterval_RED);

					struct sockaddr_in clsa;
					int sasz = sizeof(clsa);
					int clSock = accept(sock, (struct sockaddr *)&clsa, &sasz);
					errorCase(clSock == -1); // ? ���s

					closesocket(clSock);

					PostDisconnectClient();

					cout("Connection cancelled\n");
				}
				else // ? �ڑ��Ȃ�
				{
					break;
				}
			}
endForceDisconnection:
			LOGPOS(); // FD_END
		}
		else if(rs == RS_GREEN || ConnectList->GetCount() == 0)
		{
			retval = SockWait(sock, ConnectList->GetCount() ? 0 : 2000, 0);
			errorCase(retval == -1); // ? ���s

			if(retval) // ? �ڑ�����
			{
				PreConnectClient(PCCInterval);
				PostPCCInterval();

				struct sockaddr_in clsa;
				int sasz = sizeof(clsa);
				int clSock = accept(sock, (struct sockaddr *)&clsa, &sasz);
				errorCase(clSock == -1); // ? ���s

				char *dir = makeTempDir();
				addCwd(dir);
				char *ip = strx(inet_ntoa(clsa.sin_addr));
				writeLine(IP_FILE, ip);
				createFile(RECV_FILE);
				createFile(SEND_FILE);
				unaddCwd();

				Connect_t *c = nb(Connect_t);

				c->Sock = clSock;
				c->WorkDir = dir;
				c->IP = ip; // ������ c �� bind

				IncrementIPCount(ip);

				if((IsTight() ? 10 : IPCountMax) < GetIPCount(ip))
				{
					cout("Maybe DoS attacked from %s @ %s\n", ip, getTimeStamp()); // ������ ip �g�p
					Disconnect(c); // ������ ip �J��
				}
				else if(ConnectCountMax <= ConnectList->GetCount())
				{
					cout("OVERFLOW CONNECTIONS @ %s\n", getTimeStamp());
					Disconnect(c);
				}
				else if(SoftStopServerFlag)
				{
					LOGPOS();
					Disconnect(c);
				}
				else
				{
					ConnectList->AddElement(c);

					if(ConnectionsPeak < ConnectList->GetCount())
					{
						ConnectionsPeak = ConnectList->GetCount();
						cout("Peak connections %d @ %s\n", ConnectionsPeak, getTimeStamp());
					}
				}
				SockSignaled = 1;
				connFlag = 1;
			}
			goto transmission;
		}
		else // ? rs == RS_YELLOW && ConnectList->GetCount() != 0
		{
transmission:
			nextCheckTime = now() + 2;

			for(int index = 0; index < ConnectList->GetCount(); index++)
			{
				if(GetResStatus() == RS_RED)
				{
					cout("RED_TRANS_BREAK\n");
					ConnectList->Shuffle_7();
					goto forceDisconnection;
				}
				if(index == 2 && connFlag) // �b��_��ʐڑ��΍�
				{
					cout("****ZANTEI_TRANS_BREAK_1****\n");
					break;
				}
				if(nextCheckTime < now()) // �b��_��ʐڑ��΍�
				{
					cout("ZTB_CHECK: %d %d @ %s\n", index, ConnectList->GetCount(), getTimeStamp());

					retval = SockWait(sock, 0, 0);
					errorCase(retval == -1); // ? ���s

					if(retval) // ? �ڑ�����
					{
						cout("****ZANTEI_TRANS_BREAK_2****\n");
						break;
					}
					nextCheckTime = now() + 2;
				}

				SockWaitMillis = 0;

				if(!index)
				{
					if(SockSignaled)
					{
						SockSignaled = 0;
						SockWaitMillisCount = 0;
					}
					else
					{
						SockWaitMillisCount++;
						m_minim(SockWaitMillisCount, 100);
						SockWaitMillis = SockWaitMillisCount;
					}
				}

				if(Transmit(ConnectList->GetElement(index))) // ? �ؒf
				{
					Disconnect(ConnectList->FastDesertElement(index));
					index--;
				}
			}
			ConnectList->Shuffle_7(); // �����ɂ�鉽�炩�̕΂�����O...
		}
		if(nextPeriodExecTime < now()) // ? ����I�ȃT�[�r�X�̋���s
		{
			char *dir = makeTempDir();
			addCwd(dir);

			for(int index = 0; index < ServiceList->GetCount(); index++)
			{
				Service_t *s = ServiceList->GetElement(index);

				createFile(IP_FILE);
				createFile(RECV_FILE);
				createFile(SEND_FILE);

				if(IsTight())
					writeLine(TIGHT_FILE, GetTightLevel());

				if(ServiceArgsHasProblem)
				{
					BeforeServiceExecSystem();
					system(s->Command);
				}
				else
				{
					_spawnv(
						_P_WAIT,
						s->Arguments->GetElement(0),
						s->Arguments->ElementAt(0)
						);
				}
				clearDir(".");
			}
			unaddCwd();
			removeDir(dir);
			memFree(dir);

			// ���������̃`�F�b�N�Ǝ��s
			{
				static time_t lastDay;
				time_t currDay = getTime() / 86400;

				if(lastDay == 0)
					lastDay = currDay;

//				errorCase(currDay < lastDay); // 2bs // del_���v���C��������N���蓾��B

				if(lastDay < currDay)
				{
					lastDay = currDay;

					// ��������
					{
						ResetPeak();
					}
				}
			}

			nextPeriodExecTime = now() + 60;
		}
	}
	while(ConnectList->GetCount())
	{
		Disconnect(ConnectList->UnaddElement());
	}
	retval = closesocket(sock);
	errorCase(retval != 0);

	SockCleanup();

	memFree(Buffer);
	delete ConnectList;
	handleClose(StopServerEvent);
	memFree(ServiceName);
}
