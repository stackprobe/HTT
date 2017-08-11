#include "all.h"

static int SockStartupDepth;
static int SockStartupOnce;

void SockStartup(void)
{
	WORD ver;
	WSADATA wsd;
	int retval;

	if(SockStartupDepth++)
		return;

	errorCase(SockStartupOnce);
	SockStartupOnce = 1;

	ver = MAKEWORD(2, 2);
	retval = WSAStartup(ver, &wsd);
	errorCase(retval);
}
void SockCleanup(void)
{
	errorCase(!SockStartupDepth);

	if(--SockStartupDepth)
		return;

	WSACleanup();
}

int SockWait(int sock, int millis, int forWrite)
{
	fd_set fs;
	struct timeval tv;
	int retval;

	FD_ZERO(&fs);
	FD_SET(sock, &fs);
	tv.tv_sec = millis / 1000;
	tv.tv_usec = (millis % 1000) * 1000;

	retval = select(sock + 1,
		forWrite ? NULL : &fs,
		forWrite ? &fs : NULL,
		NULL,
		&tv
		);

	errorCase(retval < -1 || 1 < retval);
	return retval;
}
int SockSend(int sock, void *buffData, int dataSize)
{
	return send(sock, (const char *)buffData, dataSize, 0);
}
int SockRecv(int sock, void *buffData, int dataSize)
{
	return recv(sock, (char *)buffData, dataSize, 0);
}
int SockTransmit(int sock, void *buffData, int dataSize, int waitMillis, int forWrite) // ret: 読み込んだバイト数, -1 == エラー
{
	int retval;

	if(dataSize == 0)
		return 0;

	retval = SockWait(sock, waitMillis, forWrite);

	if(retval <= 0) // ? timeout || エラー
		goto endfunc;

	retval = (forWrite ? SockSend : SockRecv)(sock, buffData, dataSize);
	errorCase(retval < -1 || dataSize < retval);

	if(retval == 0) // ? select() が 1 を返したのに、0 バイト -> sock が相手側で閉じられたときの挙動
		retval = -1;

endfunc:
	return retval;
}
