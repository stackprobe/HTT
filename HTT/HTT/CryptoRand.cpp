#include "all.h"

void getCryptoBlock_MS(uchar *buffer, int size)
{
	HCRYPTPROV hp;

	if(!CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		cout("CSP Error, LastError=%08x\n", GetLastError());
		error();
	}
	if(!CryptGenRandom(hp, size, buffer))
	{
		CryptReleaseContext(hp, 0);
		error();
	}
	CryptReleaseContext(hp, 0);
}

#define BUFFERSIZE 65536

static void GetCryptoBlock(uchar *buffer)
{
	getCryptoBlock_MS(buffer, BUFFERSIZE);
}
int getCryptoByte(void)
{
	static uchar buffer[BUFFERSIZE];
	static uint index = BUFFERSIZE;

	if(BUFFERSIZE <= index)
	{
		GetCryptoBlock(buffer);
		index = 0;
	}
	return buffer[index++];
}
uint getCryptoRand(void)
{
	uint ret;

	((uchar *)&ret)[0] = getCryptoByte();
	((uchar *)&ret)[1] = getCryptoByte();
	((uchar *)&ret)[2] = getCryptoByte();
	((uchar *)&ret)[3] = getCryptoByte();

	return ret;
}

char *makePw(int radix, int len) // radix: 1 - 62
{
	char *pw = (char *)memAlloc(len + 1);
	int modulo = 256 - 256 % radix;

	for(int index = 0; index < len; index++)
	{
		int chr;
		do {
			chr = getCryptoByte();
		}
		while(modulo <= chr);
		chr %= radix;

		if(36 <= chr)
			chr += 'A' - 36;
		else if(10 <= chr)
			chr += 'a' - 10;
		else
			chr += '0';

		pw[index] = chr;
	}
	pw[len] = '\0';
	return pw;
}
