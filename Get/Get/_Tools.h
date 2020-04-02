extern char *SendFileFullPath;
extern int RemoveSendFileAtTermination;
extern char *ClientIP;
extern int TightFlag;

void HTT_Disconnect(void);

// ---- Mutex ----

void WaitLogMutex(void);

// ---- Log ----

extern int WriteLog_Disabled;

void WriteLog(char *line);
void WriteLog_x(char *line);

// ----
