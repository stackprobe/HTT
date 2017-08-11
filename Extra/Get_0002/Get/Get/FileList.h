extern struct _finddata_t lastFindData;

autoList<char *> *ls(char *dir = ".", char *wCard = "*");
autoList<char *> *lss(char *dir = ".", char *wCard = "*");

void updateFindData(char *path);
time_t getFileWriteTime(char *file);
