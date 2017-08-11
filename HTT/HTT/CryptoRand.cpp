#include "all.h"

void createKeyContainer(void)
{
	HCRYPTPROV hp;

	if(CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, CRYPT_NEWKEYSET)) // エラー無視
		CryptReleaseContext(hp, 0);
}
void deleteKeyContainer(void)
{
	HCRYPTPROV hp;

	CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, CRYPT_DELETEKEYSET); // エラー無視
}
void getCryptoBlock_MS(uchar *buffer, int size)
{
#if 1 // やっぱりちゃんとしよう... @ 2017.7.4
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
#else
#if 1 // 多分ソケットのシャッフルとかにしか使ってないから、暗号論的な乱数は求めていないはず。-> 適当に初期化して返す。
	cout("GCB_MS-P.1\n");
	uint64 n = getNowTick();
	uint64 t = (uint64)time(NULL);
	uint p = (uint)buffer;
	int index;

	cout("n=%016x\n", n);
	cout("t=%016x\n", t);
	cout("p=%08x\n", p);

	index = 0;

	for(int c = 0; c < 8; c++)
		buffer[index++] = (n >> (c * 8)) & 0xff;

	for(int c = 0; c < 8; c++)
		buffer[index++] = (t >> (c * 8)) & 0xff;

	for(int c = 0; c < 4; c++)
		buffer[index++] = (p >> (c * 8)) & 0xff;

	/*
	for(int c = 0; c < 16; c++)
		buffer[index++] = "0123456789abcdef"[(n >> (c * 4)) & 0x0f];
		*/

	cout("GCB_MS-P.2\n");

	for(int index = 0; index < size; index++)
		cout("%02x", buffer[index]);

	cout("\n");
	cout("GCB_MS-P.3\n");
#else // old
	HCRYPTPROV hp;

#if 1
	cout("cac_open-key-container\n");

	if(!CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, 0))
	{
		int lastError = GetLastError();
		cout("cac_error-1: %d\n", lastError);

		if(lastError == NTE_BAD_KEYSET)
		{
			cout("cac_create-key-container\n");

			if(!CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				lastError = GetLastError();
				cout("cac_error-2: %d\n", lastError);

				error();
			}
		}
	}
	cout("cac_opened\n");
#else // old
	if(!CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, 0) &&
		(GetLastError() != NTE_BAD_KEYSET ||
		!CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL, CRYPT_NEWKEYSET)))
		error();
#endif

	if(!CryptGenRandom(hp, size, buffer))
	{
		CryptReleaseContext(hp, 0);
		error();
	}
	CryptReleaseContext(hp, 0);
#endif
#endif
}

//#define SEEDSIZE 20
//#define SEEDSIZE 300
#define SEEDSIZE 4096
#define BUFFERSIZE 64 // == sha512 hash size

static void GetCryptoBlock(uchar *buffer)
{
	static sha512_t *ctx;

	sha512_evacuate();

	if(!ctx)
	{
		uchar seed[SEEDSIZE];

		getCryptoBlock_MS(seed, SEEDSIZE);

		ctx = sha512_create();
		sha512_update(ctx, seed, SEEDSIZE);
		sha512_makeHash(ctx);
	}
	else
	{
		static uint64 counter;
		sha512_t *ictx = sha512_copy(ctx);

		sha512_update(ictx, &counter, sizeof(counter));
		sha512_makeHash(ictx);
		sha512_release(ictx);

		errorCase(counter == _UI64_MAX); // ? カンスト..有り得ないだろうけど..
		counter++;
	}
	memcpy(buffer, sha512_hash, BUFFERSIZE);
	sha512_unevacuate();
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
uint16 getCryptoRand16(void)
{
	uint16 ret;

	((uchar *)&ret)[0] = getCryptoByte();
	((uchar *)&ret)[1] = getCryptoByte();

	return ret;
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
void getCryptoBlock(uchar *block, int size)
{
	for(int index = 0; index < size; index++)
	{
		block[index] = getCryptoByte();
	}
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
