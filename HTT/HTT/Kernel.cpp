#include "all.h"

uint lastMemoryFreePercent;
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

	errorCase(!GlobalMemoryStatusEx(&ms)); // ? 失敗

	lastMemoryFreePercent = 100 - (uint)ms.dwMemoryLoad;
	lastMemoryFree = (uint64)ms.ullAvailPhys;
	lastMemorySize = (uint64)ms.ullTotalPhys;
	lastPageFileFree = (uint64)ms.ullAvailPageFile;
	lastPageFileSize = (uint64)ms.ullTotalPageFile;
	lastVirtualFree = (uint64)ms.ullAvailVirtual;
	lastVirtualSize = (uint64)ms.ullTotalVirtual;
	lastExVirtualFree = (uint64)ms.ullAvailExtendedVirtual;
}

uint64 lastDiskFree;
uint64 lastDiskFree_real;
uint64 lastDiskSize;

void updateDiskSpace_Dir(char *dir)
{
	ULARGE_INTEGER a;
	ULARGE_INTEGER f;
	ULARGE_INTEGER t;

	errorCase(m_isEmpty(dir));

	/*
		ドライブが存在しない || 準備出来ていない || ディレクトリが存在しない -> 失敗する。
	*/
	errorCase(!GetDiskFreeSpaceEx((LPCTSTR)dir, &a, &t, &f)); // ? 失敗

	lastDiskFree      = (uint64)a.LowPart | (uint64)a.HighPart << 32;
	lastDiskFree_real = (uint64)f.LowPart | (uint64)f.HighPart << 32;
	lastDiskSize      = (uint64)t.LowPart | (uint64)t.HighPart << 32;
}
