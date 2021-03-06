int existPath(char *path);
int existFile(char *file);
int existDir(char *dir);

void trimPath(char *path);
char *combine(char *dir, char *file);
char *combine_cx(char *dir, char *file);
char *combine_xx(char *dir, char *file);
char *combine_xc(char *dir, char *file);
char *getDir(char *path);

void setCwd(char *dir);
void setCwd_x(char *dir);
char *getCwd(void);
void addCwd(char *dir);
void unaddCwd(void);

void createFile(char *file);
void removeFile(char *file);

void moveFile(char *srcFile, char *destFile);

char *getFullPath(char *path, char *baseDir = ".");
char *getExt(char *path);

char *makeFairLocalPath(char *localPath);

void TouchFile(char *file);

void DivideFile_FP(FILE *fp_binding, size_t fileSizeLimit, char *beforeFile, char *afterFile);
