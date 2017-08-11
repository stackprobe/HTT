#include "all.h"

static int ConnectCount;
static int TimeWaitTotal;
static int *TimeWaitRing;
static time_t LastTime;
static void (*IntervalFunc)(void);

static void TimeWaitMain(int connectFlag)
{
	time_t currTime = now();

	errorCase(currTime < LastTime); // 2bs

	if(!TimeWaitRing)
		TimeWaitRing = na(int, TimeWaitSec);

	while(LastTime < currTime)
	{
		int *tw;

		if(TimeWaitTotal == 0)
		{
			cout("TW_[%I64d]_Break\n", LastTime);

			LastTime = currTime;
			break;
		}
		LastTime++;

		tw = TimeWaitRing + LastTime % TimeWaitSec;

		if(*tw)
			cout("TW_[%I64d]: %d-%d\n", LastTime, TimeWaitTotal, *tw);

		TimeWaitTotal -= *tw;
		errorCase(TimeWaitTotal < 0); // 2bs
		*tw = 0;
	}
	cout("TW_[%I64d]: %d C=%d\n", LastTime, TimeWaitTotal, ConnectCount);

	if(connectFlag)
	{
		int busyPortTotal = ConnectCount + TimeWaitTotal;
		double rate;
		int waitMillis;

		cout("TW_BUSY: %d\n", busyPortTotal);

		rate = (double)busyPortTotal;
		rate /= TimeWaitMax;
		m_range(rate, 0.0, 1.0);

		cout("TW_RATE: %1.5f\n", rate);

		rate = dPow(rate, TimeWaitRatePower);
		cout("TW_RAT2: %1.5f\n", rate);
		rate *= TimeWaitSleepMillisMax;
		cout("TW_RAT3: %1.5f\n", rate);

		waitMillis = m_d2i(rate);
		cout("TW_WAIT: %d\n", waitMillis);

		{
			const int INT_CYC = 20;

			while(INT_CYC < waitMillis)
			{
				Sleep(INT_CYC);
				waitMillis -= INT_CYC;

				LOGPOS();
				errorCase(!IntervalFunc); // 2bs
				IntervalFunc();
				LOGPOS();
			}
			Sleep(waitMillis);
		}

		ConnectCount++;
	}
	else
	{
		cout("TW_[%I64d]: %d+1\n", LastTime, TimeWaitTotal);

		TimeWaitTotal++;
		TimeWaitRing[LastTime % TimeWaitSec]++;

		errorCase(ConnectCount <= 0); // Ø’f‚Ì‰ñ”‚ª‘½‚·‚¬‚éB
		ConnectCount--;
	}
	cout("TW_%d C=%d\n", TimeWaitTotal, ConnectCount);
}
static void PCCOrPDC(int connectFlag)
{
	if(TimeWaitEnabled)
		TimeWaitMain(connectFlag);
}

void PreConnectClient(void (*interval)(void))
{
	IntervalFunc = interval;
	PCCOrPDC(1);
	IntervalFunc = NULL;
}
void PostDisconnectClient(void)
{
	PCCOrPDC(0);
}
