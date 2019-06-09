#include <wincrypt.h>
#pragma comment(lib, "ADVAPI32")

void getCryptoBlock_MS(uchar *buffer, int size);

int getCryptoByte(void);
uint getCryptoRand(void);

char *makePw(int radix = 62, int len = 22); // log62(2^128) == 21�`22

// log36(2^128) == 24�`25
#define makePw36() (makePw(36, 25))
