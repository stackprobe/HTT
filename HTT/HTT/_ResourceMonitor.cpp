#include "all.h"

/*
	ret:
		     if メモリ空き < 1GB   || メモリ空き < 10% || ディスク空き < 1GB   then return RS_RED
		else if メモリ空き < 1.5GB || メモリ空き < 15% || ディスク空き < 1.5GB then return RS_YELLOW
		else                                                                  return RS_GREEN
*/
static ResStatus_t GetResStatus2(void)
{
	updateMemory();

	{
		int &peak = LowMemoryFreePeak;
		int lmf = (int)(lastMemoryFree / (50 * 1000 * 1000)) * 50;

		if(lmf < peak)
		{
			peak = lmf;
			cout("Low memory free %d [MB] @ %s\n", peak, getTimeStamp());
		}
	}

	{
		int &peak = HiMemoryLoadPeak;
		int hml = (int)(100 - lastMemoryFreePercent);

		if(peak < hml)
		{
			peak = hml;
			cout("Hi memory load %d [PCT] @ %s\n", peak, getTimeStamp());
		}
	}

	if(lastMemoryFree < MemFreeRed && lastMemoryFreePercent < MemFreePercentRed)
		return RS_RED;

	updateDiskSpace_Dir(getTempRtDir());

	{
		int &peak = LowDiskFreePeak;
		int ldf = (int)(lastDiskFree_User / (1000 * 1000 * 1000));

		if(ldf < peak)
		{
			peak = ldf;
			cout("Low disk free %d [GB] @ %s\n", peak, getTimeStamp());
		}
	}

	if(lastDiskFree_User < DiskFreeRed)
		return RS_RED;

	if(lastDiskFree_User < DiskFreeYellow || lastMemoryFree < MemFreeYellow && lastMemoryFreePercent < MemFreePercentYellow)
		return RS_YELLOW;

	return RS_GREEN;
}
static char *ResStatus2Str(ResStatus_t rs)
{
	switch(rs)
	{
	case RS_GREEN: return "GREEN";
	case RS_YELLOW: return "YELLOW";
	case RS_RED: return "RED";

	default:
		error();
	}
}
ResStatus_t GetResStatus(void)
{
	static ResStatus_t last_rs = RS_GREEN;
	ResStatus_t rs = GetResStatus2();

	if(last_rs != rs)
	{
		cout("Status changed %s -> %s @ %s\n", ResStatus2Str(last_rs), ResStatus2Str(rs), getTimeStamp());
		last_rs = rs;
	}
	return rs;
}
