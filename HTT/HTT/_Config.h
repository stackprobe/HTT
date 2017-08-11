extern int IPCountMax;
extern int ZanteiConnectCountMax_L;
extern int ZanteiConnectCountMax_M;
extern int ZanteiConnectCountMax_H;
extern int ConnectCountMax;
extern uint64 DiskFreeBootTime;
extern uint64 DiskFreeYellow;
extern uint64 DiskFreeRed;
extern uint BindINetAddr;
extern int TimeWaitEnabled;
extern int TimeWaitSec;
extern int TimeWaitMax;
extern int TimeWaitRatePower;
extern int TimeWaitSleepMillisMax;
extern int RecvServiceNameTimeoutSec;
extern char *ConfigTmpDir;

void LoadConfig(void);
