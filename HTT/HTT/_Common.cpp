#include "all.h"

char *UserAppUUID; // NULL == ïségóp

char *c_Join3(char *leftPtn, char *midPtn, char *rightPtn)
{
	static char *stock;
	memFree(stock);
	return stock = xcout("%s%s%s", leftPtn, midPtn, rightPtn);
}

int LowMemoryFreePeak;
int HiMemoryLoadPeak;
int LowDiskFreePeak;
int ConnectionsPeak;

void ResetPeak(void)
{
	cout("RESET PEAK! @ %s\n", getTimeStamp());

	LowMemoryFreePeak = INT_MAX;
	HiMemoryLoadPeak = -1;
	LowDiskFreePeak = INT_MAX;
	ConnectionsPeak = 0;
}
