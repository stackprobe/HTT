void SockStartup(void);
void SockCleanup(void);

int SockWait(int sock, int millis, int forWrite);
int SockSend(int sock, void *buffData, int dataSize);
int SockRecv(int sock, void *buffData, int dataSize);
int SockTransmit(int sock, void *buffData, int dataSize, int waitMillis, int forWrite);
