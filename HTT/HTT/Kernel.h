extern uint lastMemoryFreePercent;
extern uint64 lastMemoryFree;
extern uint64 lastMemorySize;
extern uint64 lastPageFileFree;
extern uint64 lastPageFileSize;
extern uint64 lastVirtualFree;
extern uint64 lastVirtualSize;
extern uint64 lastExVirtualFree;

void updateMemory(void);

// sync > @ updateDiskSpace_h

extern uint64 lastDiskFree_User; // ���̃v���Z�X���g�p�o����󂫗̈�T�C�Y
extern uint64 lastDiskFree;
extern uint64 lastDiskSize;

void updateDiskSpace(int drive);
void updateDiskSpace_Dir(char *dir);

// < sync
