/*
	mutex
		int h = mutexOpen("ABC");
		waitForMillis(h, 1000);
		waitForever(h);
		mutexRelease(h);
		handleClose(h);

	event
		int h = eventOpen("ABC");
		eventSet(h);
		waitForMillis(h, 1000);
		waitForever(h);
		handleClose(h);
*/

#include "all.h"

int mutexOpen(char *mutexName)
{
	HANDLE h = CreateMutexA(NULL, FALSE, mutexName);
	errorCase(h == NULL); // ? 失敗
	return (int)h;
}
void mutexRelease(int h)
{
	errorCase(!ReleaseMutex((HANDLE)h)); // ? 失敗
}

int waitForMillis(int h, int millis) // ret: ? ロック成功 || シグナル取得
{
#if 1
	return WaitForSingleObject((HANDLE)h, millis) == WAIT_OBJECT_0;
#else
	switch(WaitForSingleObject((HANDLE)h, millis))
	{
	case WAIT_OBJECT_0:
		return 1;

	case WAIT_TIMEOUT:
		return 0;

	default:
		error();
	}
#endif
}
void waitForever(int h)
{
	errorCase(!waitForMillis(h, INFINITE));
}
void handleClose(int h)
{
	errorCase(!CloseHandle((HANDLE)h)); // ? 失敗
}
