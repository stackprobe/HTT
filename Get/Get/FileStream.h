FILE *rfopen(char *file, char *mode);
FILE *fileOpen(char *file, char *mode);
void fileClose(FILE *fp);

__int64 getFileSize(FILE *fp);
__int64 getFileSize(char *file);
void setFileSize(char *file, __int64 size);

int readChar(FILE *fp);
char *readLine(FILE *fp);
char *readLine(char *file);
autoList<char *> *readLines(char *file);

void writeChar(FILE *fp, int chr);
void writeToken(FILE *fp, char *token);
void writeLine(FILE *fp, char *line);
void writeLine_x(FILE *fp, char *line);
void writeLine(char *file, char *line);
void writeLine_cx(char *file, char *line);
void writeAddLine(char *file, char *line);
void writeLines(char *file, autoList<char *> *lines);
void writeLines_cx(char *file, autoList<char *> *lines);

void fileSeek(FILE *fp, int origin, __int64 offset);
void fileRead(FILE *fp, void *block, int size);
void fileWrite(FILE *fp, void *block, int size);

void readBlock(char *file, void *block, int size);
void writeBlock(char *file, void *block, int size, int addMode = 0);
