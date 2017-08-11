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
	errorCase(h == NULL); // ? é∏îs
	return (int)h;
}
void mutexRelease(int h)
{
	errorCase(!ReleaseMutex((HANDLE)h)); // ? é∏îs
}

int eventOpen(char *eventName)
{
	HANDLE h = CreateEventA(NULL, FALSE, FALSE, eventName);
	errorCase(h == NULL); // ? é∏îs
	return (int)h;
}
void eventSet(int h)
{
	errorCase(!SetEvent((HANDLE)h)); // ? é∏îs
}

int waitForMillis(int h, int millis) // ret: ? ÉçÉbÉNê¨å˜ || ÉVÉOÉiÉãéÊìæ
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
	errorCase(!CloseHandle((HANDLE)h)); // ? é∏îs
}
