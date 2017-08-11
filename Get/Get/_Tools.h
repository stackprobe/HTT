extern char *SendFileFullPath;
extern int RemoveSendFileAtTermination;
extern char *ClientIP;

void HTT_Disconnect(void);

// ---- Mutex ----

void WaitLogMutex(void);

// ---- Log ----

void WriteLog(char *line);
void WriteLog_x(char *line);

// ----
