#define APP_UUID "{a84d19ed-158c-4f8d-8f63-aaf315c26381}"
#define PROC_MUTEX "cerulean.charlotte HTT after-school tea time process mutex"
#define EVENT_STOPSERVER "cerulean.charlotte HTT after-school tea time server termination"
#define EVENT_SERVERSTARTED "cerulean.charlotte HTT after-school tea time server started"

extern char *UserAppUUID;

char *c_Join3(char *leftPtn, char *midPtn, char *rightPtn);

extern int LowMemoryFreePeak;
extern int HiMemoryLoadPeak;
extern int LowDiskFreePeak;
extern int ConnectionsPeak;

void ResetPeak(void);
