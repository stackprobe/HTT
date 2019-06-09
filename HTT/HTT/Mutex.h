int mutexOpen(char *mutexName);
void mutexRelease(int h);

int eventOpen(char *eventName);
void eventSet(int h);

int waitForMillis(int h, int millis);
void handleClose(int h);
