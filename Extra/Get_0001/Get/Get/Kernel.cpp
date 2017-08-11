#include "all.h"

uint lastMemoryLoad;
uint64 lastMemoryFree;
uint64 lastMemorySize;
uint64 lastPageFileFree;
uint64 lastPageFileSize;
uint64 lastVirtualFree;
uint64 lastVirtualSize;
uint64 lastExVirtualFree;

void updateMemory(void)
{
	MEMORYSTATUSEX ms = { sizeof(MEMORYSTATUSEX) };

	errorCase(!GlobalMemoryStatusEx(&ms)); // ? é∏îs

	lastMemoryLoad = (uint)ms.dwMemoryLoad;
	lastMemoryFree = (uint64)ms.ullAvailPhys;
	lastMemorySize = (uint64)ms.ullTotalPhys;
	lastPageFileFree = (uint64)ms.ullAvailPageFile;
	lastPageFileSize = (uint64)ms.ullTotalPageFile;
	lastVirtualFree = (uint64)ms.ullAvailVirtual;
	lastVirtualSize = (uint64)ms.ullTotalVirtual;
	lastExVirtualFree = (uint64)ms.ullAvailExtendedVirtual;
}

uint64 lastDiskFree;
uint64 lastDiskSize;

void updateDiskSpace(int drive)
{
	ULARGE_INTEGER a;
	ULARGE_INTEGER f;
	ULARGE_INTEGER t;

	if('a' <= drive && drive <= 'z')
		drive += 'A' - 'a';

	errorCase(drive < 'A' || 'Z' < drive);

	char dir[] = {
		drive,
		':',
		'\\',
		'\0',
	};

	/*
		ÉhÉâÉCÉuÇ™ë∂ç›ÇµÇ»Ç¢ || èÄîıèoóàÇƒÇ¢Ç»Ç¢ -> é∏îsÇ∑ÇÈÅB
	*/
	errorCase(!GetDiskFreeSpaceEx((LPCTSTR)dir, &a, &t, &f)); // ? é∏îs

	lastDiskFree = (uint64)f.LowPart | (uint64)f.HighPart << 32;
	lastDiskSize = (uint64)t.LowPart | (uint64)t.HighPart << 32;
}
