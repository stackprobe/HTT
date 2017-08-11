int mutexOpen(char *mutexName);
void mutexRelease(int h);

int eventOpen(char *eventName);
void eventSet(int h);

int waitForMillis(int h, int millis);
void waitForever(int h);
void handleClose(int h);
